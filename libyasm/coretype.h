/* $IdPath$
 * YASM core (used by many modules/header files) type definitions.
 *
 *  Copyright (C) 2001  Peter Johnson
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of other contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND OTHER CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR OTHER CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef YASM_CORETYPE_H
#define YASM_CORETYPE_H

typedef struct arch arch;

typedef struct preproc preproc;
typedef struct parser parser;
typedef struct optimizer optimizer;
typedef struct objfmt objfmt;
typedef struct dbgfmt dbgfmt;

typedef struct bytecode bytecode;
typedef /*@reldef@*/ STAILQ_HEAD(bytecodehead, bytecode) bytecodehead;

typedef struct section section;
typedef /*@reldef@*/ STAILQ_HEAD(sectionhead, section) sectionhead;

typedef struct symrec symrec;

typedef struct expr expr;
typedef struct intnum intnum;
typedef struct floatnum floatnum;

typedef struct linemgr linemgr;
typedef struct errwarn errwarn;

typedef enum {
    EXPR_ADD,
    EXPR_SUB,
    EXPR_MUL,
    EXPR_DIV,
    EXPR_SIGNDIV,
    EXPR_MOD,
    EXPR_SIGNMOD,
    EXPR_NEG,
    EXPR_NOT,
    EXPR_OR,
    EXPR_AND,
    EXPR_XOR,
    EXPR_SHL,
    EXPR_SHR,
    EXPR_LOR,
    EXPR_LAND,
    EXPR_LNOT,
    EXPR_LT,
    EXPR_GT,
    EXPR_EQ,
    EXPR_LE,
    EXPR_GE,
    EXPR_NE,
    EXPR_SEG,
    EXPR_WRT,
    EXPR_SEGOFF,    /* The ':' in SEG:OFF */
    EXPR_IDENT	    /* no operation, just a value */
} ExprOp;

/* EXTERN and COMMON are mutually exclusive */
typedef enum {
    SYM_LOCAL = 0,		/* default, local only */
    SYM_GLOBAL = 1 << 0,	/* if it's declared GLOBAL */
    SYM_COMMON = 1 << 1,	/* if it's declared COMMON */
    SYM_EXTERN = 1 << 2		/* if it's declared EXTERN */
} SymVisibility;

/* Determines the distance, in bytes, between the starting offsets of two
 * bytecodes in a section.
 * Inputs: sect, the section in which the bytecodes reside.
 *         precbc1, preceding bytecode to the first bytecode
 *         precbc2, preceding bytecode to the second bytecode
 * Outputs: dist, the distance in bytes (bc2-bc1)
 * Returns distance in bytes (bc2-bc1), or NULL if the distance was
 *  indeterminate.
 */
typedef /*@null@*/ intnum * (*calc_bc_dist_func) (section *sect,
						  /*@null@*/ bytecode *precbc1,
						  /*@null@*/ bytecode *precbc2);

/* Converts an expr to its byte representation.  Usually implemented by
 * object formats to keep track of relocations and verify legal expressions.
 * Inputs:
 *  ep      - (double) pointer to the expression to output
 *  bufp    - (double) pointer to buffer to contain byte representation
 *  valsize - the size (in bytes) to be used for the byte rep
 *  offset  - the offset (in bytes) of the expr contents from the bc start
 *  sect    - current section (usually passed into higher-level calling fct)
 *  bc      - current bytecode (usually passed into higher-level calling fct)
 *  rel     - should the expr be treated as PC/IP-relative? (nonzero=yes)
 *  d       - objfmt-specific data (passed into higher-level calling fct)
 * Returns nonzero if an error occurred, 0 otherwise
 */
typedef int (*output_expr_func) (expr **ep, unsigned char **bufp,
				 unsigned long valsize, unsigned long offset,
				 /*@observer@*/ const section *sect,
				 /*@observer@*/ const bytecode *bc, int rel,
				 /*@null@*/ void *d)
    /*@uses *ep@*/ /*@sets **bufp@*/;

/* Converts a objfmt data bytecode into its byte representation.  Usually
 * implemented by object formats to output their own generated data.
 * Inputs:
 *  type - objfmt-specific type
 *  data - objfmt-specific data
 *  bufp - (double) pointer to buffer to contain byte representation
 * bufp is guaranteed to have enough space to store the data into (as given
 * by the original bc_new_objfmt_data() call).
 * Returns nonzero if an error occurred, 0 otherwise.
 */
typedef int (*output_bc_objfmt_data_func) (unsigned int type,
					   /*@observer@*/ void *data,
					   unsigned char **bufp)
    /*@sets **bufp@*/;
#endif
