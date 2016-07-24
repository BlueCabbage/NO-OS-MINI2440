/*************************************************************************
    > File Name: gcclib.h
    > Author: ZHAOCHAO
    > Mail: 479680168@qq.com 
    > Created Time: Sun 24 Jul 2016 04:54:40 AM PDT
 ************************************************************************/

/* gcclib.h -- definitions for various functions 'borrowed' from gcc-2.95.3 */
/* I Molton     29/07/01 */

#define BITS_PER_UNIT  8
#define SI_TYPE_SIZE (sizeof (SItype) * BITS_PER_UNIT)

typedef unsigned int UQItype    __attribute__ ((mode (QI)));
typedef          int SItype     __attribute__ ((mode (SI)));
typedef unsigned int USItype    __attribute__ ((mode (SI)));
typedef          int DItype     __attribute__ ((mode (DI)));
typedef          int word_type 	__attribute__ ((mode (__word__)));
typedef unsigned int UDItype    __attribute__ ((mode (DI)));

#ifdef __ARMEB__
  struct DIstruct {SItype high, low;};
#else
  struct DIstruct {SItype low, high;};
#endif

typedef union
{
  struct DIstruct s;
  DItype ll;
} DIunion;

