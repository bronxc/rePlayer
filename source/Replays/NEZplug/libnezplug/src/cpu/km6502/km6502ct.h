/*

 HuC6280 clock cycle table

 -0         undefined OP-code
 BRK(#$00)  +7 by interrupt

*/

#if BUILD_HUC6280 && !defined(KM6502CT_H_HUC6280)
#define KM6502_CT_H_HUC6280
static const Ubyte km6280_cl_table[256] = {
/* L 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F     H */
	 1, 7, 3, 4, 6, 4, 6, 7, 3, 2, 2,-0, 7, 5, 7, 6, /* 0 */
	 2, 7, 7, 4, 6, 4, 6, 7, 2, 5, 2,-0, 7, 5, 7, 6, /* 1 */
	 7, 7, 3, 4, 4, 4, 6, 7, 3, 2, 2,-0, 5, 5, 7, 6, /* 2 */
	 2, 7, 7,-0, 4, 4, 6, 7, 2, 5, 2,-0, 5, 5, 7, 6, /* 3 */
	 7, 7, 3, 4, 8, 4, 6, 7, 3, 2, 2,-0, 4, 5, 7, 6, /* 4 */
	 2, 7, 7, 5,2 , 4, 6, 7, 2, 5, 3,-0,-0, 5, 7, 6, /* 5 */
	 7, 7, 2,-0, 4, 4, 6, 7, 3, 2, 2,-0, 7, 5, 7, 6, /* 6 */
	 2, 7, 7,17, 4, 4, 6, 7, 2, 5, 3,-0, 7, 5, 7, 6, /* 7 */
	 2, 7, 2, 7, 4, 4, 4, 7, 2, 2, 2,-0, 5, 5, 5, 6, /* 8 */
	 2, 7, 7, 8, 4, 4, 4, 7, 2, 5, 2,-0, 5, 5, 5, 6, /* 9 */
	 2, 7, 2, 7, 4, 4, 4, 7, 2, 2, 2,-0, 5, 5, 5, 6, /* A */
	 2, 7, 7, 8, 4, 4, 4, 7, 2, 5, 2,-0, 5, 5, 5, 6, /* B */
	 2, 7, 2,17, 4, 4, 6, 7, 2, 2, 2,-0, 5, 5, 7, 6, /* C */
	 2, 7, 7,17,2 , 4, 6, 7, 2, 5, 3,-0,-0, 5, 7, 6, /* D */
	 2, 7,-0,17, 4, 4, 6, 7, 2, 2, 2,-0, 5, 5, 7, 6, /* E */
	 2, 7, 7,17, 2, 4, 6, 7, 2, 5, 3,-0,-0, 5, 7, 6, /* F */
};
#endif

/*

 m65c02 clock cycle table (incomplete)

 -0         undefined OP-code
 +n         +1 by page boundary
 BRK(#$00)  +7 by interrupt

*/
#if BUILD_M65C02 && !defined(KM6502CT_H_M65C02)
#define KM6502_CT_H_M65C02
static const Ubyte km65c02_cl_table[256] = {
/* L 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F     H */
	 0, 6,-0,-0,-0, 3, 5,-0, 3, 2, 2,-0,-0, 4, 6,-0, /* 0 */
	 2,+5,-0,-0,-0, 4, 6,-0, 2,+4, 2,-0,-0,+4, 7,-0, /* 1 */
	 6, 6,-0,-0, 3, 3, 5,-0, 4, 2, 2,-0, 4, 4, 6,-0, /* 2 */
	 2,+5,-0,-0, 4, 4, 6,-0, 2,+4, 2,-0,+4,+4, 7,-0, /* 3 */
	 6, 6,-0,-0,-0, 3, 5,-0, 3, 2, 2,-0, 3, 4, 6,-0, /* 4 */
	 2,+5,-0,-0,-0, 4, 6,-0, 2,+4,-0,-0,-0,+4, 7,-0, /* 5 */
	 6, 6,-0,-0,-0, 3, 5,-0, 4, 2, 2,-0, 5, 4, 6,-0, /* 6 */
	 2,+5,-0,-0,-0, 4, 6,-0, 2,+4,-0,-0,-0,+4, 7,-0, /* 7 */
	 2, 6,-0,-0, 3, 3, 3,-0, 2,-0, 2,-0, 4, 4, 4,-0, /* 8 */
	 2, 6,-0,-0, 4, 4, 4,-0, 2, 5, 2,-0,-0, 5,-0,-0, /* 9 */
	 2, 6, 2,-0, 3, 3, 3,-0, 2, 2, 2,-0, 4, 4, 4,-0, /* A */
	 2,+5,-0,-0, 4, 4, 4,-0, 2,+4, 2,-0,+4,+4,+4,-0, /* B */
	 2, 6,-0,-0, 3, 3, 5,-0, 2, 2, 2,-0, 4, 4, 6,-0, /* C */
	 2,+5,-0,-0,-0, 4, 6,-0, 2,+4,-0,-0,-0,+4, 7,-0, /* D */
	 2, 6,-0,-0, 3, 3, 5,-0, 2, 2, 2,-0, 4, 4, 6,-0, /* E */
	 2,+5,-0,-0,-0, 4, 6,-0, 2,+4,-0,-0,-0,+4, 7,-0, /* F */
};
#endif

/*

 m6502 clock cycle table

 (n)        undefined OP-code
 +n         +1 by page boundary case
 BRK(#$00)  +7 by interrupt

*/
#ifndef KM6502CT_H_
#define KM6502CT_H_
static const Ubyte km6502_cl_table[256] = {
/* L 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F      H */
	 0 , 6 ,(2),(8),(3), 3 , 5 ,(5), 3 , 2 , 2 ,(2),(4), 4 , 6 ,(6), /* 0 */
	 2 ,+5 ,(2),(8),(4), 4 , 6 ,(6), 2 ,+4 ,(2),(7),(5),+4 , 7 ,(7), /* 1 */
	 6 , 6 ,(2),(8), 3 , 3 , 5 ,(5), 4 , 2 , 2 ,(2), 4 , 4 , 6 ,(6), /* 2 */
	 2 ,+5 ,(2),(8),(4), 4 , 6 ,(6), 2 ,+4 ,(2),(7),(5),+4 , 7 ,(7), /* 3 */
	 6 , 6 ,(2),(8),(3), 3 , 5 ,(5), 3 , 2 , 2 ,(2), 3 , 4 , 6 ,(6), /* 4 */
	 2 ,+5 ,(2),(8),(4), 4 , 6 ,(6), 2 ,+4 ,(2),(7),(5),+4 , 7 ,(7), /* 5 */
	 6 , 6 ,(2),(8),(3), 3 , 5 ,(5), 4 , 2 , 2 ,(2), 5 , 4 , 6 ,(6), /* 6 */
	 2 ,+5 ,(2),(8),(4), 4 , 6 ,(6), 2 ,+4 ,(2),(7),(5),+4 , 7 ,(7), /* 7 */
	(2), 6 ,(2),(6), 3 , 3 , 3 ,(3), 2 ,(2), 2 ,(2), 4 , 4 , 4 ,(4), /* 8 */
	 2 , 6 ,(2),(6), 4 , 4 , 4 ,(4), 2 , 5 , 2 ,(5),(5), 5 ,(5),(5), /* 9 */
	 2 , 6 , 2 ,(6), 3 , 3 , 3 ,(3), 2 , 2 , 2 ,(2), 4 , 4 , 4 ,(4), /* A */
	 2 ,+5 ,(2),(5), 4 , 4 , 4 ,(4), 2 ,+4 , 2 ,(5),+4 ,+4 ,+4 ,(4), /* B */
	 2 , 6 ,(2),(8), 3 , 3 , 5 ,(5), 2 , 2 , 2 ,(2), 4 , 4 , 6 ,(6), /* C */
	 2 ,+5 ,(2),(8),(4), 4 , 6 ,(6), 2 ,+4 ,(2),(7),(5),+4 , 7 ,(7), /* D */
	 2 , 6 ,(2),(8), 3 , 3 , 5 ,(5), 2 , 2 , 2 ,(2), 4 , 4 , 6 ,(6), /* E */
	 2 ,+5 ,(2),(8),(4), 4 , 6 ,(6), 2 ,+4 ,(2),(7),(5),+4 , 7 ,(7), /* F */
};
#endif

#if BUILD_HUC6280
#define cl_table km6280_cl_table
#elif BUILD_M65C02
#define cl_table km65c02_cl_table
#else
#define cl_table km6502_cl_table
#endif
