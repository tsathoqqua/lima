# db_exec

### NAME

    db_exec() - executes an sql statement

### SYNOPSIS

    mixed db_exec( int handle, string sql_query );

### DESCRIPTION

    This function will execute the passed sql statement for the given data‐
    base handle.

    Returns the number of rows in result set on success, or an error string
    otherwise.

### SEE ALSO

    db_commit(3), db_fetch(3), db_rollback(3), async_db_exec(3),
    valid_database(4)

