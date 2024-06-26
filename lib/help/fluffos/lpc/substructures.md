# substructures

## LPC Substructures

### Indexing and Ranging - General Introduction

> Since v20.25a6, MudOS provides a way of indexing or getting

slices (which I will, following common use, call 'ranging') of
strings/buffers/arrays/mappings (for mappings only indexing is
available) as well as means of changing the values of data
via lvalues (i.e. 'assignable values') formed by indexing/ranging.

As an example, if we set str as `"abcdefg"`, `str[0]` will
be `'a'`, `str[1]` `'b'` etc. Similarly, **the nth element of an array
arr is accessed via arr[n-1]**, and the value corresponding to
key x of mapping m, m[x]. The '<' token can be used to denote
indexing from the right, i.e. `str[<x]` means `str[strlen(str) - x]`
if str is a string. More generally `arr[<x]` means `arr[sizeof(arr)-x]`.
(Note that sizeof(arr) is the same as strlen if arr is a string).

Indexed values are reasonable lvalues, so one could do for e.g.
`str[0] = 'g'` to change the 1st character of str to g. Although it is
possible to use `({ 1,2 })[1]` as a value (which is currently optimized
in MudOS to compile to 2 directly), it is not possible to use it as an
lvalue. It is similarly not possible to use `([ "a" : "b" ])["c"]` as an
lvalue (Even if we did support it, it would be useless, since there
is no other reference to the affected mapping). I will describe in
more detail later what are the actually allowed lvalues.

Another method of obtaining a subpart of an LPC value is via
ranging. An example of this is `str[1..2]`, where for str being
`"abcdefg"`, gives `"bc"`. In general `str[n1..n2]` returns a substring
consisting of the (n1+1) to (n2+1)th characters of str. If n1 or n2
is negative, and the driver is compiled with OLD_RANGE_BEHAVIOR
defined, then it would take the negative indices to mean counting
from the end. Unlike indexing though, ranges with indexes which are
out of bounds do not give an error. Instead if a maximal subrange
can be found within the range requested that lies within the bounds
of the indexed value, it will be used. So for e.g., without
OLD_RANGE_BEHAVIOR, `str[-1..2]` is the same as `str[0..2]`. All other
out of bounds ranges will return "" instead, which corresponds
to the idea that there is no (hence there is one, namely the empty one)
subrange within the range provided that is within bounds. Similarly,
for array elements, `arr[n1..n2]` represents the slice of the array
with elements (n1+1) to (n2+1), unless out of bounds occur.
OLD_RANGE_BEHAVIOR is only supported for buffers and strings. However,
I suggest you not use it since it maybe confusing at times (i.e. in
`str[x..y]` when x is not known at hand, it may lead to an unexpected
result if x is negative). One can however, also use < in ranging
to mean counting from the end. So `str[<x..<y]` means
`str[strlen(str)-x..strlen(str)-y]`.

Remark: If OLD_RANGE_BEHAVIOR is defined, then the priority of <
is higher than the priority of checking if it's negative.
That is, if you do `str[<x..y]`, it will mean the same
as `str[strlen(str)-x..y]`, meaning therefore that it will
check only now if `strlen(str)-x` is negative and if so,
takes it to be from the end, leading you back to x again

Thus far, `str[<x..<y]`, `str[<x..y]`, `str[x..<y]`, `str[<x..]` (meaning the
same as `str[<x..<1]`) and `str[x..]` (same as `str[x..<1]`) are supported.
The same holds for arrays/buffers.

Perhaps this might seem strange at first, but ranges also
are allowed to be lvalues! The meaning of doing

```c
str[x..y] = foo; // (1)
```

is basically 'at least' doing

```c
str = str[0..x-1] + foo + str[y+1..]; // (2)
```

Here x can range from 0..sizeof(str) and y from -1 to
sizeof(str) - 1. The reason for these bounds is because, if I
wanted to add foo to the front,

    str = foo + str;

this is essentially the same as

    str = str[0..-1] + foo + str;

since str[0..-1] is just "".

Remark: As far as range lvalues are concerned, negative indexes
do not translate into counting from the end even if
OLD_RANGE_BEHAVIOR is defined. Perhaps this is confusing,
but there is no good way of allowing for range lvalue
assignments which essentially result in the addition
of foo to the front as above otherwise

Hence, that's the same as doing

    str = str[0..0-1] + foo + str[0..];

or, what's the same

    str = str[0..0-1] + foo + str[-1+1..];

Now if you compare this to (1) and (2) you see finally that that
conforms to the prescription there if we do str[0..-1] = foo!! (Yes,
those exclamation marks are not part of the code, and neither is
this :))

Similarly, I will leave it to you to verify that

```c
str[sizeof(str)..sizeof(str)-1] = foo; // (3)
```

would lead to `str = str + foo`. Now, we can use < in range lvalues
as well, so (3) could have been written as

```c
str[<0..<1] = foo; // (4)
```

or even

```c
str[<0..] = foo; // (5)
```

which is more compact and faster.

Remark: The code for `str[<0..] = foo;` is generated at compile time
to be identical to that for `str[<0..<1] = foo;` so neither
should be faster than the other (in principle) at runtime,
but (4) is faster than (3).

Now we come to the part where I mentioned 'at least' above. For strings,
we know that when we do `x = "abc"; y = x;`, y has a new copy of the string
"abc". (This isn't always done immediately in the driver, but whenever
y does not have a new copy, and a change is to be made to y, then y
will make a new copy of itself, hence effectively, y has a new copy
in that all simple direct changes to it such as `y[0] = 'g'` does not
change x) For buffers and arrays, however, when we do `y = x`, we don't get
a new copy. So what happens is if we change one, we could potentially
change the other. This is indeed true (as has always been) for assignments
to indexed lvalues (i.e. lvalues of the form y[0]). For range lvalues
(i.e. `x[n1..n2]`), the rule is if the change of the lvalue will not
affect it's size (determined by sizeof for e.g.), i.e. essentially if
n1 and n2 are within x's bounds and the value on the right hand side
has size n2 - n1 + 1, then indeed changing x affects y, otherwise
it will not (i.e. if you do `x[0..-1] = ({ 2,3 })`. This only applies
to arrays/buffers, for strings it will never affect y if we assign to a
range of x.

### More complex lvalues and applications

Since arrays/mappings can contain other arrays/mappings, it

is possible in principle to index them twice or more. So for e.g.
if arr is ({ ({ 1,"foo",3 }),4 }) then arr[0][1] (read as the 2nd
element of arr[0], which is the 1st element of arr) is "foo". If we
do, for e.g. arr[0][2] = 5, then arr will be ({ ({ 1, "foo", 5 }), 4 }).

Remark: by the 'will be' or 'is' above, I mean technically: recursively
equal. (This is just if some people are confused)

Similarly, arr[0][1][1] = 'g' changes arr to ({ ({ 1, "fgo", 3 }),
4 }), and arr[0][1][0..1] = "heh" (note that the right hand side can
have a different length, imagine this being like taking the 1st two
characters out from arr[0][1], which is currently "foo", and putting
"heh" in place, resulting in "heho") gives arr as ({ ({ 1, "heho", 3 }),
4 }). You should now be able to generate more examples at your fancy.

Now I want to discuss some simple applications.
Some of you may know that when we are doing

```c
sscanf("This is a test", "This %s %s test", x, y) //(6)
```

that x and y are technically lvalues. This is since what the driver
does is to parse the original string into the format you give, and
then tries to assign the results (once all of them are parsed) to
the lvalues that you give (here x and y). So, now that we have more
general lvalues, we may do

```c
x = "simmetry";
arr = ({ 1, 2, 3, ({ "This is " }) });

sscanf("Written on a roadside: the char for 'y' has value 121\n",
        "Written on %sside: the char for 'y' has value %d\n",
        arr[3][0][<0..], x[1]); //(7)
```

will result in arr being ({ 1, 2, 3, ({ "This is a road" }) }) and
x being "symmetry". (See how we have extended the string in arr[3][0]
via sscanf?) The driver essentially parses the string to gives the
matches "a road" and 121, it then does the assignments to the lvalues,
which is how we got them as above.

The ++,--,+= and -= operators are supported for char lvalues,
i.e. lvalues obtained by indexing strings. Thus for e.g. to get
an array consisting of 26 elements "test.a","test.b", .., "test.z",
one might use a global var tmp as follows:

```c
mixed tmp;

create(){
    string *arr;

    // ...

    tmp = "test.'";
    arr = map_array(allocate(26), (: tmp[4]++ && tmp :));

    // ...
} // (8)
```

### General syntax of valid lvalues

Finally, as a reference, I will just put here the grammar of
valid lvalues accepted by the driver.

By a basic lvalue I mean a global or local variable name, or a
parameter in a functional function pointer such as \$2.

A basic lvalue is a valid lvalue, and so are indexed lvalues of
basic or indexed lvalues. (Notice that I did not say indexed lvalues of
just basic lvalues to allow for a[1][2]). I will generally call an lvalue
obtained from a basic lvalue by indexing only indexed lvalues.

The following lvalues are also valid at compile time:

```c
(<basic lvalue> <assignment token> <rhs>)[a_1][a_2]...[a_n]
(<indexed lvalue> <assignment token> <rhs>)[a_1][a_2]...[a_n] // (9)
/* Remark: n >= 1 here */
```

assignment token is one of +=, -=, *=, &=, /=, %=, ^=, |=, =, <<=,>>=.

However, because of the same reason that when we assign to a
string, we obtain a new copy, (x = "foo")[2] = 'a' is invalidated at
runtime. (One way to think about this is, essentially, assignment leaves
the rhs as a return value, so x = "foo" returns "foo", the right hand
side, which is not the same "foo" as the one in x. For arrays/buffers
this is no problem because by assigning, we share the array/buffer)

Call the lvalues in (9) complex lvalues. Then the following is
also a valid lvalue:

```c
(<complex lvalue> <assignment token> <rhs>)[a_1][a_2]...[a_n] // (10)
```

and if we now call the above lvalues also complex lvalues, it would
still be consistent, i.e. (((a[0] = b)[1] = c)[2] = d)[3] is an okay
lvalue (though I wouldn't suggest using it for clarity's sake :)).

Now, the last class of valid lvalues are range lvalues, which
are denoted by ranging either a basic, indexed or complex lvalue:

    <basic lvalue>[n1..n2]
    <indexed lvalue>[n1..n2]
    <complex lvalue>[n1..n2]

plus other ranges such as [<n1..<n2] etc. (11)

There is maximally only one 'range' you can take to obtain a
valid lvalue, i.e. arr[2..3][0..1] is not a valid lvalue (note that
a naive interpretation of this syntax is one equivalent to using
arr[2..3] itself)

### Compile-time errors that occur and what they mean

---

Here I put some notes on compile-time errors for valid lvalues,
hopefully to be useful for you.

Err 1:

    Can't do range lvalue of range lvalue

Diagnosis:

    You have done 'ranging' twice, e.g. something like x[2][0..<2][1..2]
    isn't a valid lvalue

Err 2:

    Can't do indexed lvalue of range lvalue.

Diagnosis:

    Something like x[0..<2][3] was done.

Err 3:

    Illegal lvalue, a possible lvalue is (x <assign> y)[a]

Diagnosis:

    Something like (x = foo)[2..3] or (x = foo) was taken to be an lvalue.

Err 4:

    Illegal to have (x[a..b] <assign> y) to be the beginning of an lvalue

Diagnosis:

    You did something as described, i.e. (x[1..6] = foo)[3] is not allowed.

Err 5:

    Illegal lvalue

Diagnosis:

    Oops, we are out of luck here :) Try looking at your lvalue more carefully,
    and see that it obeys the rules described in section 3 above.

### Coming attractions

Perhaps a pointer type will be introduced to allow passing
by reference into functions. Mappings may be multivalued and
multi-indexable.

Author : Symmetry@Tmi-2, IdeaExchange
Last Updated : Tue Jan 10 11:02:40 EST 1995
