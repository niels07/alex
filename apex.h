#ifndef APEX_H
#define APEX_H

/* Create and print an error message for apex based on
   FORMAT and following arguments. */
extern void apex_error(const char * /* format */, ...);

extern unsigned int apex_dostring(const char * /*string */);

/* Return integer value at OFFSET. */
extern int apex_getint(const int /* offset */);

/* Return number value at OFFSET. */
extern float apex_getnumber(const int /* offset */);

/* Execute a series of opcodes. */
void apex_execop(void);

/* Add an operation to the stack. */
extern void apex_addop(unsigned int /* op */, const float /* arg */); 

/* Get index of top element. */
extern int apex_gettop(void);

#endif /* APEX_H */

