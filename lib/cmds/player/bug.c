/* Do not remove the headers from this file! see /USAGE for more info. */

// _bug.c
// Rust wrote it
// Megaboz@ZorkMUD attached header 5-5-94
// Converted to use new /std/reporter superclass (Deathblade 4-Sep-94)
// Beek made one line reports work right 11/14/94
// Rust made them post news

//: PLAYERCOMMAND
//$$see: idea, typo, feedback, question
// USAGE
//     ``bug``
//
// This command directs a report of a game bug to the proper place.
//
// .. TAGS: RST

inherit CMD;

void create()
{
   ::create();
   no_redirection();
}

private
void main(string str)
{
   REPORTER_D->report_something("Bug", str);
}

void player_menu_entry()
{
   bare_init();
   main("");
   done_outputing();
}
