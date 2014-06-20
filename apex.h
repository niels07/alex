/* apex.c: header file for apex.c. */

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

/* Execute a file. */
extern unsigned apex_dofile(const char * /* path */);

/* Cleanup. */
extern void apex_end(void);

#endif /* APEX_H */

