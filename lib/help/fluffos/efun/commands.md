# commands

### NAME

    commands() - returns some information about actions the user can take

### SYNOPSIS

    mixed *commands( void );

### DESCRIPTION

    Returns an array of an array of 4 items describing the actions that are
    available to this_object().  The first item is the command  itself  (as
    passed  to  add_action()).   The  second is the set of flags (passed to
    add_action as the third argument, often defaulted to 0).  The third  is
    the  object  that defined the action.  The fourth is the function to be
    called ("<function>" if it is a function pointer).

### SEE ALSO

    add_action(3), enable_commands(3), disable_commands(3)

