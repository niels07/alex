/* vm.h: apex virtual machine. */

/*
 * Apex calculator
 * by Niels Vanden Eynde
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef VM_H
#define VM_H

#include "util.h"

/* Opcode definions. */
typedef enum {
    OP_HALT,
    OP_NUM,
    OP_ADD,
    OP_SUB, 
    OP_MULT,
    OP_DIV,
    OP_POW,
    OP_LPAR,
    OP_RPAR,
    OP_STORE,
    OP_PRINT,
    OP_VAR,
    OP_EQ,
    OP_NE,
    OP_LT,
    OP_GT,
    OP_GE,
    OP_LE,
    OP_JMP_TRUE,
    OP_JMP_FALSE
} Opcode;

/* Instructions for the vm. */
typedef struct {
    Opcode op;
    float arg;
} Instruction;

/* Return integer value at OFFSET. */
extern int apex_getint(const int /* offset */);

/* Return number value at OFFSET. */
extern float apex_getnumber(const int /* offset */);

/* Execute a series of opcodes. */
void apex_execop(void);

/* Add an operation to the stack. */
extern void apex_addop(Opcode  /* op */, const float /* arg */); 

/* Get index of top element. */
extern int apex_gettop(void);

/* Increment the offset and return the previous one. */ 
extern Uint apex_nextpos(void);

/* Return the current offset. */
extern Uint apex_getpos(void);

/* Set operation at specific address. */
extern void apex_setop(Uint /* addr */, Opcode /* op */, const float /* arg */);

/* Set apex variable. */
extern void apex_setvar(Opcode /* op */, const char * /*name */);

/* Clear the instruction stack. */
extern void apex_reset(void);

#endif /* VM_H */
