#ifndef VM_H
#define VM_H

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
    OP_RPAR
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

#endif /* VM_H */
