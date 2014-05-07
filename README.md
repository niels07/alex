Apex 
========

Apex is a simple bytecode calculator (inspired by Lua). 

It takes a string, parses, compiles and executes it. After compling using 'make' run the program followed 
by a filename, or without an argument to enter statements manually.
a single statement must always end on a semicolon.

```
 $ ./apex 
 apex> x = (5 * ((1 + 1) * 2) + 1) ^ 2;
 apex> print x;
 441.000000
```

API
--------

It can also be used as a library for your applications after you've installed apex:

```
$ make
# make install
```

Then in your application include the apex.h header file
```
#include <apex.h>
```

and link it when compiling:

```
gcc -lapex myprogram.c
```

The following functions are currently supported as an API:

```
void apex_error(const char * format , ...);
```

Create and print an error message for apex based on FORMAT and following arguments.

```
extern int apex_gettop(void);
```

Get index of the top element.

```
int apex_getint(const int offset);

```

Return value at OFFSET as an int.


```
float apex_getnumber(const int offset);
```

Return value at OFFSET as a number (float).

```
unsigned int apex_dostring(const char * string);
```

Parse a string and add the result to the stack

```
void apex_execop(void);
```

Execute the bytecode stack.

```
void apex_addop(unsigned int op, const float arg); 
```

Add an operation to the stack.

```
unsigned int apex_dofile(const char * path);
```

Execute a file.

```
void apex_end(void);
```

Cleanup all allocated memory and data.

An example to execute a single string:

```
#include <apex.h>
#include <stdio.h>

int 
main(void)
{
    /* Parse the string. */
    apex_dostring("(5 * ((1 + 1) * 2) + 1) ^ 2;");

    /* Execute the code resulting from the expression. */
    apex_execop();

    /* Get the index of the top of the stack. */
    int top = apex_gettop();

    /* Get the value at that index. */
    int result = apex_getint(top);

    /* print the result. */
    printf("%d\n", result);

    return 0;
}

```

which will output in '441'



