# switch

### The switch statement

The LPC switch statement is nearly identical to the C switch statement.

The only real difference is that the cases of the LPC switch may be strings
as well as integers.

Syntax is as follows:

```
switch (expression) {
  case constant0 :
    statements0;
    break;
  case constant1 :
    statements1;
    break;
  default :
    statements2;
}
```

The switch is a replacement for the chained if else if else if else
construct. The above switch is equivalent to:

```
tmp = expression;
if (tmp == constant0) {
    statements0;
    ...;
} else if (tmp == constant1) {
    statements1;
    ...;
} else {
    statements2;
    ...;
}
```

The main difference between the switch and the if statement is that if
the "break;" statement is ommited from the end of a particular case,
then the statements in the next case will be executed as well.
