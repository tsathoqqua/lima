# strings

string sub ranging - comments by Grey@TMI-2

You can take a substring from a variable ( str ) buy using the substring
operation ( str[n1..n2] ). Positive values are taken from the left and
values with `<` from the right. If a value is greater than the length of
the string it will be treated as being equal to the length of the string.

If the two values are equal ( str[n1..n1] ) then the character at that
position ( n1 ) is returned. If both values point to positions beyond
the same end of the string the null string ( "" ) is returned. If the
position pointed to by the first value is after the one pointed to by
the second then the null string is also returned.

Examples:

    str = "abcdefg"

    str[0..0] ==       "a"
    str[0..<1] ==      "abcdefg"
    str[<4..<2] ==     "def"
    str[<7..6] ==      "abcdefg"
    str[3..2] ==       ""
    str[2..] ==        "cdefg"
