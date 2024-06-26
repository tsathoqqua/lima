# clonep

### NAME

    clonep() - determine whether or not a given variable points to a cloned
    object

### SYNOPSIS

    int clonep( void | mixed arg );

### DESCRIPTION

    Returns true (1) iff the argument is objectp() and the O_CLONE flag  is
    set.   The  driver  sets the O_CLONE flag for those objects created via
    new(3) (clone_object(3)).  The clonep() efun will not return true  when
    called  on  objects that are the master copy (those that are loaded via
    call_other(3)).  Note that if clonep() returns true,  then  file_name()
    will   return   a  string  containing  a  '#'.   clonep()  defaults  to
    this_object().

### SEE ALSO

    virtualp(3), userp(3), wizardp(3), objectp(3), new(3), clone_object(3),
    call_other(3), file_name(3)

