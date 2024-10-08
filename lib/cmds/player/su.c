/* Do not remove the headers from this file! see /USAGE for more info. */

// _su.c   (switch user cmd)
// Rust@ZorkMUD
// Megaboz@ZorkMUD added help 5-5-94
// Beek added race support.
// LsD fixed race support to not have a security hole.

//: PLAYERCOMMAND
// USAGE
//
//   |  ``su``
//   |  ``su name``
//   |  ``su (race)name``
//
// This is the command to switch users.  su with no arguments will update
// your character.  su with a name will polymorph you into that character,
// assuming you have the password. Instead of a name, if you supply a race in (), you will polymorph
// into that race.
//
// .. TAGS: RST

#include <daemons.h>

inherit CMD;

private
void main(string arg)
{
   string name, race;
   if (!arg)
   {
      name = this_user()->query_selected_body();
      race = 0;
   }
   else if (sscanf(arg, "(%s)%s", race, name) == 2)
   {
      if (name == "")
         name = this_user()->query_selected_body();
      name = trim(name);
   }
   else
   {
      name = arg;
      race = 0;
   }
   if (race)
   {
      if (!wizardp(this_user()))
         {
            out("Players cannot change race like this, sorry.");
            return;
         }
      race = DIR_RACES + "/" + depath(evaluate_path(race));
      if (race[ < 2..] != ".c")
         race += ".c";
      if (!is_file(race))
      {
         if (wizardp(this_user()))
            race = replace_string(race, "/race/", "/race/restricted/");
         if (!is_file(race))
         {
            out("No such race.\n");
            return;
         }
      }
   }

   if (this_body())
   {
      this_body()->save_me();
      this_body()->save_autoload();
   }
   this_user()->switch_user(name, race);
}