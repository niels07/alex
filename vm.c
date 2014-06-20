/* vm.c: apex virtual machine. */

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

#include <stdlib.h>
#include <math.h>
#include "vm.h"
#include "table.h"
#include "util.h"
#include "apex.h"

#define GROW_STACK 8

static char *opnames[] = { 
    "halt", 
    "num", 
    "add", 
    "sub", 
    "mult", 
    "div", 
    "pwr", 
    "rpar", 
    "lpar", 
    "store", 
    "print", 
    "var" 
};

/* Stack with the instructions. */
static Instruction *instr_stack = NULL;

/* Number of instructions on the stack. */
static Uint instr_count = 0;

/* Current allocated size of the stack. */
static size_t instr_size = 255;

static float op_stack[255];

/* Top index of the stack. */
static int top = 0;

static float execop(Instruction * /* instructions */);

static void
print_instruction(Instruction instruction)
{
    if (instruction.op == OP_NUM)
        printf("%s (%f)\n", opnames[instruction.op], instruction.arg);
    else
        printf("%s\n", opnames[instruction.op]);
}

static Instruction *
eval_parens(Instruction *instructions)
{
    Uint pc = 0, paren_start = 0;
    Instruction instr;
    Instruction *paren_instr;
    Instruction *newstack;
    Uint paren_instr_count = 0;
    Uint newstack_count = 0;
    size_t paren_instr_size = 255;
    Uint paren_count = 0;

    instr =  instructions[pc++];
    paren_instr = apex_malloc(paren_instr_size);
    newstack = apex_malloc(instr_size);

    while (instr.op != OP_HALT) {
        /* print_instruction(instr); */
        switch (instr.op) {
        case OP_LPAR: 
            if (paren_count == 0)
                paren_start = pc - 1;
            paren_count++;
            break;

        case OP_RPAR:
            if (paren_count == 0)
                break;

            paren_count--;
            if (paren_count > 0)  {
                paren_instr[paren_instr_count].op = instr.op;
                paren_instr[paren_instr_count].arg = instr.arg;
                paren_instr_count++;
                break;
            }

            paren_instr[paren_instr_count].op = OP_HALT;
            paren_instr[paren_instr_count].arg = 0;
            newstack[newstack_count].op = OP_NUM;
            newstack[newstack_count].arg = execop(paren_instr);
            newstack_count++;
            paren_instr_count = 0;
            paren_instr_size = 255;
            paren_instr = apex_malloc(paren_instr_size);
            break;

        default:
            if (paren_count == 0) {
                newstack[newstack_count].op = instr.op;
                newstack[newstack_count].arg = instr.arg;
                newstack_count++;
                break;
            }

            if (paren_instr_count == paren_instr_size) {
                paren_instr_size += GROW_STACK;
                paren_instr = apex_realloc(paren_instr, paren_instr_size);
            }
            paren_instr[paren_instr_count].op = instr.op;
            paren_instr[paren_instr_count].arg = instr.arg;
            paren_instr_count++;
            break;
        }

        instr = instructions[pc++];
    }
    free(paren_instr);
    free(instructions);
    newstack[newstack_count].op = OP_HALT;
    newstack[newstack_count].arg = 0;
    return newstack;
}

static float
execop(Instruction *instructions)
{
    int pc = 0, stack_count = 0;
    Instruction instr;
    float *stack, result;
    size_t stack_size = 255;
    
    instructions = eval_parens(instructions);

    instr = instructions[pc++];
    stack = apex_malloc(stack_size * sizeof(float));

    while (instr.op != OP_HALT) {
        switch (instr.op) {
        case OP_ADD:
            stack[stack_count - 1] = stack[stack_count - 1] + stack[stack_count];
            stack_count--;
            break;

        case OP_SUB:
            stack[stack_count - 1] = stack[stack_count - 1] - stack[stack_count];
            stack_count--;
            break;

        case OP_MULT:
            stack[stack_count - 1] = stack[stack_count - 1] * stack[stack_count];
            stack_count--;
            break;
        
        case OP_DIV:
            stack[stack_count - 1] = stack[stack_count - 1] / stack[stack_count];
            stack_count--;
            break;
        
        case OP_POW:
            stack[stack_count - 1] = pow(stack[stack_count - 1], stack[stack_count]);
            stack_count--;
            break;

        case OP_NUM:
            if (stack_count == stack_size) {
                stack_size += GROW_STACK;
                stack = apex_realloc(stack, stack_size);
            }
            stack[++stack_count] = instr.arg;
            break;

        case OP_HALT:
            break;

        default:
            apex_error("internal error: unkown opcode (%d)", instr.op);
            break;
        }

        instr = instructions[pc++];
    }
    result = stack[stack_count];
    free(stack);
    return result;
}

/* Execute a series of opcodes. */
void
apex_execop(void)
{
    Uint pos = 0;

    instr_stack = eval_parens(instr_stack);
    Instruction instr = instr_stack[pos++];

    while (instr.op != OP_HALT) {
        switch (instr.op) {
        case OP_ADD:
            op_stack[top - 1] = op_stack[top - 1] + op_stack[top];
            top--;
            break;

        case OP_SUB:
            op_stack[top - 1] = op_stack[top - 1] - op_stack[top];
            top--;
            break;

        case OP_MULT:
            op_stack[top - 1] = op_stack[top - 1] * op_stack[top];
            top--;
            break;
        
        case OP_DIV:
            op_stack[top - 1] = op_stack[top - 1] / op_stack[top];
            top--;
            break;
        
        case OP_POW:
            op_stack[top - 1] = pow(op_stack[top - 1], op_stack[top]);
            top--;
            break;

        case OP_NUM:
            op_stack[++top] = instr.arg;
            break;

        case OP_STORE:
            op_stack[(int)instr.arg] = op_stack[top];
            top--;
            break;

        case OP_PRINT:
            fprintf(stdout, "%f\n", op_stack[top]);
            top--;
            break;

        case OP_EQ: 
            op_stack[top - 1] = op_stack[top - 1] == op_stack[top];
            top--;
            break;

        case OP_NE: 
            op_stack[top - 1] = op_stack[top - 1] != op_stack[top];
            top--;
            break;

        case OP_GT:
            op_stack[top - 1] = op_stack[top - 1] > op_stack[top];
            top--;
            break;

        case OP_LT:
            op_stack[top - 1] = op_stack[top - 1] < op_stack[top];
            top--;
            break;
        
        case OP_LE:
            op_stack[top - 1] = op_stack[top - 1] <= op_stack[top];
            top--;
            break;

        case OP_GE:
            op_stack[top - 1] = op_stack[top - 1] >= op_stack[top];
            top--;
            break;

         case OP_VAR:
            op_stack[++top] = op_stack[(int)instr.arg];
            break;

        case OP_JMP_FALSE:
            if (op_stack[top] == 0)
                pos = instr.arg;
            top--;
            break;

        case OP_JMP_TRUE:
            if (op_stack[top] > 0)
                pos = instr.arg;
            break;

        case OP_HALT:
            break;

        default:
            apex_error("internal error: unkown opcode (%d)", instr.op);
            break;
        }

        instr = instr_stack[pos++];
    }
}

/* Return integer value at OFFSET. */
int 
apex_getint(const int offset)
{
    return (offset < 0) ? op_stack[top + offset] : op_stack[offset];
}

float
apex_getnumber(const int offset)
{
    return (offset < 0) ? op_stack[top + offset] : op_stack[offset];
}

int
apex_gettop(void)
{
    return top;
}

/* Add operation to the stack. */
void
apex_addop(Opcode op, const float arg) 
{
    if (instr_count == 0)
        instr_stack = apex_malloc(instr_size);

    if (instr_count == instr_size) {
        instr_size += GROW_STACK;
        instr_stack = apex_realloc(instr_stack, instr_size);
    }
    instr_stack[instr_count].op = op;
    instr_stack[instr_count].arg = arg;
    instr_count++;
}

/* Set operation at specific address. */
void 
apex_setop(Uint addr, Opcode op, const float arg)
{
    instr_stack[addr].op = op;
    instr_stack[addr].arg = arg;
}

/* Increment the offset and return the previous one. */ 
Uint
apex_nextpos(void)
{
    return instr_count++;
}

/* Return the current offset. */
Uint
apex_getpos(void)
{
    return instr_count;
}

/* Clear the instruction stack. */
void
apex_reset(void)
{
    if (instr_stack != NULL)
        free(instr_stack);

    instr_stack = NULL;
    instr_count = 0;
    instr_size = 255;
}

void 
apex_setvar(Opcode op, const char *name)
{
    Symbol *symbol = apex_addsymbol(name);
    apex_addop(op, symbol->offset);
}
