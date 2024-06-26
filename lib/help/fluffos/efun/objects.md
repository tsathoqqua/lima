# objects

### NAME

    objects - return an array of all loaded objects

### SYNOPSIS

    object *objects( void | string func | function f );

### DESCRIPTION

    An array of every object loaded on the mud is  returned  by  objects().
    Note  that if the system's maximum array size is set too low, objects()
    will truncate its array, in which case it might not be too useful.

    If the optional 'func' parameter is given, then func() in this_object()
    is  called  with  each  loaded  object as an argument.  If the function
    returns nonzero, then that object is returned by  objects(),  otherwise
    it isn't.

    If  'f'  is  given, it will be called on all the objects as above.  For
    example, objects( (: clonep :) ) returns a list of all  the  clones  in
    existence.

### SEE ALSO

    livings(3), users(3)

