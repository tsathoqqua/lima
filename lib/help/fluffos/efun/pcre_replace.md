# pcre_replace

### NAME

    pcre_replace()

### SYNOPSIS

    string pcre_replace(string input, string pattern, string *replacments);

### DESCRIPTION

    returns a string where all captured groups have been replaced by the
    elements of the replacement array. Number of subgroups and the size of the
    replacement array must match.

### SEE ALSO

    pcre_assoc(3), pcre_cache(3), pcre_extract(3)
