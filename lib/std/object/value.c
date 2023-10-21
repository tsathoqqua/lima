/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** Moved from M_VALUABLE into OBJ. We want players to clean the MUD, so everything potentially has value. Besides, this
** is one integer and 3 functions. We will be okay.
*/

int object_value = 0;

//: FUNCTION set_value
// Set the value of the item.
void set_value(int value)
{
   object_value = value;
}

//: FUNCTION query_value
// Return the value of the item.
int query_value()
{
   return object_value;
}

//: FUNCTION nobody_will_buy
// Override this function to set this item as unsellable even though it has a
// value.
int nobody_will_buy()
{
   return 0;
}
