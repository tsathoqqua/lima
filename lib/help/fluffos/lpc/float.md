# float

MudOS LPC now provides support for the floating point type. Declare
variables like this:

    float pi;

In general the same operations are supported for floats as are for integers.
Floating point constants may contain decimal points e.g.

    pi = 3.14159265;

The LPC float type is based on the single precision floating point type
provided by C. On most machines this will give about seven (7) digits
of precision (in base 10).
