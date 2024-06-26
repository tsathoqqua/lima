# if

### The if else statement

LPC's if statement is identical to that provided by C. Syntax is as follows:

```c
if (expression)
    statement;
```

Alternately:

```c
if (expression) {
    statements;
}
```

Alternately:

```c
if (expression0) {
    statements;
} else {
    statements1;
}
```

Alternately:

```c
if (expression0) {
    statements0;
} else if (expression1) {
    statements1;
}
```

The number of else clauses is not explicitly limited.

---

Another favorite programming construct is the ? : operator, which also
operates identical to C. The syntax is:

    expression0 ? expression1_if_true : expression2_if_false

In some cases, ? : is an shorter way of expression constructs such as:

```c
if (expression0)
    var = expression1;
else
    var = expression2;
```

which can be equivalently translated to:

    var = expression0 ? expression1 : expression;
