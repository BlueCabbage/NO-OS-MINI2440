/*************************************************************************
    > File Name: nand.c
    > Author: ZHAOCHAO
    > Mail: 479680168@qq.com 
    > Created Time: Sat 16 Jul 2016 07:04:04 AM PDT
 ************************************************************************/

// NAND Flash registers
#define NFCONF		(*(volatile unsigned long *)0x4e000000) // Nand flash config register
#define NFCONT		(*(volatile unsigned long *)0x4e000004)	// Nand flash control register
#define NFCMD		(*(volatile unsigned long *)0x4e000008) // Nand flash command register
#define NFADDR		(*(volatile unsigned long *)0x4e00000C) // Nand flash address register
#define NFDATA		(*(volatile unsigned char *)0x4e000010) // Nand flash data register
#define NFSTAT		(*(volatile unsigned long *)0x4e000020) // Nand flash state register


// Standard Nand flash commands

#define NAND_CMD_READ0		0
#define NAND_CMD_READ1		1
#define NAND_CMD_RNDOUT		5
#define NAND_CMD_PAGEPROG	0x10
#define NAND_CMD_READOOB	0x50
#define NAND_CMD_ERASE1		0x60
#define NAND_CMD_STATUS		0x70
#define NAND_CMD_STATUS_MULTI	0x71
#define NAND_CMD_SEQIN		0x80
#define NAND_CMD_RNDIN		0x85
#define NAND_CMD_READID		0x90
#define NAND_CMD_ERASE2		0xd0
#define NAND_CMD_RESET		0xff

/* Extended commands for large page devices */
#define NAND_CMD_READSTART		0x30
#define NAND_CMD_RNDOUTSTART	0xE0
#define NAND_CMD_CACHEDPROG		0x15

#define NF_CMD(cmd)			(NFCMD=cmd)
#define NF_ADDR(addr)		(NFADDR=addr)
#define NF_RDDATA8			(NFDATA)
#define NF_ENABLE_CE		(NFCONT &= ~(1<<1))		// Enable CS
#define NF_DISABLE_CE		(NFCONT |= (1<<1))		// Disable CS
#define	NF_CLEAR_RB			(NFSTAT |= (1<<2))		// Clear R/B bit
#define NF_DETECT_RB		do { while (!(NFSTAT & (1<<2)));} while(0)



static void nand_reset(void)
{
	NF_ENABLE_CE;
	NF_CLEAR_RB;
	NF_CMD(NAND_CMD_RESET);
	NF_DETECT_RB;
	NF_DISABLE_CE;
}


static int nand_read_page(unsigned char *buf, unsigned long addr, int large_block)
{
	int i;
	int page_size = 512;

	if (large_block) page_size = 2048;

	NF_ENABLE_CE;
	NF_CLEAR_RB;

	NF_CMD(NAND_CMD_READ0);

	NF_ADDR(0X0);
	if (large_block) NF_ADDR(0X0);

	NF_ADDR(addr & 0xff);
	NF_ADDR((addr >> 9) & 0xFF);
	NF_ADDR((addr >> 17) & 0xFF);
	NF_ADDR((addr >> 25) & 0xFF);

	if (large_block) NF_CMD(NAND_CMD_READSTART);

	NF_DETECT_RB;

	for (i = 0; i < page_size; i ++ ) {
		buf[i] = NF_RDDATA8;
	}

	NF_DISABLE_CE;

	return 0;
}

static int nand_read_blocks(unsigned long dst_addr, unsigned long size, int large_block) 
{
	int i;
	unsigned char *buf = (unsigned char *)dst_addr;
	unsigned int page_shift = 9;

	if (large_block) page_shift = 11;

	for (i = 0; i < (size >> page_shift); i++, buf += (1<<page_shift)) {
		nand_read_page(buf, i, large_block);
	}

	return 0;
}


int copy_to_ram_from_nand(void) 
{
	int large_block = 0;
	int i;
	volatile unsigned char id;

	NF_ENABLE_CE;
	NF_CMD(NAND_CMD_READID);
	NF_ADDR(0X0);

	for (i = 0; i < 200; i ++ ); // temp delay
	
	id = NF_RDDATA8;	// Maker Code
	id = NF_RDDATA8;	// Device Code

	nand_reset();

	if (id > 0x80) {
		large_block = 1;
	}

	return nand_read_blocks((unsigned int)0x30000000, (unsigned int)0x100000, large_block);
}

