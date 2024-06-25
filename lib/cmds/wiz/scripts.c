/* Do not remove the headers from this file! see /USAGE for more info. */

//: COMMAND
// USAGE: ``scripts``
//
// Shows status of scripted NPCs.
//
// .. TAGS: RST

inherit CMD;
inherit M_WIDGETS;

private
void main(string str)
{

   object *scripted = filter(objects(), ( : $1->query_running_script() && clonep($1) :));

   printf("<bld>%-20.20s %-20.20s %-14.14s %s<res>", "Who", "Env", "Script", "Status");
   foreach (object s in scripted)
   {
      string name = s->short();
      string env = environment(s) ? environment(s)->short() : "Nowhere";
      string bar = repeat_string(" ", 50);
      string script = "";
      int *status = s->status();
      if (status)
      {
         bar = green_bar(status[0], status[1], 50);
         script = s->query_running_script();
         printf("%-20.20s %-20.20s %-14.14s %s %s", name, env, script, bar, "[" + status[0] + "/" + status[1] + "]");
      }
   }
}
