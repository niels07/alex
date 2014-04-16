Apex 
===

Apex is a simple bytecode calculator (inspired by Lua). 

It takes a string, parses, compiles and executes it.
After compling using 'make' run the program followed by an expression between quotes:

```
 $ ./apex "(5 * ((1 + 1) * 2) + 1) ^ 2"
 441.000000
```

It can also be used as a library for you applications after you've installed apex:

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



