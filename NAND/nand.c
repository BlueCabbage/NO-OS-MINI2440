/*************************************************************************
    > File Name: nand.c
    > Author: ZHAOCHAO
    > Mail: 479680168@qq.com 
    > Created Time: Sat 16 Jul 2016 07:04:04 AM PDT
 ************************************************************************/

#define GSTATUS1		(*(volatile unsigned int *)0x560000B0)
#define BUSY			1


typedef unsigned int S3C24X0_REG32;


// Nand flash (see S3C2410 manual chapter 6)
typedef struct {
	S3C24X0_REG32	NFCONF;
	S3C24X0_REG32	NFCMD;
	S3C24X0_REG32	NFADDR;
	S3C24X0_REG32	NFDATA;
	S3C24X0_REG32	NFSTAT;
	S3C24X0_REG32	NFECC;
} S3C2410_NAND;

// Nand flash (see S3C2440 manual chapter 6)
typedef struct {
	S3C24X0_REG32	NFCONF;
	S3C24X0_REG32	NFCONT;
	S3C24X0_REG32	NFCMD;
	S3C24X0_REG32	NFADDR;
	S3C24X0_REG32	NFDATA;
	S3C24X0_REG32	NFMECCD0;
	S3C24X0_REG32	NFMECCD1;
	S3C24X0_REG32	NFSECCD;
	S3C24X0_REG32	NFSTAT;
	S3C24X0_REG32	NFMECC0;
	S3C24X0_REG32	NFMECC1;
	S3C24X0_REG32	NFSECC;
	S3C24X0_REG32	NFSBLK;
	S3C24X0_REG32	NFEBLK;
} S3C2440_NAND;

typedef struct {
	void (*nand_reset)(void);
	void (*wait_idle)(void);
	void (*nand_select_chip)(void);
	void (*nand_deselect_chip)(void);
	void (*write_cmd)(int cmd);
	void (*write_addr)(unsigned int addr);
	unsigned char (*read_data)(void);
} t_nand_chip;


static S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0X4E000000;
static S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0X4E000000;

static t_nand_chip nand_chip;


// call from extern function
void nand_init(void);
void nand_read(unsigned char *buf, unsigned long start_addr, int size);


// Nand flash contrl 
static void nand_reset(void);
static void wait_idle(void);
static void nand_select_chip(void);
static void nand_deselect_chip(void);
static void write_cmd(int cmd);
static void write_addr(unsigned int addr);
static unsigned char read_data(void);


// Nand flash solve function for s3c2410
static void s3c2410_nand_reset(void);
static void s3c2410_wait_idle(void);
static void s3c2410_nand_select_chip(void);
static void s3c2410_nand_deselect_chip(void);
static void s3c2410_write_cmd(int cmd);
static void s3c2410_write_addr(unsigned int addr);
static unsigned char s3c2410_read_data();


// Nand flash solve function for s3c2440
static void s3c2440_nand_reset(void);
static void s3c2440_wait_idle(void);
static void s3c2440_nand_select_chip(void);
static void s3c2440_nand_deselect_chip(void);
static void s3c2440_write_cmd(int cmd);
static void s3c2440_write_addr(unsigned int addr);
static unsigned char s3c2440_read_data();



// S3C2410
static void s3c2410_nand_reset(void)
{
	s3c2410_nand_select_chip();
	s3c2410_write_cmd(0xff);		// reset cmd
	s3c2410_wait_idle();
	s3c2410_nand_deselect_chip();
}

static void s3c2410_wait_idle(void)
{
	int i;
	volatile unsigned char *p = (volatile unsigned char *)&s3c2410nand->NFSTAT;
	while ( !( *p & BUSY ))
		for ( i = 0; i < 10; i ++ );
}

static void s3c2410_nand_select_chip(void)
{ 
	int i;
	s3c2410nand->NFCONF &= ~(1 << 11);
	for (i = 0; i < 10; i ++);
}

static void s3c2410_nand_deselect_chip(void)
{
	s3c2410nand->NFCONF |= (1 << 11);
}

static void s3c2410_write_cmd(int cmd)
{
	volatile unsigned char *p = (volatile unsigned char *)&s3c2410nand->NFCMD;
	*p = cmd;
}

static void s3c2410_write_addr(unsigned int addr)
{
	int i;
	volatile unsigned char *p = (volatile unsigned char *)&s3c2410nand->NFADDR;

	*p = addr & 0xff;
	for (i = 0; i < 10; i ++);
	*p = (addr >> 9) & 0xff;
	for (i = 0; i < 10; i ++);
	*p = (addr >> 17) & 0xff;
	for (i = 0; i < 10; i ++);
	*p = (addr >> 25) & 0xff;
	for (i = 0; i < 10; i ++);
}

static unsigned char s3c2410_read_data(void)
{
	volatile unsigned char *p = (volatile unsigned char *)&s3c2410nand->NFDATA;
	return *p;
}


// S3C2440
static void s3c2440_nand_reset(void)
{
	s3c2440_nand_select_chip();
	s3c2440_write_cmd(0xff);		// reset cmd
	s3c2440_wait_idle();
	s3c2440_nand_deselect_chip();
}

static void s3c2440_wait_idle(void)
{
	int i;
	volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFSTAT;
	while ( !( *p & BUSY ))
		for ( i = 0; i < 10; i ++ );
}

static void s3c2440_nand_select_chip(void)
{ 
	int i;
	s3c2440nand->NFCONT &= ~(1 << 1);
	for (i = 0; i < 10; i ++);
}

static void s3c2440_nand_deselect_chip(void)
{
	s3c2440nand->NFCONT |= (1 << 1);
}

static void s3c2440_write_cmd(int cmd)
{
	volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFCMD;
	*p = cmd;
}

static void s3c2440_write_addr(unsigned int addr)
{
	int i;
	volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFADDR;

	*p = addr & 0xff;
	for (i = 0; i < 10; i ++);
	*p = (addr >> 9) & 0xff;
	for (i = 0; i < 10; i ++);
	*p = (addr >> 17) & 0xff;
	for (i = 0; i < 10; i ++);
	*p = (addr >> 25) & 0xff;
	for (i = 0; i < 10; i ++);
}

static unsigned char s3c2440_read_data(void)
{
	volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFDATA;
	return *p;
}


// at the first time use nand flash, reset nand flash
static void nand_reset(void)
{
	nand_chip.nand_reset();
}

static void wait_idle(void)
{
	nand_chip.wait_idle();
}

static void nand_select_chip(void)
{
	int i;
	nand_chip.nand_select_chip();
	for(i = 0; i < 10; i ++);
}


static void nand_deselect_chip(void)
{
	nand_chip.nand_deselect_chip();
}

static void write_cmd(int cmd)
{
	nand_chip.write_cmd(cmd);
}

static void write_addr(unsigned int addr)
{
	nand_chip.write_addr(addr);
}

static unsigned char read_data(void)
{
	return nand_chip.read_data();
}




// init nand flash
void nand_init(void)
{
	#define TACLS		0
	#define TWRPH0		3
	#define TWRPH1		0

	// judge it is s3c2410 or s3c2440
	if ((GSTATUS1 == 0X32410000) || (GSTATUS1 == 0X32410002))
	{
		nand_chip.nand_reset			= s3c2410_nand_reset;
		nand_chip.wait_idle				= s3c2410_wait_idle;
		nand_chip.nand_select_chip		= s3c2410_nand_select_chip;
		nand_chip.nand_deselect_chip	= s3c2410_nand_deselect_chip;
		nand_chip.write_cmd				= s3c2410_write_cmd;
		nand_chip.write_addr			= s3c2410_write_addr;
		nand_chip.read_data				= s3c2410_read_data;

		// enable nand flash control, initial ECC, disable cs, set the clock
		s3c2410nand->NFCONF = (1<<15)|(1<<12)|(1<<11)|(TACLS<<8)|(TWRPH0<<4)|(TWRPH1<<0);

	}
	else {
		
		nand_chip.nand_reset			= s3c2440_nand_reset;
		nand_chip.wait_idle				= s3c2440_wait_idle;
		nand_chip.nand_select_chip		= s3c2440_nand_select_chip;
		nand_chip.nand_deselect_chip	= s3c2440_nand_deselect_chip;
		nand_chip.write_cmd				= s3c2440_write_cmd;
		nand_chip.write_addr			= s3c2440_write_addr;
		nand_chip.read_data				= s3c2440_read_data;

		// set the clock
		s3c2440nand->NFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4);
		//enable nand flash control, initial ECC, disable cs
		s3c2440nand->NFCONT = (1<<4)|(1<<1)|(1<<0);

	}

	// reset nand flash
	nand_reset();
}


#define NAND_SECTOR_SIZE		512
#define NAND_BLOCK_MASK			(NAND_SECTOR_SIZE - 1)

// read 
void nand_read(unsigned char *buf, unsigned long start_addr, int size)
{
	int i, j;

	if ((start_addr & NAND_BLOCK_MASK) || (size & NAND_BLOCK_MASK)) {
		return; // addr or length not align
	}

	// Select the chip
	nand_select_chip();

	for (i = start_addr; i < (start_addr + size); ) {
		write_cmd(0);		// send cmd read0
		
		write_addr(i);;
		wait_idle();

		for (j = 0; j < NAND_SECTOR_SIZE; j ++, i ++) {
			*buf = read_data();
			buf ++;
		}
	}

	// deselect the chip
	nand_deselect_chip();


	return;
}
