
/*
 * riscv32.md -- RISCV-32bit back-end specification
 *
 * register usage:
 *   x0   always zero
 *   x1   proc/func return address (caller-save)
 *   x2   stack pointer (callee-save)
 *   x3   global pointer
 *   x4   thread pointer
 *   x5   temporary register (caller-save)
 *   x6   temporary register (caller-save)
 *   x7   temporary register (caller-save)
 *   x8   frame pointer (callee-save)
 *   x9   register variable  (callee-save)
 *   x10  return value
 *   x11  return value
 *   x12  functions argument
 *   x13  functions argument
 *   x14  functions argument
 *   x15  functions argument
 *   x16  functions argument
 *   x17  functions argument
 *   x18  register variable  (callee-save)
 *   x19  register variable  (callee-save)
 *   x20  register variable  (callee-save)
 *   x21  register variable  (callee-save)
 *   x22  register variable  (callee-save)
 *   x23  register variable  (callee-save)
 *   x24  register variable  (callee-save)
 *   x25  register variable  (callee-save)
 *   x26  register variable  (callee-save)
 *   x27  register variable  (callee-save)
 *   x28   temporary register (caller-save)
 *   x29   temporary register (caller-save)
 *   x30   temporary register (caller-save)
 *   x31   temporary register (caller-save)
 * caller-save registers are not preserved across procedure calls
 * callee-save registers are preserved across procedure calls
 *
 * tree grammar terminals produced by:
 *   ops c=1 s=2 i=4 l=4 h=4 f=4 d=4 x=4 p=4
 */

#include "c.h"

#define NODEPTR_TYPE	Node
#define OP_LABEL(p)	((p)->op)
#define LEFT_CHILD(p)	((p)->kids[0])
#define RIGHT_CHILD(p)	((p)->kids[1])
#define STATE_LABEL(p)	((p)->x.state)

static void address(Symbol, Symbol, long);
static void defaddress(Symbol);
static void defconst(int, int, Value);
static void defstring(int, char *);
static void defsymbol(Symbol);
static void export(Symbol);
static void function(Symbol, Symbol [], Symbol [], int);
static void global(Symbol);
static void import(Symbol);
static void local(Symbol);
static void progbeg(int, char * []);
static void progend(void);
static void segment(int);
static void space(int);
static Symbol rmap(int);
static void blkfetch(int, int, int, int);
static void blkstore(int, int, int, int);
static void blkloop(int, int, int, int, int, int []);
static void emit2(Node);
static void doarg(Node);
static void target(Node);
static void clobber(Node);
static int mulops_calls(int op);
static int move100(Node p);
static int movehard(Node p);
extern void stabend(Coordinate *, Symbol, Coordinate **, Symbol *, Symbol *); 

#define INTTMP	0x700000E0
#define INTVAR	0x0FFC0000
#define INTRET	0x00000400
#define FLTTMP	0xF00000FF
#define FLTVAR	0x0FFC0300
#define FLTRET	0x00000400

static Symbol ireg[32];
static Symbol iregw;
static Symbol freg[32];
static Symbol fregw;
static Symbol blkreg;
static int tmpregs[] = { 11, 6, 7 };
static int hardfloat = 0;

/*
generated at Mon Dec 20 14:19:37 2021
by $Id$
*/
static void _kids(NODEPTR_TYPE, int, NODEPTR_TYPE[]);
static void _label(NODEPTR_TYPE);
static int _rule(void*, int);

#define _stmt_NT 1
#define _reg_NT 2
#define _con_NT 3
#define _lab_NT 4
#define _addr_NT 5
#define _freg_NT 6
#define _cons12_NT 7
#define _con5_NT 8

static char *_ntname[] = {
	0,
	"stmt",
	"reg",
	"con",
	"lab",
	"addr",
	"freg",
	"cons12",
	"con5",
	0
};

struct _state {
	short cost[9];
	struct {
		unsigned int _stmt:6;
		unsigned int _reg:7;
		unsigned int _con:3;
		unsigned int _lab:1;
		unsigned int _addr:2;
		unsigned int _freg:4;
		unsigned int _cons12:1;
		unsigned int _con5:1;
	} rule;
};

static short _nts_0[] = { 0 };
static short _nts_1[] = { _reg_NT, 0 };
static short _nts_2[] = { _lab_NT, 0 };
static short _nts_3[] = { _con_NT, 0 };
static short _nts_4[] = { _addr_NT, _reg_NT, 0 };
static short _nts_5[] = { _addr_NT, _freg_NT, 0 };
static short _nts_6[] = { _addr_NT, 0 };
static short _nts_7[] = { _reg_NT, _reg_NT, 0 };
static short _nts_8[] = { _reg_NT, _cons12_NT, 0 };
static short _nts_9[] = { _reg_NT, _con5_NT, 0 };
static short _nts_10[] = { _freg_NT, 0 };
static short _nts_11[] = { _freg_NT, _freg_NT, 0 };

static short *_nts[] = {
	0,	/* 0 */
	_nts_0,	/* 1 */
	_nts_0,	/* 2 */
	_nts_0,	/* 3 */
	_nts_0,	/* 4 */
	_nts_0,	/* 5 */
	_nts_0,	/* 6 */
	_nts_0,	/* 7 */
	_nts_1,	/* 8 */
	_nts_1,	/* 9 */
	_nts_1,	/* 10 */
	_nts_1,	/* 11 */
	_nts_1,	/* 12 */
	_nts_1,	/* 13 */
	_nts_1,	/* 14 */
	_nts_0,	/* 15 */
	_nts_0,	/* 16 */
	_nts_0,	/* 17 */
	_nts_0,	/* 18 */
	_nts_0,	/* 19 */
	_nts_0,	/* 20 */
	_nts_0,	/* 21 */
	_nts_1,	/* 22 */
	_nts_0,	/* 23 */
	_nts_2,	/* 24 */
	_nts_3,	/* 25 */
	_nts_0,	/* 26 */
	_nts_0,	/* 27 */
	_nts_0,	/* 28 */
	_nts_0,	/* 29 */
	_nts_0,	/* 30 */
	_nts_0,	/* 31 */
	_nts_0,	/* 32 */
	_nts_1,	/* 33 */
	_nts_0,	/* 34 */
	_nts_0,	/* 35 */
	_nts_0,	/* 36 */
	_nts_0,	/* 37 */
	_nts_4,	/* 38 */
	_nts_4,	/* 39 */
	_nts_4,	/* 40 */
	_nts_4,	/* 41 */
	_nts_4,	/* 42 */
	_nts_4,	/* 43 */
	_nts_4,	/* 44 */
	_nts_4,	/* 45 */
	_nts_5,	/* 46 */
	_nts_6,	/* 47 */
	_nts_6,	/* 48 */
	_nts_6,	/* 49 */
	_nts_6,	/* 50 */
	_nts_6,	/* 51 */
	_nts_6,	/* 52 */
	_nts_6,	/* 53 */
	_nts_6,	/* 54 */
	_nts_6,	/* 55 */
	_nts_6,	/* 56 */
	_nts_6,	/* 57 */
	_nts_6,	/* 58 */
	_nts_6,	/* 59 */
	_nts_6,	/* 60 */
	_nts_6,	/* 61 */
	_nts_7,	/* 62 */
	_nts_7,	/* 63 */
	_nts_7,	/* 64 */
	_nts_7,	/* 65 */
	_nts_7,	/* 66 */
	_nts_7,	/* 67 */
	_nts_0,	/* 68 */
	_nts_7,	/* 69 */
	_nts_8,	/* 70 */
	_nts_7,	/* 71 */
	_nts_8,	/* 72 */
	_nts_7,	/* 73 */
	_nts_8,	/* 74 */
	_nts_7,	/* 75 */
	_nts_8,	/* 76 */
	_nts_7,	/* 77 */
	_nts_8,	/* 78 */
	_nts_7,	/* 79 */
	_nts_8,	/* 80 */
	_nts_7,	/* 81 */
	_nts_8,	/* 82 */
	_nts_7,	/* 83 */
	_nts_8,	/* 84 */
	_nts_7,	/* 85 */
	_nts_8,	/* 86 */
	_nts_7,	/* 87 */
	_nts_8,	/* 88 */
	_nts_7,	/* 89 */
	_nts_8,	/* 90 */
	_nts_7,	/* 91 */
	_nts_8,	/* 92 */
	_nts_1,	/* 93 */
	_nts_1,	/* 94 */
	_nts_1,	/* 95 */
	_nts_0,	/* 96 */
	_nts_9,	/* 97 */
	_nts_7,	/* 98 */
	_nts_9,	/* 99 */
	_nts_7,	/* 100 */
	_nts_9,	/* 101 */
	_nts_7,	/* 102 */
	_nts_9,	/* 103 */
	_nts_7,	/* 104 */
	_nts_1,	/* 105 */
	_nts_1,	/* 106 */
	_nts_1,	/* 107 */
	_nts_1,	/* 108 */
	_nts_1,	/* 109 */
	_nts_1,	/* 110 */
	_nts_1,	/* 111 */
	_nts_1,	/* 112 */
	_nts_1,	/* 113 */
	_nts_1,	/* 114 */
	_nts_0,	/* 115 */
	_nts_2,	/* 116 */
	_nts_1,	/* 117 */
	_nts_7,	/* 118 */
	_nts_7,	/* 119 */
	_nts_7,	/* 120 */
	_nts_7,	/* 121 */
	_nts_7,	/* 122 */
	_nts_7,	/* 123 */
	_nts_7,	/* 124 */
	_nts_7,	/* 125 */
	_nts_7,	/* 126 */
	_nts_7,	/* 127 */
	_nts_7,	/* 128 */
	_nts_7,	/* 129 */
	_nts_2,	/* 130 */
	_nts_2,	/* 131 */
	_nts_2,	/* 132 */
	_nts_2,	/* 133 */
	_nts_2,	/* 134 */
	_nts_2,	/* 135 */
	_nts_1,	/* 136 */
	_nts_1,	/* 137 */
	_nts_1,	/* 138 */
	_nts_1,	/* 139 */
	_nts_1,	/* 140 */
	_nts_1,	/* 141 */
	_nts_1,	/* 142 */
	_nts_1,	/* 143 */
	_nts_1,	/* 144 */
	_nts_1,	/* 145 */
	_nts_10,	/* 146 */
	_nts_1,	/* 147 */
	_nts_1,	/* 148 */
	_nts_1,	/* 149 */
	_nts_1,	/* 150 */
	_nts_10,	/* 151 */
	_nts_1,	/* 152 */
	_nts_7,	/* 153 */
	_nts_0,	/* 154 */
	_nts_1,	/* 155 */
	_nts_0,	/* 156 */
	_nts_10,	/* 157 */
	_nts_11,	/* 158 */
	_nts_11,	/* 159 */
	_nts_11,	/* 160 */
	_nts_11,	/* 161 */
	_nts_10,	/* 162 */
	_nts_10,	/* 163 */
	_nts_10,	/* 164 */
	_nts_1,	/* 165 */
	_nts_10,	/* 166 */
	_nts_11,	/* 167 */
	_nts_11,	/* 168 */
	_nts_11,	/* 169 */
	_nts_11,	/* 170 */
	_nts_11,	/* 171 */
	_nts_11,	/* 172 */
	_nts_7,	/* 173 */
	_nts_7,	/* 174 */
	_nts_7,	/* 175 */
	_nts_7,	/* 176 */
	_nts_1,	/* 177 */
	_nts_1,	/* 178 */
	_nts_1,	/* 179 */
	_nts_1,	/* 180 */
	_nts_1,	/* 181 */
	_nts_7,	/* 182 */
	_nts_7,	/* 183 */
	_nts_7,	/* 184 */
	_nts_7,	/* 185 */
	_nts_7,	/* 186 */
	_nts_7,	/* 187 */
};

static char *_templates[] = {
/* 0 */	0,
/* 1 */	"# read register\n",	/* reg: INDIRI1(VREGP) */
/* 2 */	"# read register\n",	/* reg: INDIRI2(VREGP) */
/* 3 */	"# read register\n",	/* reg: INDIRI4(VREGP) */
/* 4 */	"# read register\n",	/* reg: INDIRP4(VREGP) */
/* 5 */	"# read register\n",	/* reg: INDIRU1(VREGP) */
/* 6 */	"# read register\n",	/* reg: INDIRU2(VREGP) */
/* 7 */	"# read register\n",	/* reg: INDIRU4(VREGP) */
/* 8 */	"# write register\n",	/* stmt: ASGNI1(VREGP,reg) */
/* 9 */	"# write register\n",	/* stmt: ASGNI2(VREGP,reg) */
/* 10 */	"# write register\n",	/* stmt: ASGNI4(VREGP,reg) */
/* 11 */	"# write register\n",	/* stmt: ASGNP4(VREGP,reg) */
/* 12 */	"# write register\n",	/* stmt: ASGNU1(VREGP,reg) */
/* 13 */	"# write register\n",	/* stmt: ASGNU2(VREGP,reg) */
/* 14 */	"# write register\n",	/* stmt: ASGNU4(VREGP,reg) */
/* 15 */	"%a",	/* con: CNSTI1 */
/* 16 */	"%a",	/* con: CNSTI2 */
/* 17 */	"%a",	/* con: CNSTI4 */
/* 18 */	"%a",	/* con: CNSTP4 */
/* 19 */	"%a",	/* con: CNSTU1 */
/* 20 */	"%a",	/* con: CNSTU2 */
/* 21 */	"%a",	/* con: CNSTU4 */
/* 22 */	"",	/* stmt: reg */
/* 23 */	"%a",	/* lab: ADDRGP4 */
/* 24 */	"\tla x%c,%0\n",	/* reg: lab */
/* 25 */	"\tli x%c,%0\n",	/* reg: con */
/* 26 */	"# reg\n",	/* reg: CNSTI1 */
/* 27 */	"# reg\n",	/* reg: CNSTI2 */
/* 28 */	"# reg\n",	/* reg: CNSTI4 */
/* 29 */	"# reg\n",	/* reg: CNSTU1 */
/* 30 */	"# reg\n",	/* reg: CNSTU2 */
/* 31 */	"# reg\n",	/* reg: CNSTU4 */
/* 32 */	"# reg\n",	/* reg: CNSTP4 */
/* 33 */	"0(x%0)",	/* addr: reg */
/* 34 */	"%a+%F(x8)",	/* addr: ADDRFP4 */
/* 35 */	"%a+%F(x8)",	/* addr: ADDRLP4 */
/* 36 */	"\taddi x%c,x8,%a+%F\n",	/* reg: ADDRFP4 */
/* 37 */	"\taddi x%c,x8,%a+%F\n",	/* reg: ADDRLP4 */
/* 38 */	"\tsb x%1,%0\n",	/* stmt: ASGNI1(addr,reg) */
/* 39 */	"\tsh x%1,%0\n",	/* stmt: ASGNI2(addr,reg) */
/* 40 */	"\tsw x%1,%0\n",	/* stmt: ASGNI4(addr,reg) */
/* 41 */	"\tsw x%1,%0\n",	/* stmt: ASGNP4(addr,reg) */
/* 42 */	"\tsb x%1,%0\n",	/* stmt: ASGNU1(addr,reg) */
/* 43 */	"\tsh x%1,%0\n",	/* stmt: ASGNU2(addr,reg) */
/* 44 */	"\tsw x%1,%0\n",	/* stmt: ASGNU4(addr,reg) */
/* 45 */	"\tsw x%1,%0\n",	/* stmt: ASGNF4(addr,reg) */
/* 46 */	"\tfsw f%1,%0\n",	/* stmt: ASGNF4(addr,freg) */
/* 47 */	"\tlb x%c,%0\n",	/* reg: INDIRI1(addr) */
/* 48 */	"\tlh x%c,%0\n",	/* reg: INDIRI2(addr) */
/* 49 */	"\tlw x%c,%0\n",	/* reg: INDIRI4(addr) */
/* 50 */	"\tlw x%c,%0\n",	/* reg: INDIRP4(addr) */
/* 51 */	"\tlbu x%c,%0\n",	/* reg: INDIRU1(addr) */
/* 52 */	"\tlhu x%c,%0\n",	/* reg: INDIRU2(addr) */
/* 53 */	"\tlw x%c,%0\n",	/* reg: INDIRU4(addr) */
/* 54 */	"\tlw x%c,%0\n",	/* reg: INDIRF4(addr) */
/* 55 */	"\tflw f%c,%0\n",	/* freg: INDIRF4(addr) */
/* 56 */	"\tlb x%c,%0\n",	/* reg: CVII4(INDIRI1(addr)) */
/* 57 */	"\tlh x%c,%0\n",	/* reg: CVII4(INDIRI2(addr)) */
/* 58 */	"\tlbu x%c,%0\n",	/* reg: CVUU4(INDIRU1(addr)) */
/* 59 */	"\tlhu x%c,%0\n",	/* reg: CVUU4(INDIRU2(addr)) */
/* 60 */	"\tlbu x%c,%0\n",	/* reg: CVUI4(INDIRU1(addr)) */
/* 61 */	"\tlhu x%c,%0\n",	/* reg: CVUI4(INDIRU2(addr)) */
/* 62 */	"\tdiv x%c,x%0,x%1\n",	/* reg: DIVI4(reg,reg) */
/* 63 */	"\tdivu x%c,x%0,x%1\n",	/* reg: DIVU4(reg,reg) */
/* 64 */	"\trem x%c,x%0,x%1\n",	/* reg: MODI4(reg,reg) */
/* 65 */	"\tremu x%c,x%0,x%1\n",	/* reg: MODU4(reg,reg) */
/* 66 */	"\tmul x%c,x%0,x%1\n",	/* reg: MULI4(reg,reg) */
/* 67 */	"\tmul x%c,x%0,x%1\n",	/* reg: MULU4(reg,reg) */
/* 68 */	"%a",	/* cons12: CNSTI4 */
/* 69 */	"\tadd x%c,x%0,x%1\n",	/* reg: ADDI4(reg,reg) */
/* 70 */	"\taddi x%c,x%0,%1\n",	/* reg: ADDI4(reg,cons12) */
/* 71 */	"\tadd x%c,x%0,x%1\n",	/* reg: ADDP4(reg,reg) */
/* 72 */	"\taddi x%c,x%0,%1\n",	/* reg: ADDP4(reg,CVIU4(cons12)) */
/* 73 */	"\tadd x%c,x%0,x%1\n",	/* reg: ADDU4(reg,reg) */
/* 74 */	"\taddi x%c,x%0,%1\n",	/* reg: ADDU4(reg,CVIU4(cons12)) */
/* 75 */	"\tand x%c,x%0,x%1\n",	/* reg: BANDI4(reg,reg) */
/* 76 */	"\tandi x%c,x%0,%1\n",	/* reg: BANDI4(reg,cons12) */
/* 77 */	"\tor x%c,x%0,x%1\n",	/* reg: BORI4(reg,reg) */
/* 78 */	"\tori x%c,x%0,%1\n",	/* reg: BORI4(reg,cons12) */
/* 79 */	"\txor x%c,x%0,x%1\n",	/* reg: BXORI4(reg,reg) */
/* 80 */	"\txori x%c,x%0,%1\n",	/* reg: BXORI4(reg,cons12) */
/* 81 */	"\tand x%c,x%0,x%1\n",	/* reg: BANDU4(reg,reg) */
/* 82 */	"\tandi x%c,x%0,%1\n",	/* reg: BANDU4(reg,CVIU4(cons12)) */
/* 83 */	"\tor x%c,x%0,x%1\n",	/* reg: BORU4(reg,reg) */
/* 84 */	"\tori x%c,x%0,%1\n",	/* reg: BORU4(reg,CVIU4(cons12)) */
/* 85 */	"\txor x%c,x%0,x%1\n",	/* reg: BXORU4(reg,reg) */
/* 86 */	"\txori x%c,x%0,%1\n",	/* reg: BXORU4(reg,CVIU4(cons12)) */
/* 87 */	"\tsub x%c,x%0,x%1\n",	/* reg: SUBI4(reg,reg) */
/* 88 */	"\taddi x%c,x%0,-%1\n",	/* reg: SUBI4(reg,cons12) */
/* 89 */	"\tsub x%c,x%0,x%1\n",	/* reg: SUBP4(reg,reg) */
/* 90 */	"\taddi x%c,x%0,-%1\n",	/* reg: SUBP4(reg,cons12) */
/* 91 */	"\tsub x%c,x%0,x%1\n",	/* reg: SUBU4(reg,reg) */
/* 92 */	"\taddi x%c,x%0,-%1\n",	/* reg: SUBU4(reg,CVIU4(cons12)) */
/* 93 */	"\tsub x%c,x0,x%0\n",	/* reg: NEGI4(reg) */
/* 94 */	"\txori x%c,x%0,-1\n",	/* reg: BCOMI4(reg) */
/* 95 */	"\txori x%c,x%0,-1\n",	/* reg: BCOMU4(reg) */
/* 96 */	"%a",	/* con5: CNSTU4 */
/* 97 */	"\tslli x%c,x%0,%1\n",	/* reg: LSHI4(reg,CVUI4(con5)) */
/* 98 */	"\tsll x%c,x%0,x%1\n",	/* reg: LSHI4(reg,reg) */
/* 99 */	"\tslli x%c,x%0,%1\n",	/* reg: LSHU4(reg,con5) */
/* 100 */	"\tsll x%c,x%0,x%1\n",	/* reg: LSHU4(reg,reg) */
/* 101 */	"\tsrai x%c,x%0,%1\n",	/* reg: RSHI4(reg,CVUI4(con5)) */
/* 102 */	"\tsra x%c,x%0,x%1\n",	/* reg: RSHI4(reg,reg) */
/* 103 */	"\tsrli x%c,x%0,%1\n",	/* reg: RSHU4(reg,con5) */
/* 104 */	"\tsrl x%c,x%0,x%1\n",	/* reg: RSHU4(reg,reg) */
/* 105 */	"\taddi x%c,x%0,0\n",	/* reg: LOADI1(reg) */
/* 106 */	"\taddi x%c,x%0,0\n",	/* reg: LOADU1(reg) */
/* 107 */	"\taddi x%c,x%0,0\n",	/* reg: LOADI2(reg) */
/* 108 */	"\taddi x%c,x%0,0\n",	/* reg: LOADU2(reg) */
/* 109 */	"\taddi x%c,x%0,0\n",	/* reg: LOADI4(reg) */
/* 110 */	"\taddi x%c,x%0,0\n",	/* reg: LOADP4(reg) */
/* 111 */	"\taddi x%c,x%0,0\n",	/* reg: LOADU4(reg) */
/* 112 */	"\tslli x%c,x%0,8*(4-%a)\n\tsrai x%c,x%c,8*(4-%a)\n",	/* reg: CVII4(reg) */
/* 113 */	"\tslli x%c,x%0,8*(4-%a)\n\tsrli x%c,x%c,8*(4-%a)\n",	/* reg: CVUI4(reg) */
/* 114 */	"\tslli x%c,x%0,8*(4-%a)\n\tsrli x%c,x%c,8*(4-%a)\n",	/* reg: CVUU4(reg) */
/* 115 */	"%a:\n",	/* stmt: LABELV */
/* 116 */	"\tjal x0,%0\n",	/* stmt: JUMPV(lab) */
/* 117 */	"\tjalr x0,x%0,0\n",	/* stmt: JUMPV(reg) */
/* 118 */	"\tbeq x%0,x%1,%a\n",	/* stmt: EQI4(reg,reg) */
/* 119 */	"\tbeq x%0,x%1,%a\n",	/* stmt: EQU4(reg,reg) */
/* 120 */	"\tbge x%0,x%1,%a\n",	/* stmt: GEI4(reg,reg) */
/* 121 */	"\tbgeu x%0,x%1,%a\n",	/* stmt: GEU4(reg,reg) */
/* 122 */	"\tbgt x%0,x%1,%a\n",	/* stmt: GTI4(reg,reg) */
/* 123 */	"\tbgtu x%0,x%1,%a\n",	/* stmt: GTU4(reg,reg) */
/* 124 */	"\tble x%0,x%1,%a\n",	/* stmt: LEI4(reg,reg) */
/* 125 */	"\tbleu x%0,x%1,%a\n",	/* stmt: LEU4(reg,reg) */
/* 126 */	"\tblt x%0,x%1,%a\n",	/* stmt: LTI4(reg,reg) */
/* 127 */	"\tbltu x%0,x%1,%a\n",	/* stmt: LTU4(reg,reg) */
/* 128 */	"\tbne x%0,x%1,%a\n",	/* stmt: NEI4(reg,reg) */
/* 129 */	"\tbne x%0,x%1,%a\n",	/* stmt: NEU4(reg,reg) */
/* 130 */	"\tjal x1,%0\n",	/* reg: CALLF4(lab) */
/* 131 */	"\tjal x1,%0\n",	/* freg: CALLF4(lab) */
/* 132 */	"\tjal x1,%0\n",	/* reg: CALLI4(lab) */
/* 133 */	"\tjal x1,%0\n",	/* reg: CALLP4(lab) */
/* 134 */	"\tjal x1,%0\n",	/* reg: CALLU4(lab) */
/* 135 */	"\tjal x1,%0\n",	/* stmt: CALLV(lab) */
/* 136 */	"\tjalr x1,x%0,0\n",	/* reg: CALLF4(reg) */
/* 137 */	"\tjalr x1,x%0,0\n",	/* reg: CALLI4(reg) */
/* 138 */	"\tjalr x1,x%0,0\n",	/* reg: CALLP4(reg) */
/* 139 */	"\tjalr x1,x%0,0\n",	/* reg: CALLU4(reg) */
/* 140 */	"\tjalr x1,x%0,0\n",	/* stmt: CALLV(reg) */
/* 141 */	"# ret\n",	/* stmt: RETI4(reg) */
/* 142 */	"# ret\n",	/* stmt: RETP4(reg) */
/* 143 */	"# ret\n",	/* stmt: RETU4(reg) */
/* 144 */	"# ret\n",	/* stmt: RETV(reg) */
/* 145 */	"# ret\n",	/* stmt: RETF4(reg) */
/* 146 */	"# ret\n",	/* stmt: RETF4(freg) */
/* 147 */	"# arg\n",	/* stmt: ARGI4(reg) */
/* 148 */	"# arg\n",	/* stmt: ARGP4(reg) */
/* 149 */	"# arg\n",	/* stmt: ARGU4(reg) */
/* 150 */	"# arg\n",	/* stmt: ARGF4(reg) */
/* 151 */	"# arg\n",	/* stmt: ARGF4(freg) */
/* 152 */	"# argb %0\n",	/* stmt: ARGB(INDIRB(reg)) */
/* 153 */	"# asgnb %0 %1\n",	/* stmt: ASGNB(reg,INDIRB(reg)) */
/* 154 */	"# read register\n",	/* reg: INDIRF4(VREGP) */
/* 155 */	"# write register\n",	/* stmt: ASGNF4(VREGP,reg) */
/* 156 */	"# read register\n",	/* freg: INDIRF4(VREGP) */
/* 157 */	"# write register\n",	/* stmt: ASGNF4(VREGP,freg) */
/* 158 */	"\tfsub.s f%c, f%0, f%1\n",	/* freg: SUBF4(freg,freg) */
/* 159 */	"\tfmul.s f%c, f%0, f%1\n",	/* freg: MULF4(freg,freg) */
/* 160 */	"\tfdiv.s f%c, f%0, f%1\n",	/* freg: DIVF4(freg,freg) */
/* 161 */	"\tfadd.s f%c, f%0, f%1\n",	/* freg: ADDF4(freg,freg) */
/* 162 */	"\tfsgnj.s f%c, f%0, f%0\n",	/* freg: LOADF4(freg) */
/* 163 */	"\tfsgnjn.s f%c, f%0, f%0\n",	/* freg: NEGF4(freg) */
/* 164 */	"\t",	/* freg: CVFF4(freg) */
/* 165 */	"\tfcvt.s.w f%c, x%0\n",	/* freg: CVIF4(reg) */
/* 166 */	"\tfcvt.w.s x%c, f%0\n",	/* reg: CVFI4(freg) */
/* 167 */	"\tfeq.s x11,f%0,f%1\n\tbne x11,x0,%a\n",	/* stmt: EQF4(freg,freg) */
/* 168 */	"\tfeq.s x11,f%0,f%1\n\tbeq x11,x0,%a\n",	/* stmt: NEF4(freg,freg) */
/* 169 */	"\tfle.s x11,f%0,f%1\n\tbne x11,x0,%a\n",	/* stmt: LEF4(freg,freg) */
/* 170 */	"\tfle.s x11,f%0,f%1\n\tbeq x11,x0,%a\n",	/* stmt: GTF4(freg,freg) */
/* 171 */	"\tflt.s x11,f%0,f%1\n\tbne x11,x0,%a\n",	/* stmt: LTF4(freg,freg) */
/* 172 */	"\tflt.s x11,f%0,f%1\n\tbeq x11,x0,%a\n",	/* stmt: GEF4(freg,freg) */
/* 173 */	"\tjal x1,float32_add\n",	/* reg: ADDF4(reg,reg) */
/* 174 */	"\tjal x1,float32_sub\n",	/* reg: SUBF4(reg,reg) */
/* 175 */	"\tjal x1,float32_mul\n",	/* reg: MULF4(reg,reg) */
/* 176 */	"\tjal x1,float32_div\n",	/* reg: DIVF4(reg,reg) */
/* 177 */	"\taddi x%c,x%0,0\n",	/* reg: LOADF4(reg) */
/* 178 */	"\tjal x1,float32_neg\n",	/* reg: NEGF4(reg) */
/* 179 */	"\t",	/* reg: CVFF4(reg) */
/* 180 */	"\tjal x1,int32_to_float32\n",	/* reg: CVIF4(reg) */
/* 181 */	"\tjal x1,float32_to_int32\n",	/* reg: CVFI4(reg) */
/* 182 */	"\tjal x1,float32_eq\n\tbne x10,x0,%a\n",	/* stmt: EQF4(reg,reg) */
/* 183 */	"\tjal x1,float32_le\n\tbne x10,x0,%a\n",	/* stmt: LEF4(reg,reg) */
/* 184 */	"\tjal x1,float32_lt\n\tbne x10,x0,%a\n",	/* stmt: LTF4(reg,reg) */
/* 185 */	"\tjal x1,float32_ge\n\tbne x10,x0,%a\n",	/* stmt: GEF4(reg,reg) */
/* 186 */	"\tjal x1,float32_gt\n\tbne x10,x0,%a\n",	/* stmt: GTF4(reg,reg) */
/* 187 */	"\tjal x1,float32_ne\n\tbne x10,x0,%a\n",	/* stmt: NEF4(reg,reg) */
};

static char _isinstruction[] = {
/* 0 */	0,
/* 1 */	1,	/* # read register\n */
/* 2 */	1,	/* # read register\n */
/* 3 */	1,	/* # read register\n */
/* 4 */	1,	/* # read register\n */
/* 5 */	1,	/* # read register\n */
/* 6 */	1,	/* # read register\n */
/* 7 */	1,	/* # read register\n */
/* 8 */	1,	/* # write register\n */
/* 9 */	1,	/* # write register\n */
/* 10 */	1,	/* # write register\n */
/* 11 */	1,	/* # write register\n */
/* 12 */	1,	/* # write register\n */
/* 13 */	1,	/* # write register\n */
/* 14 */	1,	/* # write register\n */
/* 15 */	0,	/* %a */
/* 16 */	0,	/* %a */
/* 17 */	0,	/* %a */
/* 18 */	0,	/* %a */
/* 19 */	0,	/* %a */
/* 20 */	0,	/* %a */
/* 21 */	0,	/* %a */
/* 22 */	0,	/*  */
/* 23 */	0,	/* %a */
/* 24 */	1,	/* \tla x%c,%0\n */
/* 25 */	1,	/* \tli x%c,%0\n */
/* 26 */	1,	/* # reg\n */
/* 27 */	1,	/* # reg\n */
/* 28 */	1,	/* # reg\n */
/* 29 */	1,	/* # reg\n */
/* 30 */	1,	/* # reg\n */
/* 31 */	1,	/* # reg\n */
/* 32 */	1,	/* # reg\n */
/* 33 */	0,	/* 0(x%0) */
/* 34 */	0,	/* %a+%F(x8) */
/* 35 */	0,	/* %a+%F(x8) */
/* 36 */	1,	/* \taddi x%c,x8,%a+%F\n */
/* 37 */	1,	/* \taddi x%c,x8,%a+%F\n */
/* 38 */	1,	/* \tsb x%1,%0\n */
/* 39 */	1,	/* \tsh x%1,%0\n */
/* 40 */	1,	/* \tsw x%1,%0\n */
/* 41 */	1,	/* \tsw x%1,%0\n */
/* 42 */	1,	/* \tsb x%1,%0\n */
/* 43 */	1,	/* \tsh x%1,%0\n */
/* 44 */	1,	/* \tsw x%1,%0\n */
/* 45 */	1,	/* \tsw x%1,%0\n */
/* 46 */	1,	/* \tfsw f%1,%0\n */
/* 47 */	1,	/* \tlb x%c,%0\n */
/* 48 */	1,	/* \tlh x%c,%0\n */
/* 49 */	1,	/* \tlw x%c,%0\n */
/* 50 */	1,	/* \tlw x%c,%0\n */
/* 51 */	1,	/* \tlbu x%c,%0\n */
/* 52 */	1,	/* \tlhu x%c,%0\n */
/* 53 */	1,	/* \tlw x%c,%0\n */
/* 54 */	1,	/* \tlw x%c,%0\n */
/* 55 */	1,	/* \tflw f%c,%0\n */
/* 56 */	1,	/* \tlb x%c,%0\n */
/* 57 */	1,	/* \tlh x%c,%0\n */
/* 58 */	1,	/* \tlbu x%c,%0\n */
/* 59 */	1,	/* \tlhu x%c,%0\n */
/* 60 */	1,	/* \tlbu x%c,%0\n */
/* 61 */	1,	/* \tlhu x%c,%0\n */
/* 62 */	1,	/* \tdiv x%c,x%0,x%1\n */
/* 63 */	1,	/* \tdivu x%c,x%0,x%1\n */
/* 64 */	1,	/* \trem x%c,x%0,x%1\n */
/* 65 */	1,	/* \tremu x%c,x%0,x%1\n */
/* 66 */	1,	/* \tmul x%c,x%0,x%1\n */
/* 67 */	1,	/* \tmul x%c,x%0,x%1\n */
/* 68 */	0,	/* %a */
/* 69 */	1,	/* \tadd x%c,x%0,x%1\n */
/* 70 */	1,	/* \taddi x%c,x%0,%1\n */
/* 71 */	1,	/* \tadd x%c,x%0,x%1\n */
/* 72 */	1,	/* \taddi x%c,x%0,%1\n */
/* 73 */	1,	/* \tadd x%c,x%0,x%1\n */
/* 74 */	1,	/* \taddi x%c,x%0,%1\n */
/* 75 */	1,	/* \tand x%c,x%0,x%1\n */
/* 76 */	1,	/* \tandi x%c,x%0,%1\n */
/* 77 */	1,	/* \tor x%c,x%0,x%1\n */
/* 78 */	1,	/* \tori x%c,x%0,%1\n */
/* 79 */	1,	/* \txor x%c,x%0,x%1\n */
/* 80 */	1,	/* \txori x%c,x%0,%1\n */
/* 81 */	1,	/* \tand x%c,x%0,x%1\n */
/* 82 */	1,	/* \tandi x%c,x%0,%1\n */
/* 83 */	1,	/* \tor x%c,x%0,x%1\n */
/* 84 */	1,	/* \tori x%c,x%0,%1\n */
/* 85 */	1,	/* \txor x%c,x%0,x%1\n */
/* 86 */	1,	/* \txori x%c,x%0,%1\n */
/* 87 */	1,	/* \tsub x%c,x%0,x%1\n */
/* 88 */	1,	/* \taddi x%c,x%0,-%1\n */
/* 89 */	1,	/* \tsub x%c,x%0,x%1\n */
/* 90 */	1,	/* \taddi x%c,x%0,-%1\n */
/* 91 */	1,	/* \tsub x%c,x%0,x%1\n */
/* 92 */	1,	/* \taddi x%c,x%0,-%1\n */
/* 93 */	1,	/* \tsub x%c,x0,x%0\n */
/* 94 */	1,	/* \txori x%c,x%0,-1\n */
/* 95 */	1,	/* \txori x%c,x%0,-1\n */
/* 96 */	0,	/* %a */
/* 97 */	1,	/* \tslli x%c,x%0,%1\n */
/* 98 */	1,	/* \tsll x%c,x%0,x%1\n */
/* 99 */	1,	/* \tslli x%c,x%0,%1\n */
/* 100 */	1,	/* \tsll x%c,x%0,x%1\n */
/* 101 */	1,	/* \tsrai x%c,x%0,%1\n */
/* 102 */	1,	/* \tsra x%c,x%0,x%1\n */
/* 103 */	1,	/* \tsrli x%c,x%0,%1\n */
/* 104 */	1,	/* \tsrl x%c,x%0,x%1\n */
/* 105 */	1,	/* \taddi x%c,x%0,0\n */
/* 106 */	1,	/* \taddi x%c,x%0,0\n */
/* 107 */	1,	/* \taddi x%c,x%0,0\n */
/* 108 */	1,	/* \taddi x%c,x%0,0\n */
/* 109 */	1,	/* \taddi x%c,x%0,0\n */
/* 110 */	1,	/* \taddi x%c,x%0,0\n */
/* 111 */	1,	/* \taddi x%c,x%0,0\n */
/* 112 */	1,	/* \tslli x%c,x%0,8*(4-%a)\n\tsrai x%c,x%c,8*(4-%a)\n */
/* 113 */	1,	/* \tslli x%c,x%0,8*(4-%a)\n\tsrli x%c,x%c,8*(4-%a)\n */
/* 114 */	1,	/* \tslli x%c,x%0,8*(4-%a)\n\tsrli x%c,x%c,8*(4-%a)\n */
/* 115 */	1,	/* %a:\n */
/* 116 */	1,	/* \tjal x0,%0\n */
/* 117 */	1,	/* \tjalr x0,x%0,0\n */
/* 118 */	1,	/* \tbeq x%0,x%1,%a\n */
/* 119 */	1,	/* \tbeq x%0,x%1,%a\n */
/* 120 */	1,	/* \tbge x%0,x%1,%a\n */
/* 121 */	1,	/* \tbgeu x%0,x%1,%a\n */
/* 122 */	1,	/* \tbgt x%0,x%1,%a\n */
/* 123 */	1,	/* \tbgtu x%0,x%1,%a\n */
/* 124 */	1,	/* \tble x%0,x%1,%a\n */
/* 125 */	1,	/* \tbleu x%0,x%1,%a\n */
/* 126 */	1,	/* \tblt x%0,x%1,%a\n */
/* 127 */	1,	/* \tbltu x%0,x%1,%a\n */
/* 128 */	1,	/* \tbne x%0,x%1,%a\n */
/* 129 */	1,	/* \tbne x%0,x%1,%a\n */
/* 130 */	1,	/* \tjal x1,%0\n */
/* 131 */	1,	/* \tjal x1,%0\n */
/* 132 */	1,	/* \tjal x1,%0\n */
/* 133 */	1,	/* \tjal x1,%0\n */
/* 134 */	1,	/* \tjal x1,%0\n */
/* 135 */	1,	/* \tjal x1,%0\n */
/* 136 */	1,	/* \tjalr x1,x%0,0\n */
/* 137 */	1,	/* \tjalr x1,x%0,0\n */
/* 138 */	1,	/* \tjalr x1,x%0,0\n */
/* 139 */	1,	/* \tjalr x1,x%0,0\n */
/* 140 */	1,	/* \tjalr x1,x%0,0\n */
/* 141 */	1,	/* # ret\n */
/* 142 */	1,	/* # ret\n */
/* 143 */	1,	/* # ret\n */
/* 144 */	1,	/* # ret\n */
/* 145 */	1,	/* # ret\n */
/* 146 */	1,	/* # ret\n */
/* 147 */	1,	/* # arg\n */
/* 148 */	1,	/* # arg\n */
/* 149 */	1,	/* # arg\n */
/* 150 */	1,	/* # arg\n */
/* 151 */	1,	/* # arg\n */
/* 152 */	1,	/* # argb %0\n */
/* 153 */	1,	/* # asgnb %0 %1\n */
/* 154 */	1,	/* # read register\n */
/* 155 */	1,	/* # write register\n */
/* 156 */	1,	/* # read register\n */
/* 157 */	1,	/* # write register\n */
/* 158 */	1,	/* \tfsub.s f%c, f%0, f%1\n */
/* 159 */	1,	/* \tfmul.s f%c, f%0, f%1\n */
/* 160 */	1,	/* \tfdiv.s f%c, f%0, f%1\n */
/* 161 */	1,	/* \tfadd.s f%c, f%0, f%1\n */
/* 162 */	1,	/* \tfsgnj.s f%c, f%0, f%0\n */
/* 163 */	1,	/* \tfsgnjn.s f%c, f%0, f%0\n */
/* 164 */	0,	/* \t */
/* 165 */	1,	/* \tfcvt.s.w f%c, x%0\n */
/* 166 */	1,	/* \tfcvt.w.s x%c, f%0\n */
/* 167 */	1,	/* \tfeq.s x11,f%0,f%1\n\tbne x11,x0,%a\n */
/* 168 */	1,	/* \tfeq.s x11,f%0,f%1\n\tbeq x11,x0,%a\n */
/* 169 */	1,	/* \tfle.s x11,f%0,f%1\n\tbne x11,x0,%a\n */
/* 170 */	1,	/* \tfle.s x11,f%0,f%1\n\tbeq x11,x0,%a\n */
/* 171 */	1,	/* \tflt.s x11,f%0,f%1\n\tbne x11,x0,%a\n */
/* 172 */	1,	/* \tflt.s x11,f%0,f%1\n\tbeq x11,x0,%a\n */
/* 173 */	1,	/* \tjal x1,float32_add\n */
/* 174 */	1,	/* \tjal x1,float32_sub\n */
/* 175 */	1,	/* \tjal x1,float32_mul\n */
/* 176 */	1,	/* \tjal x1,float32_div\n */
/* 177 */	1,	/* \taddi x%c,x%0,0\n */
/* 178 */	1,	/* \tjal x1,float32_neg\n */
/* 179 */	0,	/* \t */
/* 180 */	1,	/* \tjal x1,int32_to_float32\n */
/* 181 */	1,	/* \tjal x1,float32_to_int32\n */
/* 182 */	1,	/* \tjal x1,float32_eq\n\tbne x10,x0,%a\n */
/* 183 */	1,	/* \tjal x1,float32_le\n\tbne x10,x0,%a\n */
/* 184 */	1,	/* \tjal x1,float32_lt\n\tbne x10,x0,%a\n */
/* 185 */	1,	/* \tjal x1,float32_ge\n\tbne x10,x0,%a\n */
/* 186 */	1,	/* \tjal x1,float32_gt\n\tbne x10,x0,%a\n */
/* 187 */	1,	/* \tjal x1,float32_ne\n\tbne x10,x0,%a\n */
};

static char *_string[] = {
/* 0 */	0,
/* 1 */	"reg: INDIRI1(VREGP)",
/* 2 */	"reg: INDIRI2(VREGP)",
/* 3 */	"reg: INDIRI4(VREGP)",
/* 4 */	"reg: INDIRP4(VREGP)",
/* 5 */	"reg: INDIRU1(VREGP)",
/* 6 */	"reg: INDIRU2(VREGP)",
/* 7 */	"reg: INDIRU4(VREGP)",
/* 8 */	"stmt: ASGNI1(VREGP,reg)",
/* 9 */	"stmt: ASGNI2(VREGP,reg)",
/* 10 */	"stmt: ASGNI4(VREGP,reg)",
/* 11 */	"stmt: ASGNP4(VREGP,reg)",
/* 12 */	"stmt: ASGNU1(VREGP,reg)",
/* 13 */	"stmt: ASGNU2(VREGP,reg)",
/* 14 */	"stmt: ASGNU4(VREGP,reg)",
/* 15 */	"con: CNSTI1",
/* 16 */	"con: CNSTI2",
/* 17 */	"con: CNSTI4",
/* 18 */	"con: CNSTP4",
/* 19 */	"con: CNSTU1",
/* 20 */	"con: CNSTU2",
/* 21 */	"con: CNSTU4",
/* 22 */	"stmt: reg",
/* 23 */	"lab: ADDRGP4",
/* 24 */	"reg: lab",
/* 25 */	"reg: con",
/* 26 */	"reg: CNSTI1",
/* 27 */	"reg: CNSTI2",
/* 28 */	"reg: CNSTI4",
/* 29 */	"reg: CNSTU1",
/* 30 */	"reg: CNSTU2",
/* 31 */	"reg: CNSTU4",
/* 32 */	"reg: CNSTP4",
/* 33 */	"addr: reg",
/* 34 */	"addr: ADDRFP4",
/* 35 */	"addr: ADDRLP4",
/* 36 */	"reg: ADDRFP4",
/* 37 */	"reg: ADDRLP4",
/* 38 */	"stmt: ASGNI1(addr,reg)",
/* 39 */	"stmt: ASGNI2(addr,reg)",
/* 40 */	"stmt: ASGNI4(addr,reg)",
/* 41 */	"stmt: ASGNP4(addr,reg)",
/* 42 */	"stmt: ASGNU1(addr,reg)",
/* 43 */	"stmt: ASGNU2(addr,reg)",
/* 44 */	"stmt: ASGNU4(addr,reg)",
/* 45 */	"stmt: ASGNF4(addr,reg)",
/* 46 */	"stmt: ASGNF4(addr,freg)",
/* 47 */	"reg: INDIRI1(addr)",
/* 48 */	"reg: INDIRI2(addr)",
/* 49 */	"reg: INDIRI4(addr)",
/* 50 */	"reg: INDIRP4(addr)",
/* 51 */	"reg: INDIRU1(addr)",
/* 52 */	"reg: INDIRU2(addr)",
/* 53 */	"reg: INDIRU4(addr)",
/* 54 */	"reg: INDIRF4(addr)",
/* 55 */	"freg: INDIRF4(addr)",
/* 56 */	"reg: CVII4(INDIRI1(addr))",
/* 57 */	"reg: CVII4(INDIRI2(addr))",
/* 58 */	"reg: CVUU4(INDIRU1(addr))",
/* 59 */	"reg: CVUU4(INDIRU2(addr))",
/* 60 */	"reg: CVUI4(INDIRU1(addr))",
/* 61 */	"reg: CVUI4(INDIRU2(addr))",
/* 62 */	"reg: DIVI4(reg,reg)",
/* 63 */	"reg: DIVU4(reg,reg)",
/* 64 */	"reg: MODI4(reg,reg)",
/* 65 */	"reg: MODU4(reg,reg)",
/* 66 */	"reg: MULI4(reg,reg)",
/* 67 */	"reg: MULU4(reg,reg)",
/* 68 */	"cons12: CNSTI4",
/* 69 */	"reg: ADDI4(reg,reg)",
/* 70 */	"reg: ADDI4(reg,cons12)",
/* 71 */	"reg: ADDP4(reg,reg)",
/* 72 */	"reg: ADDP4(reg,CVIU4(cons12))",
/* 73 */	"reg: ADDU4(reg,reg)",
/* 74 */	"reg: ADDU4(reg,CVIU4(cons12))",
/* 75 */	"reg: BANDI4(reg,reg)",
/* 76 */	"reg: BANDI4(reg,cons12)",
/* 77 */	"reg: BORI4(reg,reg)",
/* 78 */	"reg: BORI4(reg,cons12)",
/* 79 */	"reg: BXORI4(reg,reg)",
/* 80 */	"reg: BXORI4(reg,cons12)",
/* 81 */	"reg: BANDU4(reg,reg)",
/* 82 */	"reg: BANDU4(reg,CVIU4(cons12))",
/* 83 */	"reg: BORU4(reg,reg)",
/* 84 */	"reg: BORU4(reg,CVIU4(cons12))",
/* 85 */	"reg: BXORU4(reg,reg)",
/* 86 */	"reg: BXORU4(reg,CVIU4(cons12))",
/* 87 */	"reg: SUBI4(reg,reg)",
/* 88 */	"reg: SUBI4(reg,cons12)",
/* 89 */	"reg: SUBP4(reg,reg)",
/* 90 */	"reg: SUBP4(reg,cons12)",
/* 91 */	"reg: SUBU4(reg,reg)",
/* 92 */	"reg: SUBU4(reg,CVIU4(cons12))",
/* 93 */	"reg: NEGI4(reg)",
/* 94 */	"reg: BCOMI4(reg)",
/* 95 */	"reg: BCOMU4(reg)",
/* 96 */	"con5: CNSTU4",
/* 97 */	"reg: LSHI4(reg,CVUI4(con5))",
/* 98 */	"reg: LSHI4(reg,reg)",
/* 99 */	"reg: LSHU4(reg,con5)",
/* 100 */	"reg: LSHU4(reg,reg)",
/* 101 */	"reg: RSHI4(reg,CVUI4(con5))",
/* 102 */	"reg: RSHI4(reg,reg)",
/* 103 */	"reg: RSHU4(reg,con5)",
/* 104 */	"reg: RSHU4(reg,reg)",
/* 105 */	"reg: LOADI1(reg)",
/* 106 */	"reg: LOADU1(reg)",
/* 107 */	"reg: LOADI2(reg)",
/* 108 */	"reg: LOADU2(reg)",
/* 109 */	"reg: LOADI4(reg)",
/* 110 */	"reg: LOADP4(reg)",
/* 111 */	"reg: LOADU4(reg)",
/* 112 */	"reg: CVII4(reg)",
/* 113 */	"reg: CVUI4(reg)",
/* 114 */	"reg: CVUU4(reg)",
/* 115 */	"stmt: LABELV",
/* 116 */	"stmt: JUMPV(lab)",
/* 117 */	"stmt: JUMPV(reg)",
/* 118 */	"stmt: EQI4(reg,reg)",
/* 119 */	"stmt: EQU4(reg,reg)",
/* 120 */	"stmt: GEI4(reg,reg)",
/* 121 */	"stmt: GEU4(reg,reg)",
/* 122 */	"stmt: GTI4(reg,reg)",
/* 123 */	"stmt: GTU4(reg,reg)",
/* 124 */	"stmt: LEI4(reg,reg)",
/* 125 */	"stmt: LEU4(reg,reg)",
/* 126 */	"stmt: LTI4(reg,reg)",
/* 127 */	"stmt: LTU4(reg,reg)",
/* 128 */	"stmt: NEI4(reg,reg)",
/* 129 */	"stmt: NEU4(reg,reg)",
/* 130 */	"reg: CALLF4(lab)",
/* 131 */	"freg: CALLF4(lab)",
/* 132 */	"reg: CALLI4(lab)",
/* 133 */	"reg: CALLP4(lab)",
/* 134 */	"reg: CALLU4(lab)",
/* 135 */	"stmt: CALLV(lab)",
/* 136 */	"reg: CALLF4(reg)",
/* 137 */	"reg: CALLI4(reg)",
/* 138 */	"reg: CALLP4(reg)",
/* 139 */	"reg: CALLU4(reg)",
/* 140 */	"stmt: CALLV(reg)",
/* 141 */	"stmt: RETI4(reg)",
/* 142 */	"stmt: RETP4(reg)",
/* 143 */	"stmt: RETU4(reg)",
/* 144 */	"stmt: RETV(reg)",
/* 145 */	"stmt: RETF4(reg)",
/* 146 */	"stmt: RETF4(freg)",
/* 147 */	"stmt: ARGI4(reg)",
/* 148 */	"stmt: ARGP4(reg)",
/* 149 */	"stmt: ARGU4(reg)",
/* 150 */	"stmt: ARGF4(reg)",
/* 151 */	"stmt: ARGF4(freg)",
/* 152 */	"stmt: ARGB(INDIRB(reg))",
/* 153 */	"stmt: ASGNB(reg,INDIRB(reg))",
/* 154 */	"reg: INDIRF4(VREGP)",
/* 155 */	"stmt: ASGNF4(VREGP,reg)",
/* 156 */	"freg: INDIRF4(VREGP)",
/* 157 */	"stmt: ASGNF4(VREGP,freg)",
/* 158 */	"freg: SUBF4(freg,freg)",
/* 159 */	"freg: MULF4(freg,freg)",
/* 160 */	"freg: DIVF4(freg,freg)",
/* 161 */	"freg: ADDF4(freg,freg)",
/* 162 */	"freg: LOADF4(freg)",
/* 163 */	"freg: NEGF4(freg)",
/* 164 */	"freg: CVFF4(freg)",
/* 165 */	"freg: CVIF4(reg)",
/* 166 */	"reg: CVFI4(freg)",
/* 167 */	"stmt: EQF4(freg,freg)",
/* 168 */	"stmt: NEF4(freg,freg)",
/* 169 */	"stmt: LEF4(freg,freg)",
/* 170 */	"stmt: GTF4(freg,freg)",
/* 171 */	"stmt: LTF4(freg,freg)",
/* 172 */	"stmt: GEF4(freg,freg)",
/* 173 */	"reg: ADDF4(reg,reg)",
/* 174 */	"reg: SUBF4(reg,reg)",
/* 175 */	"reg: MULF4(reg,reg)",
/* 176 */	"reg: DIVF4(reg,reg)",
/* 177 */	"reg: LOADF4(reg)",
/* 178 */	"reg: NEGF4(reg)",
/* 179 */	"reg: CVFF4(reg)",
/* 180 */	"reg: CVIF4(reg)",
/* 181 */	"reg: CVFI4(reg)",
/* 182 */	"stmt: EQF4(reg,reg)",
/* 183 */	"stmt: LEF4(reg,reg)",
/* 184 */	"stmt: LTF4(reg,reg)",
/* 185 */	"stmt: GEF4(reg,reg)",
/* 186 */	"stmt: GTF4(reg,reg)",
/* 187 */	"stmt: NEF4(reg,reg)",
};

static short _decode_stmt[] = {
	0,
	8,
	9,
	10,
	11,
	12,
	13,
	14,
	22,
	38,
	39,
	40,
	41,
	42,
	43,
	44,
	45,
	46,
	115,
	116,
	117,
	118,
	119,
	120,
	121,
	122,
	123,
	124,
	125,
	126,
	127,
	128,
	129,
	135,
	140,
	141,
	142,
	143,
	144,
	145,
	146,
	147,
	148,
	149,
	150,
	151,
	152,
	153,
	155,
	157,
	167,
	168,
	169,
	170,
	171,
	172,
	182,
	183,
	184,
	185,
	186,
	187,
};

static short _decode_reg[] = {
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	24,
	25,
	26,
	27,
	28,
	29,
	30,
	31,
	32,
	36,
	37,
	47,
	48,
	49,
	50,
	51,
	52,
	53,
	54,
	56,
	57,
	58,
	59,
	60,
	61,
	62,
	63,
	64,
	65,
	66,
	67,
	69,
	70,
	71,
	72,
	73,
	74,
	75,
	76,
	77,
	78,
	79,
	80,
	81,
	82,
	83,
	84,
	85,
	86,
	87,
	88,
	89,
	90,
	91,
	92,
	93,
	94,
	95,
	97,
	98,
	99,
	100,
	101,
	102,
	103,
	104,
	105,
	106,
	107,
	108,
	109,
	110,
	111,
	112,
	113,
	114,
	130,
	132,
	133,
	134,
	136,
	137,
	138,
	139,
	154,
	166,
	173,
	174,
	175,
	176,
	177,
	178,
	179,
	180,
	181,
};

static short _decode_con[] = {
	0,
	15,
	16,
	17,
	18,
	19,
	20,
	21,
};

static short _decode_lab[] = {
	0,
	23,
};

static short _decode_addr[] = {
	0,
	33,
	34,
	35,
};

static short _decode_freg[] = {
	0,
	55,
	131,
	156,
	158,
	159,
	160,
	161,
	162,
	163,
	164,
	165,
};

static short _decode_cons12[] = {
	0,
	68,
};

static short _decode_con5[] = {
	0,
	96,
};

static int _rule(void *state, int goalnt) {
	if (goalnt < 1 || goalnt > 8)
		fatal("_rule", "Bad goal nonterminal %d\n", goalnt);
	if (!state)
		return 0;
	switch (goalnt) {
	case _stmt_NT:	return _decode_stmt[((struct _state *)state)->rule._stmt];
	case _reg_NT:	return _decode_reg[((struct _state *)state)->rule._reg];
	case _con_NT:	return _decode_con[((struct _state *)state)->rule._con];
	case _lab_NT:	return _decode_lab[((struct _state *)state)->rule._lab];
	case _addr_NT:	return _decode_addr[((struct _state *)state)->rule._addr];
	case _freg_NT:	return _decode_freg[((struct _state *)state)->rule._freg];
	case _cons12_NT:	return _decode_cons12[((struct _state *)state)->rule._cons12];
	case _con5_NT:	return _decode_con5[((struct _state *)state)->rule._con5];
	default:
		fatal("_rule", "Bad goal nonterminal %d\n", goalnt);
		return 0;
	}
}

static void _closure_reg(NODEPTR_TYPE, int);
static void _closure_con(NODEPTR_TYPE, int);
static void _closure_lab(NODEPTR_TYPE, int);

static void _closure_reg(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 0 < p->cost[_addr_NT]) {
		p->cost[_addr_NT] = c + 0;
		p->rule._addr = 1;
	}
	if (c + 0 < p->cost[_stmt_NT]) {
		p->cost[_stmt_NT] = c + 0;
		p->rule._stmt = 8;
	}
}

static void _closure_con(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 1 < p->cost[_reg_NT]) {
		p->cost[_reg_NT] = c + 1;
		p->rule._reg = 9;
		_closure_reg(a, c + 1);
	}
}

static void _closure_lab(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 1 < p->cost[_reg_NT]) {
		p->cost[_reg_NT] = c + 1;
		p->rule._reg = 8;
		_closure_reg(a, c + 1);
	}
}

static void _label(NODEPTR_TYPE a) {
	int c;
	struct _state *p;

	if (!a)
		fatal("_label", "Null tree\n", 0);
	STATE_LABEL(a) = p = allocate(sizeof *p, FUNC);
	p->rule._stmt = 0;
	p->cost[1] =
	p->cost[2] =
	p->cost[3] =
	p->cost[4] =
	p->cost[5] =
	p->cost[6] =
	p->cost[7] =
	p->cost[8] =
		0x7fff;
	switch (OP_LABEL(a)) {
	case 41: /* ARGB */
		_label(LEFT_CHILD(a));
		if (	/* stmt: ARGB(INDIRB(reg)) */
			LEFT_CHILD(a)->op == 73 /* INDIRB */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 46;
			}
		}
		break;
	case 57: /* ASGNB */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNB(reg,INDIRB(reg)) */
			RIGHT_CHILD(a)->op == 73 /* INDIRB */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 47;
			}
		}
		break;
	case 73: /* INDIRB */
		_label(LEFT_CHILD(a));
		break;
	case 216: /* CALLV */
		_label(LEFT_CHILD(a));
		/* stmt: CALLV(lab) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_lab_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 33;
		}
		/* stmt: CALLV(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 34;
		}
		break;
	case 217: /* CALLB */
		break;
	case 233: /* LOADB */
		break;
	case 248: /* RETV */
		_label(LEFT_CHILD(a));
		/* stmt: RETV(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 38;
		}
		break;
	case 584: /* JUMPV */
		_label(LEFT_CHILD(a));
		/* stmt: JUMPV(lab) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_lab_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 19;
		}
		/* stmt: JUMPV(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 20;
		}
		break;
	case 600: /* LABELV */
		/* stmt: LABELV */
		if (0 + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = 0 + 0;
			p->rule._stmt = 18;
		}
		break;
	case 711: /* VREGP */
		break;
	case 1045: /* CNSTI1 */
		/* con: CNSTI1 */
		if (0 + 0 < p->cost[_con_NT]) {
			p->cost[_con_NT] = 0 + 0;
			p->rule._con = 1;
			_closure_con(a, 0 + 0);
		}
		/* reg: CNSTI1 */
		c = (range(a, 0, 0));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 10;
			_closure_reg(a, c + 0);
		}
		break;
	case 1046: /* CNSTU1 */
		/* con: CNSTU1 */
		if (0 + 0 < p->cost[_con_NT]) {
			p->cost[_con_NT] = 0 + 0;
			p->rule._con = 5;
			_closure_con(a, 0 + 0);
		}
		/* reg: CNSTU1 */
		c = (range(a, 0, 0));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 13;
			_closure_reg(a, c + 0);
		}
		break;
	case 1077: /* ASGNI1 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNI1(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 1;
			}
		}
		/* stmt: ASGNI1(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 9;
		}
		break;
	case 1078: /* ASGNU1 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNU1(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 5;
			}
		}
		/* stmt: ASGNU1(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 13;
		}
		break;
	case 1093: /* INDIRI1 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRI1(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_lab_NT] == 0) {
					p->cost[_lab_NT] = 0;
					p->rule._lab = q->rule._lab;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_freg_NT] == 0) {
					p->cost[_freg_NT] = 0;
					p->rule._freg = q->rule._freg;
				}
				if (q->cost[_cons12_NT] == 0) {
					p->cost[_cons12_NT] = 0;
					p->rule._cons12 = q->rule._cons12;
				}
				if (q->cost[_con5_NT] == 0) {
					p->cost[_con5_NT] = 0;
					p->rule._con5 = q->rule._con5;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 1;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRI1(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 19;
			_closure_reg(a, c + 0);
		}
		break;
	case 1094: /* INDIRU1 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRU1(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_lab_NT] == 0) {
					p->cost[_lab_NT] = 0;
					p->rule._lab = q->rule._lab;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_freg_NT] == 0) {
					p->cost[_freg_NT] = 0;
					p->rule._freg = q->rule._freg;
				}
				if (q->cost[_cons12_NT] == 0) {
					p->cost[_cons12_NT] = 0;
					p->rule._cons12 = q->rule._cons12;
				}
				if (q->cost[_con5_NT] == 0) {
					p->cost[_con5_NT] = 0;
					p->rule._con5 = q->rule._con5;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 5;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRU1(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 23;
			_closure_reg(a, c + 0);
		}
		break;
	case 1157: /* CVII1 */
		break;
	case 1158: /* CVIU1 */
		break;
	case 1205: /* CVUI1 */
		break;
	case 1206: /* CVUU1 */
		break;
	case 1253: /* LOADI1 */
		_label(LEFT_CHILD(a));
		/* reg: LOADI1(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 74;
			_closure_reg(a, c + 0);
		}
		break;
	case 1254: /* LOADU1 */
		_label(LEFT_CHILD(a));
		/* reg: LOADU1(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 75;
			_closure_reg(a, c + 0);
		}
		break;
	case 2069: /* CNSTI2 */
		/* con: CNSTI2 */
		if (0 + 0 < p->cost[_con_NT]) {
			p->cost[_con_NT] = 0 + 0;
			p->rule._con = 2;
			_closure_con(a, 0 + 0);
		}
		/* reg: CNSTI2 */
		c = (range(a, 0, 0));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 11;
			_closure_reg(a, c + 0);
		}
		break;
	case 2070: /* CNSTU2 */
		/* con: CNSTU2 */
		if (0 + 0 < p->cost[_con_NT]) {
			p->cost[_con_NT] = 0 + 0;
			p->rule._con = 6;
			_closure_con(a, 0 + 0);
		}
		/* reg: CNSTU2 */
		c = (range(a, 0, 0));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 14;
			_closure_reg(a, c + 0);
		}
		break;
	case 2101: /* ASGNI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNI2(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 2;
			}
		}
		/* stmt: ASGNI2(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 10;
		}
		break;
	case 2102: /* ASGNU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNU2(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 6;
			}
		}
		/* stmt: ASGNU2(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 14;
		}
		break;
	case 2117: /* INDIRI2 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRI2(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_lab_NT] == 0) {
					p->cost[_lab_NT] = 0;
					p->rule._lab = q->rule._lab;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_freg_NT] == 0) {
					p->cost[_freg_NT] = 0;
					p->rule._freg = q->rule._freg;
				}
				if (q->cost[_cons12_NT] == 0) {
					p->cost[_cons12_NT] = 0;
					p->rule._cons12 = q->rule._cons12;
				}
				if (q->cost[_con5_NT] == 0) {
					p->cost[_con5_NT] = 0;
					p->rule._con5 = q->rule._con5;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 2;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRI2(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 20;
			_closure_reg(a, c + 0);
		}
		break;
	case 2118: /* INDIRU2 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRU2(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_lab_NT] == 0) {
					p->cost[_lab_NT] = 0;
					p->rule._lab = q->rule._lab;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_freg_NT] == 0) {
					p->cost[_freg_NT] = 0;
					p->rule._freg = q->rule._freg;
				}
				if (q->cost[_cons12_NT] == 0) {
					p->cost[_cons12_NT] = 0;
					p->rule._cons12 = q->rule._cons12;
				}
				if (q->cost[_con5_NT] == 0) {
					p->cost[_con5_NT] = 0;
					p->rule._con5 = q->rule._con5;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 6;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRU2(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 24;
			_closure_reg(a, c + 0);
		}
		break;
	case 2181: /* CVII2 */
		break;
	case 2182: /* CVIU2 */
		break;
	case 2229: /* CVUI2 */
		break;
	case 2230: /* CVUU2 */
		break;
	case 2277: /* LOADI2 */
		_label(LEFT_CHILD(a));
		/* reg: LOADI2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 76;
			_closure_reg(a, c + 0);
		}
		break;
	case 2278: /* LOADU2 */
		_label(LEFT_CHILD(a));
		/* reg: LOADU2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 77;
			_closure_reg(a, c + 0);
		}
		break;
	case 4113: /* CNSTF4 */
		break;
	case 4117: /* CNSTI4 */
		/* con: CNSTI4 */
		if (0 + 0 < p->cost[_con_NT]) {
			p->cost[_con_NT] = 0 + 0;
			p->rule._con = 3;
			_closure_con(a, 0 + 0);
		}
		/* reg: CNSTI4 */
		c = (range(a, 0, 0));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 12;
			_closure_reg(a, c + 0);
		}
		/* cons12: CNSTI4 */
		c = (range(a,-2048,2047));
		if (c + 0 < p->cost[_cons12_NT]) {
			p->cost[_cons12_NT] = c + 0;
			p->rule._cons12 = 1;
		}
		break;
	case 4118: /* CNSTU4 */
		/* con: CNSTU4 */
		if (0 + 0 < p->cost[_con_NT]) {
			p->cost[_con_NT] = 0 + 0;
			p->rule._con = 7;
			_closure_con(a, 0 + 0);
		}
		/* reg: CNSTU4 */
		c = (range(a, 0, 0));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 15;
			_closure_reg(a, c + 0);
		}
		/* con5: CNSTU4 */
		c = (range(a, 0, 31));
		if (c + 0 < p->cost[_con5_NT]) {
			p->cost[_con5_NT] = c + 0;
			p->rule._con5 = 1;
		}
		break;
	case 4119: /* CNSTP4 */
		/* con: CNSTP4 */
		if (0 + 0 < p->cost[_con_NT]) {
			p->cost[_con_NT] = 0 + 0;
			p->rule._con = 4;
			_closure_con(a, 0 + 0);
		}
		/* reg: CNSTP4 */
		c = (range(a, 0, 0));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 16;
			_closure_reg(a, c + 0);
		}
		break;
	case 4129: /* ARGF4 */
		_label(LEFT_CHILD(a));
		/* stmt: ARGF4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 44;
		}
		/* stmt: ARGF4(freg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_freg_NT] + (!hardfloat);
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 45;
		}
		break;
	case 4133: /* ARGI4 */
		_label(LEFT_CHILD(a));
		/* stmt: ARGI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 41;
		}
		break;
	case 4134: /* ARGU4 */
		_label(LEFT_CHILD(a));
		/* stmt: ARGU4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 43;
		}
		break;
	case 4135: /* ARGP4 */
		_label(LEFT_CHILD(a));
		/* stmt: ARGP4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 42;
		}
		break;
	case 4145: /* ASGNF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: ASGNF4(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 100;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 16;
		}
		/* stmt: ASGNF4(addr,freg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_freg_NT] + ((hardfloat?10:LBURG_MAX));
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 17;
		}
		if (	/* stmt: ASGNF4(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 48;
			}
		}
		if (	/* stmt: ASGNF4(VREGP,freg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_freg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 49;
			}
		}
		break;
	case 4149: /* ASGNI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNI4(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 3;
			}
		}
		/* stmt: ASGNI4(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 11;
		}
		break;
	case 4150: /* ASGNU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNU4(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 7;
			}
		}
		/* stmt: ASGNU4(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 15;
		}
		break;
	case 4151: /* ASGNP4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNP4(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 4;
			}
		}
		/* stmt: ASGNP4(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 12;
		}
		break;
	case 4161: /* INDIRF4 */
		_label(LEFT_CHILD(a));
		/* reg: INDIRF4(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 100;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 26;
			_closure_reg(a, c + 0);
		}
		/* freg: INDIRF4(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((hardfloat?10:LBURG_MAX));
		if (c + 0 < p->cost[_freg_NT]) {
			p->cost[_freg_NT] = c + 0;
			p->rule._freg = 1;
		}
		if (	/* reg: INDIRF4(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_lab_NT] == 0) {
					p->cost[_lab_NT] = 0;
					p->rule._lab = q->rule._lab;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_freg_NT] == 0) {
					p->cost[_freg_NT] = 0;
					p->rule._freg = q->rule._freg;
				}
				if (q->cost[_cons12_NT] == 0) {
					p->cost[_cons12_NT] = 0;
					p->rule._cons12 = q->rule._cons12;
				}
				if (q->cost[_con5_NT] == 0) {
					p->cost[_con5_NT] = 0;
					p->rule._con5 = q->rule._con5;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 92;
				_closure_reg(a, c + 0);
			}
		}
		if (	/* freg: INDIRF4(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_lab_NT] == 0) {
					p->cost[_lab_NT] = 0;
					p->rule._lab = q->rule._lab;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_freg_NT] == 0) {
					p->cost[_freg_NT] = 0;
					p->rule._freg = q->rule._freg;
				}
				if (q->cost[_cons12_NT] == 0) {
					p->cost[_cons12_NT] = 0;
					p->rule._cons12 = q->rule._cons12;
				}
				if (q->cost[_con5_NT] == 0) {
					p->cost[_con5_NT] = 0;
					p->rule._con5 = q->rule._con5;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_freg_NT]) {
				p->cost[_freg_NT] = c + 0;
				p->rule._freg = 3;
			}
		}
		break;
	case 4165: /* INDIRI4 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRI4(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_lab_NT] == 0) {
					p->cost[_lab_NT] = 0;
					p->rule._lab = q->rule._lab;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_freg_NT] == 0) {
					p->cost[_freg_NT] = 0;
					p->rule._freg = q->rule._freg;
				}
				if (q->cost[_cons12_NT] == 0) {
					p->cost[_cons12_NT] = 0;
					p->rule._cons12 = q->rule._cons12;
				}
				if (q->cost[_con5_NT] == 0) {
					p->cost[_con5_NT] = 0;
					p->rule._con5 = q->rule._con5;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 3;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRI4(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 21;
			_closure_reg(a, c + 0);
		}
		break;
	case 4166: /* INDIRU4 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRU4(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_lab_NT] == 0) {
					p->cost[_lab_NT] = 0;
					p->rule._lab = q->rule._lab;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_freg_NT] == 0) {
					p->cost[_freg_NT] = 0;
					p->rule._freg = q->rule._freg;
				}
				if (q->cost[_cons12_NT] == 0) {
					p->cost[_cons12_NT] = 0;
					p->rule._cons12 = q->rule._cons12;
				}
				if (q->cost[_con5_NT] == 0) {
					p->cost[_con5_NT] = 0;
					p->rule._con5 = q->rule._con5;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 7;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRU4(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 25;
			_closure_reg(a, c + 0);
		}
		break;
	case 4167: /* INDIRP4 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRP4(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_lab_NT] == 0) {
					p->cost[_lab_NT] = 0;
					p->rule._lab = q->rule._lab;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_freg_NT] == 0) {
					p->cost[_freg_NT] = 0;
					p->rule._freg = q->rule._freg;
				}
				if (q->cost[_cons12_NT] == 0) {
					p->cost[_cons12_NT] = 0;
					p->rule._cons12 = q->rule._cons12;
				}
				if (q->cost[_con5_NT] == 0) {
					p->cost[_con5_NT] = 0;
					p->rule._con5 = q->rule._con5;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 4;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRP4(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 22;
			_closure_reg(a, c + 0);
		}
		break;
	case 4209: /* CVFF4 */
		_label(LEFT_CHILD(a));
		/* freg: CVFF4(freg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_freg_NT] + ((hardfloat?10:LBURG_MAX));
		if (c + 0 < p->cost[_freg_NT]) {
			p->cost[_freg_NT] = c + 0;
			p->rule._freg = 10;
		}
		/* reg: CVFF4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 100;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 100;
			_closure_reg(a, c + 0);
		}
		break;
	case 4213: /* CVFI4 */
		_label(LEFT_CHILD(a));
		/* reg: CVFI4(freg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_freg_NT] + ((hardfloat?10:LBURG_MAX));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 93;
			_closure_reg(a, c + 0);
		}
		/* reg: CVFI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 100;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 102;
			_closure_reg(a, c + 0);
		}
		break;
	case 4225: /* CVIF4 */
		_label(LEFT_CHILD(a));
		/* freg: CVIF4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((hardfloat?10:LBURG_MAX));
		if (c + 0 < p->cost[_freg_NT]) {
			p->cost[_freg_NT] = c + 0;
			p->rule._freg = 11;
		}
		/* reg: CVIF4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 100;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 101;
			_closure_reg(a, c + 0);
		}
		break;
	case 4229: /* CVII4 */
		_label(LEFT_CHILD(a));
		if (	/* reg: CVII4(INDIRI1(addr)) */
			LEFT_CHILD(a)->op == 1093 /* INDIRI1 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_addr_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 27;
				_closure_reg(a, c + 0);
			}
		}
		if (	/* reg: CVII4(INDIRI2(addr)) */
			LEFT_CHILD(a)->op == 2117 /* INDIRI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_addr_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 28;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: CVII4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 81;
			_closure_reg(a, c + 0);
		}
		break;
	case 4230: /* CVIU4 */
		_label(LEFT_CHILD(a));
		break;
	case 4246: /* CVPU4 */
		break;
	case 4277: /* CVUI4 */
		_label(LEFT_CHILD(a));
		if (	/* reg: CVUI4(INDIRU1(addr)) */
			LEFT_CHILD(a)->op == 1094 /* INDIRU1 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_addr_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 31;
				_closure_reg(a, c + 0);
			}
		}
		if (	/* reg: CVUI4(INDIRU2(addr)) */
			LEFT_CHILD(a)->op == 2118 /* INDIRU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_addr_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 32;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: CVUI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 82;
			_closure_reg(a, c + 0);
		}
		break;
	case 4278: /* CVUU4 */
		_label(LEFT_CHILD(a));
		if (	/* reg: CVUU4(INDIRU1(addr)) */
			LEFT_CHILD(a)->op == 1094 /* INDIRU1 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_addr_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 29;
				_closure_reg(a, c + 0);
			}
		}
		if (	/* reg: CVUU4(INDIRU2(addr)) */
			LEFT_CHILD(a)->op == 2118 /* INDIRU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_addr_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 30;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: CVUU4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 83;
			_closure_reg(a, c + 0);
		}
		break;
	case 4279: /* CVUP4 */
		break;
	case 4289: /* NEGF4 */
		_label(LEFT_CHILD(a));
		/* freg: NEGF4(freg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_freg_NT] + ((hardfloat?10:LBURG_MAX));
		if (c + 0 < p->cost[_freg_NT]) {
			p->cost[_freg_NT] = c + 0;
			p->rule._freg = 9;
		}
		/* reg: NEGF4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 100;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 99;
			_closure_reg(a, c + 0);
		}
		break;
	case 4293: /* NEGI4 */
		_label(LEFT_CHILD(a));
		/* reg: NEGI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 63;
			_closure_reg(a, c + 0);
		}
		break;
	case 4305: /* CALLF4 */
		_label(LEFT_CHILD(a));
		/* reg: CALLF4(lab) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_lab_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 84;
			_closure_reg(a, c + 0);
		}
		/* freg: CALLF4(lab) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_lab_NT] + (!hardfloat);
		if (c + 0 < p->cost[_freg_NT]) {
			p->cost[_freg_NT] = c + 0;
			p->rule._freg = 2;
		}
		/* reg: CALLF4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 88;
			_closure_reg(a, c + 0);
		}
		break;
	case 4309: /* CALLI4 */
		_label(LEFT_CHILD(a));
		/* reg: CALLI4(lab) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_lab_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 85;
			_closure_reg(a, c + 0);
		}
		/* reg: CALLI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 89;
			_closure_reg(a, c + 0);
		}
		break;
	case 4310: /* CALLU4 */
		_label(LEFT_CHILD(a));
		/* reg: CALLU4(lab) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_lab_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 87;
			_closure_reg(a, c + 0);
		}
		/* reg: CALLU4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 91;
			_closure_reg(a, c + 0);
		}
		break;
	case 4311: /* CALLP4 */
		_label(LEFT_CHILD(a));
		/* reg: CALLP4(lab) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_lab_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 86;
			_closure_reg(a, c + 0);
		}
		/* reg: CALLP4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 90;
			_closure_reg(a, c + 0);
		}
		break;
	case 4321: /* LOADF4 */
		_label(LEFT_CHILD(a));
		/* freg: LOADF4(freg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_freg_NT] + (movehard(a));
		if (c + 0 < p->cost[_freg_NT]) {
			p->cost[_freg_NT] = c + 0;
			p->rule._freg = 8;
		}
		/* reg: LOADF4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move100(a));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 98;
			_closure_reg(a, c + 0);
		}
		break;
	case 4325: /* LOADI4 */
		_label(LEFT_CHILD(a));
		/* reg: LOADI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 78;
			_closure_reg(a, c + 0);
		}
		break;
	case 4326: /* LOADU4 */
		_label(LEFT_CHILD(a));
		/* reg: LOADU4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 80;
			_closure_reg(a, c + 0);
		}
		break;
	case 4327: /* LOADP4 */
		_label(LEFT_CHILD(a));
		/* reg: LOADP4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 79;
			_closure_reg(a, c + 0);
		}
		break;
	case 4337: /* RETF4 */
		_label(LEFT_CHILD(a));
		/* stmt: RETF4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 39;
		}
		/* stmt: RETF4(freg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_freg_NT] + (!hardfloat);
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 40;
		}
		break;
	case 4341: /* RETI4 */
		_label(LEFT_CHILD(a));
		/* stmt: RETI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 35;
		}
		break;
	case 4342: /* RETU4 */
		_label(LEFT_CHILD(a));
		/* stmt: RETU4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 37;
		}
		break;
	case 4343: /* RETP4 */
		_label(LEFT_CHILD(a));
		/* stmt: RETP4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 36;
		}
		break;
	case 4359: /* ADDRGP4 */
		/* lab: ADDRGP4 */
		if (0 + 0 < p->cost[_lab_NT]) {
			p->cost[_lab_NT] = 0 + 0;
			p->rule._lab = 1;
			_closure_lab(a, 0 + 0);
		}
		break;
	case 4375: /* ADDRFP4 */
		/* addr: ADDRFP4 */
		if (0 + 0 < p->cost[_addr_NT]) {
			p->cost[_addr_NT] = 0 + 0;
			p->rule._addr = 2;
		}
		/* reg: ADDRFP4 */
		if (1 + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = 1 + 0;
			p->rule._reg = 17;
			_closure_reg(a, 1 + 0);
		}
		break;
	case 4391: /* ADDRLP4 */
		/* addr: ADDRLP4 */
		if (0 + 0 < p->cost[_addr_NT]) {
			p->cost[_addr_NT] = 0 + 0;
			p->rule._addr = 3;
		}
		/* reg: ADDRLP4 */
		if (1 + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = 1 + 0;
			p->rule._reg = 18;
			_closure_reg(a, 1 + 0);
		}
		break;
	case 4401: /* ADDF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* freg: ADDF4(freg,freg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_freg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_freg_NT] + ((hardfloat?10:LBURG_MAX));
		if (c + 0 < p->cost[_freg_NT]) {
			p->cost[_freg_NT] = c + 0;
			p->rule._freg = 7;
		}
		/* reg: ADDF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 100;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 94;
			_closure_reg(a, c + 0);
		}
		break;
	case 4405: /* ADDI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: ADDI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 39;
			_closure_reg(a, c + 0);
		}
		/* reg: ADDI4(reg,cons12) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_cons12_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 40;
			_closure_reg(a, c + 0);
		}
		break;
	case 4406: /* ADDU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: ADDU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 43;
			_closure_reg(a, c + 0);
		}
		if (	/* reg: ADDU4(reg,CVIU4(cons12)) */
			RIGHT_CHILD(a)->op == 4230 /* CVIU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_cons12_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 44;
				_closure_reg(a, c + 0);
			}
		}
		break;
	case 4407: /* ADDP4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: ADDP4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 41;
			_closure_reg(a, c + 0);
		}
		if (	/* reg: ADDP4(reg,CVIU4(cons12)) */
			RIGHT_CHILD(a)->op == 4230 /* CVIU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_cons12_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 42;
				_closure_reg(a, c + 0);
			}
		}
		break;
	case 4417: /* SUBF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* freg: SUBF4(freg,freg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_freg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_freg_NT] + ((hardfloat?10:LBURG_MAX));
		if (c + 0 < p->cost[_freg_NT]) {
			p->cost[_freg_NT] = c + 0;
			p->rule._freg = 4;
		}
		/* reg: SUBF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 100;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 95;
			_closure_reg(a, c + 0);
		}
		break;
	case 4421: /* SUBI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: SUBI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 57;
			_closure_reg(a, c + 0);
		}
		/* reg: SUBI4(reg,cons12) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_cons12_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 58;
			_closure_reg(a, c + 0);
		}
		break;
	case 4422: /* SUBU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: SUBU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 61;
			_closure_reg(a, c + 0);
		}
		if (	/* reg: SUBU4(reg,CVIU4(cons12)) */
			RIGHT_CHILD(a)->op == 4230 /* CVIU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_cons12_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 62;
				_closure_reg(a, c + 0);
			}
		}
		break;
	case 4423: /* SUBP4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: SUBP4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 59;
			_closure_reg(a, c + 0);
		}
		/* reg: SUBP4(reg,cons12) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_cons12_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 60;
			_closure_reg(a, c + 0);
		}
		break;
	case 4437: /* LSHI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* reg: LSHI4(reg,CVUI4(con5)) */
			RIGHT_CHILD(a)->op == 4277 /* CVUI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_con5_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 66;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: LSHI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 67;
			_closure_reg(a, c + 0);
		}
		break;
	case 4438: /* LSHU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: LSHU4(reg,con5) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con5_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 68;
			_closure_reg(a, c + 0);
		}
		/* reg: LSHU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 69;
			_closure_reg(a, c + 0);
		}
		break;
	case 4453: /* MODI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: MODI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 35;
			_closure_reg(a, c + 0);
		}
		break;
	case 4454: /* MODU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: MODU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 36;
			_closure_reg(a, c + 0);
		}
		break;
	case 4469: /* RSHI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* reg: RSHI4(reg,CVUI4(con5)) */
			RIGHT_CHILD(a)->op == 4277 /* CVUI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_con5_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 70;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: RSHI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 71;
			_closure_reg(a, c + 0);
		}
		break;
	case 4470: /* RSHU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: RSHU4(reg,con5) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con5_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 72;
			_closure_reg(a, c + 0);
		}
		/* reg: RSHU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 73;
			_closure_reg(a, c + 0);
		}
		break;
	case 4485: /* BANDI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BANDI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 45;
			_closure_reg(a, c + 0);
		}
		/* reg: BANDI4(reg,cons12) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_cons12_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 46;
			_closure_reg(a, c + 0);
		}
		break;
	case 4486: /* BANDU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BANDU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 51;
			_closure_reg(a, c + 0);
		}
		if (	/* reg: BANDU4(reg,CVIU4(cons12)) */
			RIGHT_CHILD(a)->op == 4230 /* CVIU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_cons12_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 52;
				_closure_reg(a, c + 0);
			}
		}
		break;
	case 4501: /* BCOMI4 */
		_label(LEFT_CHILD(a));
		/* reg: BCOMI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 64;
			_closure_reg(a, c + 0);
		}
		break;
	case 4502: /* BCOMU4 */
		_label(LEFT_CHILD(a));
		/* reg: BCOMU4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 65;
			_closure_reg(a, c + 0);
		}
		break;
	case 4517: /* BORI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BORI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 47;
			_closure_reg(a, c + 0);
		}
		/* reg: BORI4(reg,cons12) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_cons12_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 48;
			_closure_reg(a, c + 0);
		}
		break;
	case 4518: /* BORU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BORU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 53;
			_closure_reg(a, c + 0);
		}
		if (	/* reg: BORU4(reg,CVIU4(cons12)) */
			RIGHT_CHILD(a)->op == 4230 /* CVIU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_cons12_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 54;
				_closure_reg(a, c + 0);
			}
		}
		break;
	case 4533: /* BXORI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BXORI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 49;
			_closure_reg(a, c + 0);
		}
		/* reg: BXORI4(reg,cons12) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_cons12_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 50;
			_closure_reg(a, c + 0);
		}
		break;
	case 4534: /* BXORU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BXORU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 55;
			_closure_reg(a, c + 0);
		}
		if (	/* reg: BXORU4(reg,CVIU4(cons12)) */
			RIGHT_CHILD(a)->op == 4230 /* CVIU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_cons12_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 56;
				_closure_reg(a, c + 0);
			}
		}
		break;
	case 4545: /* DIVF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* freg: DIVF4(freg,freg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_freg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_freg_NT] + ((hardfloat?10:LBURG_MAX));
		if (c + 0 < p->cost[_freg_NT]) {
			p->cost[_freg_NT] = c + 0;
			p->rule._freg = 6;
		}
		/* reg: DIVF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 100;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 97;
			_closure_reg(a, c + 0);
		}
		break;
	case 4549: /* DIVI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: DIVI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 33;
			_closure_reg(a, c + 0);
		}
		break;
	case 4550: /* DIVU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: DIVU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 34;
			_closure_reg(a, c + 0);
		}
		break;
	case 4561: /* MULF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* freg: MULF4(freg,freg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_freg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_freg_NT] + ((hardfloat?10:LBURG_MAX));
		if (c + 0 < p->cost[_freg_NT]) {
			p->cost[_freg_NT] = c + 0;
			p->rule._freg = 5;
		}
		/* reg: MULF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 100;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 96;
			_closure_reg(a, c + 0);
		}
		break;
	case 4565: /* MULI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: MULI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 37;
			_closure_reg(a, c + 0);
		}
		break;
	case 4566: /* MULU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: MULU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 38;
			_closure_reg(a, c + 0);
		}
		break;
	case 4577: /* EQF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: EQF4(freg,freg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_freg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_freg_NT] + ((hardfloat?10:LBURG_MAX));
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 50;
		}
		/* stmt: EQF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 100;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 56;
		}
		break;
	case 4581: /* EQI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: EQI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 21;
		}
		break;
	case 4582: /* EQU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: EQU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 22;
		}
		break;
	case 4593: /* GEF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GEF4(freg,freg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_freg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_freg_NT] + ((hardfloat?10:LBURG_MAX));
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 55;
		}
		/* stmt: GEF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 100;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 59;
		}
		break;
	case 4597: /* GEI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GEI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 23;
		}
		break;
	case 4598: /* GEU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GEU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 24;
		}
		break;
	case 4609: /* GTF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GTF4(freg,freg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_freg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_freg_NT] + ((hardfloat?10:LBURG_MAX));
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 53;
		}
		/* stmt: GTF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 100;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 60;
		}
		break;
	case 4613: /* GTI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GTI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 25;
		}
		break;
	case 4614: /* GTU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GTU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 26;
		}
		break;
	case 4625: /* LEF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LEF4(freg,freg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_freg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_freg_NT] + ((hardfloat?10:LBURG_MAX));
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 52;
		}
		/* stmt: LEF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 100;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 57;
		}
		break;
	case 4629: /* LEI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LEI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 27;
		}
		break;
	case 4630: /* LEU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LEU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 28;
		}
		break;
	case 4641: /* LTF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LTF4(freg,freg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_freg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_freg_NT] + ((hardfloat?10:LBURG_MAX));
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 54;
		}
		/* stmt: LTF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 100;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 58;
		}
		break;
	case 4645: /* LTI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LTI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 29;
		}
		break;
	case 4646: /* LTU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LTU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 30;
		}
		break;
	case 4657: /* NEF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: NEF4(freg,freg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_freg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_freg_NT] + ((hardfloat?10:LBURG_MAX));
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 51;
		}
		/* stmt: NEF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 100;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 61;
		}
		break;
	case 4661: /* NEI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: NEI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 31;
		}
		break;
	case 4662: /* NEU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: NEU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 32;
		}
		break;
	case 8417: /* LOADF8 */
		break;
	case 8421: /* LOADI8 */
		break;
	case 8422: /* LOADU8 */
		break;
	case 8423: /* LOADP8 */
		break;
	case 16609: /* LOADF16 */
		break;
	default:
		fatal("_label", "Bad terminal %d\n", OP_LABEL(a));
	}
}

static void _kids(NODEPTR_TYPE p, int eruleno, NODEPTR_TYPE kids[]) {
	if (!p)
		fatal("_kids", "Null tree\n", 0);
	if (!kids)
		fatal("_kids", "Null kids\n", 0);
	switch (eruleno) {
	case 156: /* freg: INDIRF4(VREGP) */
	case 154: /* reg: INDIRF4(VREGP) */
	case 115: /* stmt: LABELV */
	case 96: /* con5: CNSTU4 */
	case 68: /* cons12: CNSTI4 */
	case 37: /* reg: ADDRLP4 */
	case 36: /* reg: ADDRFP4 */
	case 35: /* addr: ADDRLP4 */
	case 34: /* addr: ADDRFP4 */
	case 32: /* reg: CNSTP4 */
	case 31: /* reg: CNSTU4 */
	case 30: /* reg: CNSTU2 */
	case 29: /* reg: CNSTU1 */
	case 28: /* reg: CNSTI4 */
	case 27: /* reg: CNSTI2 */
	case 26: /* reg: CNSTI1 */
	case 23: /* lab: ADDRGP4 */
	case 21: /* con: CNSTU4 */
	case 20: /* con: CNSTU2 */
	case 19: /* con: CNSTU1 */
	case 18: /* con: CNSTP4 */
	case 17: /* con: CNSTI4 */
	case 16: /* con: CNSTI2 */
	case 15: /* con: CNSTI1 */
	case 7: /* reg: INDIRU4(VREGP) */
	case 6: /* reg: INDIRU2(VREGP) */
	case 5: /* reg: INDIRU1(VREGP) */
	case 4: /* reg: INDIRP4(VREGP) */
	case 3: /* reg: INDIRI4(VREGP) */
	case 2: /* reg: INDIRI2(VREGP) */
	case 1: /* reg: INDIRI1(VREGP) */
		break;
	case 157: /* stmt: ASGNF4(VREGP,freg) */
	case 155: /* stmt: ASGNF4(VREGP,reg) */
	case 14: /* stmt: ASGNU4(VREGP,reg) */
	case 13: /* stmt: ASGNU2(VREGP,reg) */
	case 12: /* stmt: ASGNU1(VREGP,reg) */
	case 11: /* stmt: ASGNP4(VREGP,reg) */
	case 10: /* stmt: ASGNI4(VREGP,reg) */
	case 9: /* stmt: ASGNI2(VREGP,reg) */
	case 8: /* stmt: ASGNI1(VREGP,reg) */
		kids[0] = RIGHT_CHILD(p);
		break;
	case 33: /* addr: reg */
	case 25: /* reg: con */
	case 24: /* reg: lab */
	case 22: /* stmt: reg */
		kids[0] = p;
		break;
	case 187: /* stmt: NEF4(reg,reg) */
	case 186: /* stmt: GTF4(reg,reg) */
	case 185: /* stmt: GEF4(reg,reg) */
	case 184: /* stmt: LTF4(reg,reg) */
	case 183: /* stmt: LEF4(reg,reg) */
	case 182: /* stmt: EQF4(reg,reg) */
	case 176: /* reg: DIVF4(reg,reg) */
	case 175: /* reg: MULF4(reg,reg) */
	case 174: /* reg: SUBF4(reg,reg) */
	case 173: /* reg: ADDF4(reg,reg) */
	case 172: /* stmt: GEF4(freg,freg) */
	case 171: /* stmt: LTF4(freg,freg) */
	case 170: /* stmt: GTF4(freg,freg) */
	case 169: /* stmt: LEF4(freg,freg) */
	case 168: /* stmt: NEF4(freg,freg) */
	case 167: /* stmt: EQF4(freg,freg) */
	case 161: /* freg: ADDF4(freg,freg) */
	case 160: /* freg: DIVF4(freg,freg) */
	case 159: /* freg: MULF4(freg,freg) */
	case 158: /* freg: SUBF4(freg,freg) */
	case 129: /* stmt: NEU4(reg,reg) */
	case 128: /* stmt: NEI4(reg,reg) */
	case 127: /* stmt: LTU4(reg,reg) */
	case 126: /* stmt: LTI4(reg,reg) */
	case 125: /* stmt: LEU4(reg,reg) */
	case 124: /* stmt: LEI4(reg,reg) */
	case 123: /* stmt: GTU4(reg,reg) */
	case 122: /* stmt: GTI4(reg,reg) */
	case 121: /* stmt: GEU4(reg,reg) */
	case 120: /* stmt: GEI4(reg,reg) */
	case 119: /* stmt: EQU4(reg,reg) */
	case 118: /* stmt: EQI4(reg,reg) */
	case 104: /* reg: RSHU4(reg,reg) */
	case 103: /* reg: RSHU4(reg,con5) */
	case 102: /* reg: RSHI4(reg,reg) */
	case 100: /* reg: LSHU4(reg,reg) */
	case 99: /* reg: LSHU4(reg,con5) */
	case 98: /* reg: LSHI4(reg,reg) */
	case 91: /* reg: SUBU4(reg,reg) */
	case 90: /* reg: SUBP4(reg,cons12) */
	case 89: /* reg: SUBP4(reg,reg) */
	case 88: /* reg: SUBI4(reg,cons12) */
	case 87: /* reg: SUBI4(reg,reg) */
	case 85: /* reg: BXORU4(reg,reg) */
	case 83: /* reg: BORU4(reg,reg) */
	case 81: /* reg: BANDU4(reg,reg) */
	case 80: /* reg: BXORI4(reg,cons12) */
	case 79: /* reg: BXORI4(reg,reg) */
	case 78: /* reg: BORI4(reg,cons12) */
	case 77: /* reg: BORI4(reg,reg) */
	case 76: /* reg: BANDI4(reg,cons12) */
	case 75: /* reg: BANDI4(reg,reg) */
	case 73: /* reg: ADDU4(reg,reg) */
	case 71: /* reg: ADDP4(reg,reg) */
	case 70: /* reg: ADDI4(reg,cons12) */
	case 69: /* reg: ADDI4(reg,reg) */
	case 67: /* reg: MULU4(reg,reg) */
	case 66: /* reg: MULI4(reg,reg) */
	case 65: /* reg: MODU4(reg,reg) */
	case 64: /* reg: MODI4(reg,reg) */
	case 63: /* reg: DIVU4(reg,reg) */
	case 62: /* reg: DIVI4(reg,reg) */
	case 46: /* stmt: ASGNF4(addr,freg) */
	case 45: /* stmt: ASGNF4(addr,reg) */
	case 44: /* stmt: ASGNU4(addr,reg) */
	case 43: /* stmt: ASGNU2(addr,reg) */
	case 42: /* stmt: ASGNU1(addr,reg) */
	case 41: /* stmt: ASGNP4(addr,reg) */
	case 40: /* stmt: ASGNI4(addr,reg) */
	case 39: /* stmt: ASGNI2(addr,reg) */
	case 38: /* stmt: ASGNI1(addr,reg) */
		kids[0] = LEFT_CHILD(p);
		kids[1] = RIGHT_CHILD(p);
		break;
	case 181: /* reg: CVFI4(reg) */
	case 180: /* reg: CVIF4(reg) */
	case 179: /* reg: CVFF4(reg) */
	case 178: /* reg: NEGF4(reg) */
	case 177: /* reg: LOADF4(reg) */
	case 166: /* reg: CVFI4(freg) */
	case 165: /* freg: CVIF4(reg) */
	case 164: /* freg: CVFF4(freg) */
	case 163: /* freg: NEGF4(freg) */
	case 162: /* freg: LOADF4(freg) */
	case 151: /* stmt: ARGF4(freg) */
	case 150: /* stmt: ARGF4(reg) */
	case 149: /* stmt: ARGU4(reg) */
	case 148: /* stmt: ARGP4(reg) */
	case 147: /* stmt: ARGI4(reg) */
	case 146: /* stmt: RETF4(freg) */
	case 145: /* stmt: RETF4(reg) */
	case 144: /* stmt: RETV(reg) */
	case 143: /* stmt: RETU4(reg) */
	case 142: /* stmt: RETP4(reg) */
	case 141: /* stmt: RETI4(reg) */
	case 140: /* stmt: CALLV(reg) */
	case 139: /* reg: CALLU4(reg) */
	case 138: /* reg: CALLP4(reg) */
	case 137: /* reg: CALLI4(reg) */
	case 136: /* reg: CALLF4(reg) */
	case 135: /* stmt: CALLV(lab) */
	case 134: /* reg: CALLU4(lab) */
	case 133: /* reg: CALLP4(lab) */
	case 132: /* reg: CALLI4(lab) */
	case 131: /* freg: CALLF4(lab) */
	case 130: /* reg: CALLF4(lab) */
	case 117: /* stmt: JUMPV(reg) */
	case 116: /* stmt: JUMPV(lab) */
	case 114: /* reg: CVUU4(reg) */
	case 113: /* reg: CVUI4(reg) */
	case 112: /* reg: CVII4(reg) */
	case 111: /* reg: LOADU4(reg) */
	case 110: /* reg: LOADP4(reg) */
	case 109: /* reg: LOADI4(reg) */
	case 108: /* reg: LOADU2(reg) */
	case 107: /* reg: LOADI2(reg) */
	case 106: /* reg: LOADU1(reg) */
	case 105: /* reg: LOADI1(reg) */
	case 95: /* reg: BCOMU4(reg) */
	case 94: /* reg: BCOMI4(reg) */
	case 93: /* reg: NEGI4(reg) */
	case 55: /* freg: INDIRF4(addr) */
	case 54: /* reg: INDIRF4(addr) */
	case 53: /* reg: INDIRU4(addr) */
	case 52: /* reg: INDIRU2(addr) */
	case 51: /* reg: INDIRU1(addr) */
	case 50: /* reg: INDIRP4(addr) */
	case 49: /* reg: INDIRI4(addr) */
	case 48: /* reg: INDIRI2(addr) */
	case 47: /* reg: INDIRI1(addr) */
		kids[0] = LEFT_CHILD(p);
		break;
	case 152: /* stmt: ARGB(INDIRB(reg)) */
	case 61: /* reg: CVUI4(INDIRU2(addr)) */
	case 60: /* reg: CVUI4(INDIRU1(addr)) */
	case 59: /* reg: CVUU4(INDIRU2(addr)) */
	case 58: /* reg: CVUU4(INDIRU1(addr)) */
	case 57: /* reg: CVII4(INDIRI2(addr)) */
	case 56: /* reg: CVII4(INDIRI1(addr)) */
		kids[0] = LEFT_CHILD(LEFT_CHILD(p));
		break;
	case 153: /* stmt: ASGNB(reg,INDIRB(reg)) */
	case 101: /* reg: RSHI4(reg,CVUI4(con5)) */
	case 97: /* reg: LSHI4(reg,CVUI4(con5)) */
	case 92: /* reg: SUBU4(reg,CVIU4(cons12)) */
	case 86: /* reg: BXORU4(reg,CVIU4(cons12)) */
	case 84: /* reg: BORU4(reg,CVIU4(cons12)) */
	case 82: /* reg: BANDU4(reg,CVIU4(cons12)) */
	case 74: /* reg: ADDU4(reg,CVIU4(cons12)) */
	case 72: /* reg: ADDP4(reg,CVIU4(cons12)) */
		kids[0] = LEFT_CHILD(p);
		kids[1] = LEFT_CHILD(RIGHT_CHILD(p));
		break;
	default:
		fatal("_kids", "Bad rule number %d\n", eruleno);
	}
}


static int move100(Node p) {
	p->x.copy = 1;
	return 100;
}

static int movehard(Node p) {
	p->x.copy = 1;
    if(hardfloat) return 10;
     else return LBURG_MAX;
}

static void address(Symbol s1, Symbol s2, long n) {
  if (s2->scope == GLOBAL ||
      s2->sclass == STATIC ||
      s2->sclass == EXTERN) {
    s1->x.name = stringf("%s%s%D", s2->x.name, n >= 0 ? "+" : "", n);
  } else {
    assert(n >= INT_MIN && n <= INT_MAX);
    s1->x.offset = s2->x.offset + n;
    s1->x.name = stringd(s1->x.offset);
  }
}


static void defaddress(Symbol s) {
  print("\t.word\t%s\n", s->x.name);
}


static void defconst(int suffix, int size, Value v) {
  float f;
  double d;
  unsigned *p;

  if (suffix == F && size == 4) {
    f = v.d;
    print("\t.word\t0x%x\n", * (unsigned *) &f);
  } else
  if (suffix == F && size == 8) {
    d = v.d;
    p = (unsigned *) &d;
    print("\t.word\t0x%x\n", p[swap]);
    print("\t.word\t0x%x\n", p[1 - swap]);
  } else
  if (suffix == P) {
    print("\t.word\t0x%X\n", (unsigned long) v.p);
  } else
  if (size == 1) {
    print("\t.byte\t0x%x\n",
          (unsigned) ((unsigned char) (suffix == I ? v.i : v.u)));
  } else
  if (size == 2) {
    print("\t.half\t0x%x\n",
          (unsigned) ((unsigned short) (suffix == I ? v.i : v.u)));
  } else
  if (size == 4) {
    print("\t.word\t0x%x\n", (unsigned) (suffix == I ? v.i : v.u));
  }
}


static void defstring(int n, char *str) {
  char *s;

  for (s = str; s < str + n; s++) {
    print("\t.byte\t0x%x\n", (*s) & 0xFF);
  }
}


static void defsymbol(Symbol s) {
  if (s->scope >= LOCAL && s->sclass == STATIC) {
    s->x.name = stringf("L.%d", genlabel(1));
  } else
  if (s->generated) {
    s->x.name = stringf("L.%s", s->name);
  } else {
    assert(s->scope != CONSTANTS || isint(s->type) || isptr(s->type));
    s->x.name = s->name;
  }
}


static void export(Symbol s) {
  print("\t.globl %s\n", s->name);
}


static int bitcount(unsigned mask) {
  unsigned i, n;

  n = 0;
  for (i = 1; i != 0; i <<= 1) {
    if (mask & i) {
      n++;
    }
  }
  return n;
}


static Symbol argreg(int argno, int offset, int ty, int sz, int ty0) {
  assert((offset & 3) == 0);
  if (offset > 20) {
    return NULL;
  }
  if(hardfloat && ty==F)
    return freg[(offset / 4) + 12];
  else
   return ireg[(offset / 4) + 12];
}


static void function(Symbol f, Symbol caller[], Symbol callee[], int ncalls) {
  int i, varargs;
  Symbol p, q;
  Symbol r;
  int sizeisave;
  int sizefsave;
  int saved;
  int framesizeabs;
  Symbol argregs[6];

  usedmask[0] = usedmask[1] = 0;
  freemask[0] = freemask[1] = ~((unsigned) 0);
  offset = 0;
  maxoffset = 0;
  maxargoffset = 0;
  
  for (i = 0; callee[i]; i++); 
  varargs = variadic(f->type)
                || i > 0 && strcmp(callee[i-1]->name, "va_alist") == 0; 
  assert(caller[i] == NULL);  
  
  for (i = 0; callee[i] != NULL; i++) {
    p = callee[i];
    q = caller[i];
    assert(q != NULL);
    offset = roundup(offset, q->type->align);
    p->x.offset = q->x.offset = offset;
    p->x.name = q->x.name = stringd(offset);
    r = argreg(i, offset, optype(ttob(q->type)),
               q->type->size, optype(ttob(caller[0]->type)));
    if (i < 6) {
      argregs[i] = r;
    }
    offset = roundup(offset + q->type->size, 4);
    if (varargs) {      
      p->sclass = AUTO;       
    } else
    if (r != NULL && ncalls == 0 && !isstruct(q->type) &&
        !p->addressed && !(isfloat(q->type) && r->x.regnode->set == IREG)) {
      p->sclass = q->sclass = REGISTER;
      askregvar(p, r);
      assert(p->x.regnode && p->x.regnode->vbl == p);
      q->x = p->x;
      q->type = p->type;       
    } else {    
    if (askregvar(p, rmap(ttob(p->type))) &&
        r != NULL && (isint(p->type) || p->type == q->type)) {
      assert(q->sclass != REGISTER);
      p->sclass = q->sclass = REGISTER;
      q->type = p->type;      
    }
    }
  }
  
  offset = 4;
  gencode(caller, callee);
  if (ncalls != 0) {
    usedmask[IREG] |= ((unsigned) 1) << 1;
  }
  usedmask[IREG] &= 0x0FFC0002;
  usedmask[FREG] &= 0x0FFC0000;
  maxargoffset = roundup(maxargoffset, 4);
  if (ncalls != 0 && maxargoffset < 24) {
    maxargoffset = 24;
  }
  sizeisave = 4 * bitcount(usedmask[IREG]);
  sizefsave = 4 * bitcount(usedmask[FREG]);
  framesize = roundup(maxoffset, 16);
  framesizeabs = roundup(maxargoffset + sizeisave + sizefsave + 4, 16) + framesize;
  segment(CODE);
  print("\t.align\t4\n");
  print("%s:\n", f->x.name);
  if (framesizeabs > 0) {
    print("\taddi x2,x2,-%d\n", framesizeabs);
  }
  print("\tsw  x8,%d(x2)\n", framesizeabs-4);
  print("\taddi  x8,x2,%d\n",framesizeabs-framesize);
  
  saved = maxargoffset;
  for (i = 0; i < 32; i++) {
    if (usedmask[FREG] & (1 << i)) {
      print("\tfsw f%d,%d(x2)\n", i, saved);
      saved += 4;
    }
  }
  for (i = 0; i < 32; i++) {
    if (usedmask[IREG] & (1 << i)) {
      print("\tsw x%d,%d(x2)\n", i, saved);
      saved += 4;
    }
  }
  for (i = 0; i < 6 && callee[i] != NULL; i++) {
    r = argregs[i];
    if (r && r->x.regnode != callee[i]->x.regnode) {      
      Symbol out = callee[i];
      Symbol in = caller[i];
      int rn = r->x.regnode->number;
      int rs = r->x.regnode->set;
      int tyin = ttob(in->type);
      assert(out && in && r && r->x.regnode);
      assert(out->sclass != REGISTER || out->x.regnode);
      if (out->sclass == REGISTER &&
          (isint(out->type) || out->type == in->type)) {
        int outn = out->x.regnode->number;
        if(rs == FREG)        
          print("\tfsgnj.s f%d,f%d,f%d\n", outn, rn, rn);           
        else
          print("\tmv x%d,x%d\n", outn, rn);        
      } else {
        int off = in->x.offset;
        int n = (in->type->size + 3) / 4;
        int i;
        for (i = rn; i < rn + n; i++) {
            if(rs == FREG)
                print("\tfsw f%d,%d(x8)\n", i, framesize + off + (i - rn) * 4);
            else
                print("\tsw x%d,%d(x8)\n", i, framesize + off + (i - rn) * 4);
        }
      }
    }
  }
  if (varargs && callee[i - 1] != NULL) {
    i = callee[i - 1]->x.offset + callee[i - 1]->type->size;
	for (i = roundup(i, 4)/4; i < 6; i++) {
      print("\tsw x%d,%d(x8)\n", i + 12, framesize + 4 * i);
    }
  }
  emitcode();
  saved = maxargoffset;
  for (i = 0; i < 32; i++) {
    if (usedmask[FREG] & (1 << i)) {
      print("\tflw f%d,%d(x2)\n", i, saved);
      saved += 4;
    }
  }
  for (i = 0; i < 32; i++) {
    if (usedmask[IREG] & (1 << i)) {
      print("\tlw x%d,%d(x2)\n", i, saved);
      saved += 4;
    }
  }  
  print("\tlw  x8,%d(x2)\n",framesizeabs-4);  
  print("\taddi  x2,x2,%d\n",framesizeabs);
  print("\tjalr x0,x1,0\n");
  print("\n");
}


static void global(Symbol s) {
  if (s->type->align != 0) {
    print("\t.align\t%d\n", s->type->align);
  } else {
    print("\t.align\t%d\n", 4);
  }
  print("%s:\n", s->x.name);
}


static void import(Symbol s) {
  //print("\t.import %s\n", s->name);
}


static void local(Symbol s) {
  if (askregvar(s, rmap(ttob(s->type))) == 0) {
    mkauto(s);
  }
}


static void setSwap(void) {
  union {
    char c;
    int i;
  } u;

  u.i = 0;
  u.c = 1;
  swap = ((u.i == 1) != IR->little_endian);
}


static void progbeg(int argc, char *argv[]) {
  int i;

  setSwap();
  segment(CODE);
  parseflags(argc, argv);
  for (i = 0; i < argc; i++)
		if (strcmp(argv[i], "-hf") == 0)
			hardfloat = 1;
		else  if (strcmp(argv[i], "-d") == 0)
			dflag = 1;		
  for (i = 0; i < 32; i++) {
    ireg[i] = mkreg("%d", i, 1, IREG);
  }
  iregw = mkwildcard(ireg);
  for (i = 0; i < 32; i++) {
    freg[i] = mkreg("%d", i, 1, FREG);
  }
  ireg[2]->x.name = "x2";
  fregw = mkwildcard(freg);
  tmask[IREG] = INTTMP;
  vmask[IREG] = INTVAR;
  tmask[FREG] = FLTTMP;
  vmask[FREG] = FLTVAR;
  blkreg = mkreg("5", 5, 7, IREG);
}


static void progend(void) {
 print("\t.align 4\n");
}


static void segment(int n) {
  static int currSeg = -1;
  int newSeg;
 
  if(n == DBGBEG) {
    print("\t.section .debug_info\n");
    return;
  }
  if(n != DBGEND){
  
    switch (n) {
      case CODE:
        newSeg = CODE;
        break;
      case BSS:
        newSeg = BSS;
        break;
      case DATA:
        newSeg = DATA;
        break;
      case LIT:
        newSeg = DATA;
        break;
    }
    if (currSeg == newSeg) {
      return;
    }
  } else {
   if(currSeg>0) newSeg = currSeg;
  }
  print("\t.align 4\n");
  switch (newSeg) {
    case CODE:
      print("\t.text\n");
      break;
    case BSS:
      print("\t.bss\n");
      break;
    case DATA:
      print("\t.data\n");
      break;
  }
  currSeg = newSeg;
}


static void space(int n) {
  print("\t.space\t%d\n", n);
}


static Symbol rmap(int opk) {
  switch (optype(opk)) {
    case I:
    case U:
    case P:
    case B: 
     return iregw;
    case F:
      if (hardfloat)
      return fregw;
      else
      return iregw;      
    default:
      return 0;
  }
}


static void blkfetch(int size, int off, int reg, int tmp) {
  assert(size == 1 || size == 2 || size == 4);
  if (size == 1) {
    print("\tlb x%d,%d(x%d)\n", tmp, off, reg);
  } else
  if (size == 2) {
    print("\tlh x%d,%d(x%d)\n", tmp, off, reg);
  } else
  if (size == 4) {
    print("\tlw x%d,%d(x%d)\n", tmp, off, reg);
  }
}


static void blkstore(int size, int off, int reg, int tmp) {
  assert(size == 1 || size == 2 || size == 4);
  if (size == 1) {
    print("\tsb\tx%d, %d(x%d)\n", tmp, off, reg);
  } else
  if (size == 2) {
    print("\tsh\tx%d, %d(x%d)\n", tmp, off, reg);
  } else
  if (size == 4) {
    print("\tsw\tx%d, %d(x%d)\n", tmp, off, reg);
  }
}


static void blkloop(int dreg, int doff,
                    int sreg, int soff,
                    int size, int tmps[]) {
  int label;

  label = genlabel(1);
  print("\taddi\tx%d,x%d,%d\n", sreg, sreg, size & ~7);
  print("\taddi\tx%d,x%d,%d\n", tmps[2], dreg, size & ~7);
  blkcopy(tmps[2], doff, sreg, soff, size & 7, tmps);
  print("L.%d:\n", label);
  print("\taddi\tx%d,x%d,-%d\n", sreg, sreg, 8);
  print("\taddi\tx%d,x%d,-%d\n", tmps[2], tmps[2], 8);
  blkcopy(tmps[2], doff, sreg, soff, 8, tmps);
  print("\tbltu\tx%d,x%d,L.%d\n", dreg, tmps[2], label);
}


static void emit2(Node p) {
  static int ty0;
  int ty, sz;
  Symbol q;
  int src;
  int dst, n;

  switch (specific(p->op)) {
    case ARG+F:
    case ARG+I:
    case ARG+P:
    case ARG+U:
      ty = optype(p->op);
      sz = opsize(p->op);
      if (p->x.argno == 0) {
        ty0 = ty;
      }
      q = argreg(p->x.argno, p->syms[2]->u.c.v.i, ty, sz, ty0);
      src = getregnum(p->x.kids[0]);
      if (q == NULL) {
        if(ty==F && hardfloat)
            print("\tfsw f%d,%d(x2)\n", src, p->syms[2]->u.c.v.i);           
        else
            print("\tsw x%d,%d(x2)\n", src, p->syms[2]->u.c.v.i);
      } else {
        if(ty==F && hardfloat) print("\tfmv.x.s x%d,f%d\n", src, src);
      }
      break;
    case ASGN+B:
      dalign = p->syms[1]->u.c.v.i;
      salign = p->syms[1]->u.c.v.i;
      blkcopy(getregnum(p->x.kids[0]), 0,
              getregnum(p->x.kids[1]), 0,
              p->syms[0]->u.c.v.i, tmpregs);
      break;    
  }
}


static void doarg(Node p) {
  static int argno;
  int align;
  int size;
  int offset;

  if (argoffset == 0) {
    argno = 0;
  }
  p->x.argno = argno++;
  align = p->syms[1]->u.c.v.i;
  if (align < 4) {
    align = 4;
  }
  size = p->syms[0]->u.c.v.i;
  offset = mkactual(align, size);
  p->syms[2] = intconst(offset);
}


static void target(Node p) {
  static int ty0;
  int ty;
  Symbol q;

  assert(p);
  switch (specific(p->op)) {
    case DIV + F:
    case MOD + F:
    case MUL + F:
    case ADD + F:
    case SUB + F:
    case EQ + F:
    case NE + F:
    case LE + F:
    case LT + F:
    case GT + F:
    case GE + F:
      if (!hardfloat) {
      setreg (p, ireg[10]);
      rtarget (p, 0, ireg[12]);
      rtarget (p, 1, ireg[13]);
      }
      break;
    case NEG + F:
    case CVI + F:
    case CVF + I:
      if (!hardfloat) {
      setreg (p, ireg[10]);
      rtarget (p, 0, ireg[12]);
      }
      break;
    case CNST+I:
    case CNST+P:
    case CNST+U:
      if (range(p, 0, 0) == 0) {
        setreg(p, ireg[0]);
        p->x.registered = 1;
      }
      break;
    case CALL+I:
    case CALL+P:
    case CALL+U:    
      rtarget(p, 0, ireg[31]);
      setreg(p, ireg[10]);
      break;
    case CALL+F:
      rtarget(p, 0, ireg[31]);
      if(hardfloat)
        setreg(p, freg[10]);
      else 
        setreg(p, ireg[10]);
      break;
    case CALL+V:
      rtarget(p, 0, ireg[31]);
      break;
    case RET+I:
    case RET+P:
    case RET+U:    
      rtarget(p, 0, ireg[10]);
      break;
    case RET+F:
        if(hardfloat)
          rtarget(p, 0, freg[10]);
        else
          rtarget(p, 0, ireg[10]);
      break;
    case ARG+I:
    case ARG+P:
    case ARG+U:
    case ARG+F:
      ty = optype(p->op);
      q = argreg(p->x.argno, p->syms[2]->u.c.v.i, ty, opsize(p->op), ty0);
      if (p->x.argno == 0) {
        ty0 = ty;
      }
      if (q) {
        rtarget(p, 0, q);
      }
      break;
    case ASGN+B:
      rtarget(p->kids[1], 0, blkreg);
      break;
    case ARG+B:
      rtarget(p->kids[0], 0, blkreg);
      break;
  }
}


static void clobber(Node p) {
  assert(p);
  switch (specific(p->op)) {
    case CALL+F:
      if (hardfloat) spill(FLTTMP, FREG, p);
      else spill(INTTMP, IREG, p);      
     break;
    case CALL+I:
    case CALL+P:
    case CALL+U:
      spill(INTTMP, IREG, p);
      if (hardfloat) spill(FLTTMP | FLTRET, FREG, p);
      break;         
    case ADD+F:
    case SUB+F:
    case NEG+F:
    case DIV+F:
    case MOD+F:
    case MUL+F:
    case EQ+F:
    case NE+F:
    case LE+F:
    case LT+F:
    case GT+F:
    case GE+F:
    case CVI+F:
    case CVF+I:
      if(hardfloat) break;
      spill(INTTMP, IREG, p);      
      break;      
    case CALL+V:
      spill(INTTMP | INTRET, IREG, p);
      if (hardfloat) spill(FLTTMP | FLTRET, FREG, p);
      break;
  }
}

static int
mulops_calls (int op)
{

  if ((generic (op) == ADD || generic (op) == SUB || generic (op) == DIV
  || generic (op) == MOD || generic (op) == MUL) &&   optype (op) == F)
    return 1;

  if (generic (op) == NEG && optype (op) == F)
    return 1;

  if (generic (op) == CVI && optype (op) == F)
    return 1;

  if (generic (op) == CVF && optype (op) == I)
    return 1;

  return 0;
}

extern void stabinit(char *, int, char *[]);
static void stabline(Coordinate *);
extern void stabsym(Symbol);
extern void stabtype(Symbol);
extern void stabblock(int, int, Symbol*);
extern void stabfend(Symbol, int);
static char *currentfile;
static int curfileno=1;

/* stabinit - initialize stab output */
static void stabinitrv(char *file, int argc, char *argv[]) {        
		if (file) {
                print("\t.file %d \"%s\"\n", curfileno, file);
                currentfile = file;				
        }
		stabinit(NULL, 0, NULL);
}

/* stabline - emit stab entry for source coordinate *cp */
static void stabline(Coordinate *cp) {
        if (cp->file && cp->file != currentfile) {
				curfileno++;
                print("\t.file %d \"%s\"\n", curfileno, cp->file);
                currentfile = cp->file;				
        }
        print("\t.loc %d %d\n", curfileno, cp->y);

}

/* stabsym - output a stab entry for symbol p */
//static void stabsym(Symbol p) {
//        if (p == cfunc && IR->stabline)
//                (*IR->stabline)(&p->src);
//}

Interface riscv32IR = {
  1, 1, 0,      /* char */
  2, 2, 0,      /* short */
  4, 4, 0,      /* int */
  4, 4, 0,      /* long */
  4, 4, 0,      /* long long */
  4, 4, 1,      /* float */
  4, 4, 1,      /* double */
  4, 4, 1,      /* long double */
  4, 4, 0,      /* T * */
  0, 1, 0,      /* struct */
  1,            /* little_endian */
  mulops_calls, /* mulops_calls */
  0,            /* wants_callb */
  0,            /* wants_argb */
  1,            /* left_to_right */
  0,            /* wants_dag */
  0,            /* unsigned_char */
  address,
  blockbeg,
  blockend,
  defaddress,
  defconst,
  defstring,
  defsymbol,
  emit,
  export,
  function,
  gen,
  global,
  import,
  local,
  progbeg,
  progend,
  segment,
  space,
  stabblock, stabend, stabfend, stabinitrv, stabline, stabsym, stabtype,
  {
    1,      /* max_unaligned_load */
    rmap,
    blkfetch, blkstore, blkloop,
    _label,
    _rule,
    _nts,
    _kids,
    _string,
    _templates,
    _isinstruction,
    _ntname,
    emit2,
    doarg,
    target,
    clobber
  }
};
