# deep_inherit_list

### NAME

    deep_inherit_list() - get a list of ancestors of an object

### SYNOPSIS

    string *deep_inherit_list( object obj );

### DESCRIPTION

    Returns  an  array  of filenames of all objects inherited (directly and
    indirectly) by obj.

    If no object is supplied, this efun will default to this_object().

### SEE ALSO

    inherit_list(3), inherits(3)

