# lpc

- What is LPC?

LPC is the language in which MudOS (and LPmud) objects are written.
LPC stands for Lars Pensj| C. As one might surmise from the name,
LPC is based on the syntax of C. LPC provides the C while loop, for loop,
if statement, switch statement, a variant of sscanf, and integer data type,
(LPC also provides other data types not in C such as the object and the
mapping). LPC uses C's syntax for defining and calling functions and for
declaring variables. Note that LPC's version of the string datatype is
much different from that provided by C. See the LPC tutorial on syntax
and language constructs for more information.

Here are some differences between LPC and C:

There is no need for a function named "main" in LPC objects (although there
is one called "create").

The efuns (or system calls) provided by the gamedriver are different than
those typically found in the C library (libc.a).

There is no malloc(). However, there is an allocate(int value) efun that
lets space be allocated for arrays. Note that the argument to 'allocate'
is not in units of bytes, but rather in units of elements.

Memory is never explicitly deallocated. The gamedriver keeps track of
how many times a given piece of data has been referenced. When the
reference count goes to zero (when no object has a copy of that variable),
then the space used by the variable is reclaimed (garbage collected).

The string data type in LPC is closer to that provided by BASIC than that
provided by C. Strings are not declared as arrays of characters but rather
as a basic intrinsic type. Strings may be concatenated using the '+' operator.

For example, the LPC statements:

string ack;

ack = foo + bar;

are equivalent to the C statements:

char \*ack;

ack = (char _)malloc(strlen(foo) + 1);
strcpy(ack,foo);
ack = (char _)realloc(strlen(ack) + strlen(bar) + 1);
strcat(ack,bar);

LPC is an interpreted language (however it is compiled into an internal
compact tokenized form before being interpreted).

LPC has no structures or unions. In fact, the -> operator is used to
indicate a call to another object. The mapping datatype can serve
as an effective substitute for structures in some situations.

sscanf does not work in the same way as in C. arguments to sscanf need not
be pointers (since LPC does not have the explicit pointer data type). Also,
sscanf(arg,"%s %s",str1,str2) does not operate as the C programmer would
expect. In C, the first word of arg would be copied into str1 and the
second word of arg into str2. In LPC, the first word is copied into str1
and the _remainder_ of arg is copied into str2.
