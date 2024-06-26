# bind

### NAME

    bind() - change the owner of a function pointer

### SYNOPSIS

    function bind(function f, object ob)

### DESCRIPTION

    Returns  a  function pointer that is exactly the same as f, but belongs
    to the object 'ob' instead of the object that created f.  Useful if the
    creator  of 'f' has been destructed, or f is an efun pointer to an efun
    which does something to 'this_object'.

    For example:

    void make_living(object ob) {
       function f;

       f = bind( (: enable_commands :), ob );

       evaluate(f); }

    The effect of the above is the same as if 'ob' itself had evaluated the
    enable_commands() efun.  Note that there are security risks involved in
    this, as bind() allows you to force another object to run  a  piece  of
    code.   To  protect  against this, there is a valid_bind() master apply
    which must return 1 or the call to bind() will fail.

