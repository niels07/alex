#include <stdlib.h>
#include <math.h>
#include "vm.h"
#include "util.h"
#include "apex.h"

#define GROW_STACK 8

static char *opnames[] = { "halt", "num", "add", "sub", "mult", "div", "pwr", "rpar", "lpar" };

static Instruction *instr_stack;
static Uint instr_count = 0;
static size_t instr_size = 255;

static float op_stack[255];

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
        print_instruction(instr);
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
    int pc = 0;

    instr_stack = eval_parens(instr_stack);
    Instruction instr = instr_stack[pc++];

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

        case OP_HALT:
            break;

        default:
            apex_error("internal error: unkown opcode (%d)", instr.op);
            break;
        }

        instr = instr_stack[pc++];
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
