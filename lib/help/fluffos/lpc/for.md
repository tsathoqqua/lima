# for

### The LPC for loop

The LPC for loop is also identical to that provided by C. Syntax is as
follows:

```c
for (expression0; expression1; expression2) {
    statements;
    ...;
}
```

Expression0 is evaluated once prior to the execution of the loop. Expression1
is evaluated at the beginning of each iteration of the loop. If expression1
evaluates to zero, then the loop terminates. Expression2 is evaluated at
the end of each loop iteration.

A `break` in the body of the loop will terminate the loop. A `continue` will
continue the execution from the beginning of the loop (after evaluating
Expression2).

A typical usage of the for loop is to execute a body of code some
fixed number of times:

```c
int i;

for (i = 0; i < 10; i++) {
    write("i == " + i + "\n");
    write("10 - i == " + (10 - i) + "\n");
}
```
