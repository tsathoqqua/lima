/* Do not remove the headers from this file! see /USAGE for more info. */

/*
**	User menu by Tsath
**	For the Lima mudlib.
*/

#include <menu.h>
#include <mudlib.h>
#include <playerflags.h>
#include <commands.h>
#include <security.h>

inherit MENUS;
inherit M_ACCESS;

MENU toplevel;
// submenus of the toplevel (main) menu
MENU_ITEM quit_item;
MENU_ITEM goto_main_menu_item;
MENU_ITEM main_seperator;

// right now, we're just going to call the help command.
// private MENU helpmenu;

private
mapping dispatch = (["w":"who"]);
private
int gender;
private
string name;
private
string race;
private
string fname;

int name_available(string name)
{
    return !arrayp(LAST_LOGIN_D->query_last(name));
}

private
nomask void simple_cmd(string cmd)
{
    call_other(CMD_DIR_PLAYER "/" + dispatch[cmd], "player_menu_entry");
}

void enter_game()
{
    string selected = this_user()->query_selected_body();
    string fname = this_user()->query_body_fname(selected);
    if (!selected)
    {
        write("Select a character first.\n");
        return;
    }
    //modal_pop();
    this_user()->enter_game(selected,fname);
}

private
nomask int valid_name(string str)
{
    int len;
    str = lower_case(str);

    if (!name_available(str))
    {
        write("Sorry, that name is already taken.\n");
        write("('quit' will abort).\n");
        return 0;
    }

    if (BANISH_D->check_name(str))
    {
        write("Sorry, that name is forbidden by the implementors.  Please choose another.\n");
        write("('quit' will abort).\n");
        return 0;
    }

    if (BANISH_D->check_name(str))
    {
        write("Sorry, that name is forbidden by the implementors.  Please choose another.\n");
        write("('quit' will abort).\n");
        return 0;
    }

    len = strlen(str);

    if (len < 2)
    {
        write("Sorry, that name's too short.  Try again.\n> ");
        return 0;
    }

    if (len > 12)
    {
        write("Sorry, that name's too long.  Try again.\n");
        write("('quit' will abort).\n");
        return 0;
    }

    /*
    ** We used to rely on the banish code to do this, but that is a
    ** Bad Thing as it is too easy to see those entries deleted.  In
    ** particular, we've been distributing the lib for a while now
    ** without a banish.o that contains this rule.  To use the banish
    ** code to implement a feature here is too convoluted and unintuitive.
    **
    ** So... we'll put a regex here to check the name.
    **
    ** Note that this regex matches the restriction imposed by the
    ** SECURE_D.  Also note the name is in lower case right now.
    */
    if (!regexp(str, "^[a-z]+$"))
    {
        write("Sorry, that name is forbidden by the implementors.  Please\n"
              "choose a name containing only letters.\n");
        return 0;
    }

    return 1;
}

void got_entry(function when_done, string line)
{
    int width = 0;
    string format = "%#-75." + (75 / (width + 3)) + "s\n\n";
    mapping races = RACE_D->query_race_data();

    if (line == "list")
    {
        write("Please select a race from the following list:\n");
        printf(format, implode(keys(races), "\n"));
        return;
    }

    if (races[line])
    {
        modal_pop();
        race = line;
        fname = races[line];
        evaluate(when_done);
        return;
    }

    if (sscanf(line, "help %s", line) && races[line])
    {
        write(races[line]->short_description());
        return;
    }

    write("No such race.\n");
}

void do_select(string name)
{
    mapping bodies = this_user()->query_bodies();
    string *sorted_bodies = sort_array(keys(bodies), 1);
    int indexname = to_int(name) - 1;
    if (indexname >= 0)
    {
        name = sorted_bodies[indexname];
    }

    if (this_user()->set_selected_body(name))
        write("Selected character '" + capitalize(name) + "' for playing.\n");
    else
        write("Failed to select character '" + capitalize(name) + "' for playing.\n");
}

void select_char()
{
    mapping bodies = this_user()->query_bodies();
    string *sorted_bodies = sort_array(keys(bodies), 1);
    int body_count = sizeof(sorted_bodies);
    string selected = this_user()->query_selected_body();
    string *genders = ({"None", "Male", "Female", "Non-binary"});
    string format = "%-8.8s %-14.14s %-7.7s %-25.25s %-20.20s\n";
    int count = 1;
    TBUG(bodies);
    printf(format, "Select", "Name", "Level", "Race", "Gender");
    write("--------------------------------------------------------------------------\n");
    foreach (string name in sorted_bodies)
    {
        mixed *ar = bodies[name];
        string s = "[" + count + (name == selected ? "*" : "") + "]";
        printf(format, s, capitalize(name), "" + ar[1], capitalize(ar[2]), genders[ar[3]]);
        count++;
    }
    write("--------------------------------------------------------------------------\n" +
          "(* Selected character)\n");

    // We only have one body, select it.
    switch (body_count)
    {
    case 1:
        do_select(sorted_bodies[0]);
        break;
    case 0:
        write("Create a character to select one.\n");
        break;
    default:
        input_one_arg("Name or [1-" + body_count + "]: ", (
                                                              : do_select:));
        break;
    }
}

private
void creation_done()
{
    TBUG("Name: " + name + " Gender: " + gender + " Fname: " + fname + " Race: " + race);
    this_user()->set_body(name, fname, race, gender);
}

private
void input_gender(int inputgender)
{
    string race = DIR_RACES "/human";
    string format;
    string *races;
    function when_done = (
        : creation_done:);
    int width = 0;
    int intconvert = to_int(inputgender);

    if (intconvert < 1 || intconvert > 3)
    {
        write("\nSorry? Try again, 1-3 only.\n");
        input_one_arg("Gender: ", (
                                      : input_gender:));
        return;
    }
    gender = intconvert;
#ifdef USE_RACES
    races = RACE_D->query_races();
    if (sizeof(races) == 1)
    {
        string default_race = races[0];
        write("You will be a " + default_race + ".\n");
        creation_done();
    }
    else
    {
        foreach (string racename in races)
        {
            if (strlen(racename) > width)
                width = strlen(racename);
        }

        format = "%#-75." + (75 / (width + 3)) + "s\n\n";

        write("\n\nPlease select a race from the following list:\n");
        printf(format, implode(races, "\n"));

        write("Type 'help race' for a brief description.  Type 'list' to show the choices again.\n");
        modal_push((
                       : got_entry, when_done:),
                   "Race? ");
    }
#endif /* USE_RACES */
}

private
void char_name(string inputname)
{
    if (!strlen(inputname))
    {
        write("\nName is not optional ('quit' will abort).\n");
        input_one_arg("Name: ", (
                                    : char_name:));
        return;
    }
    if (inputname == "quit")
    {
        return;
    }
    if (!valid_name(inputname))
    {
        input_one_arg("Name: ", (
                                    : char_name:));
        return;
    }
    name = inputname;
    write("Gender for " + capitalize(name) + "?\n\t1. Male\n\t2. Female\n\t3. Non-binary\n\n");
    input_one_arg("Gender: ", (
                                  : input_gender:));
}

void create_char()
{
    input_one_arg("Name: ", (
                                : char_name:));
}

void quit_game()
{
    this_user()->save_me();
    this_user()->quit();
}

void create()
{
    set_privilege(1);

    toplevel = new_menu(mud_name() + " User Menu");
    quit_item = new_menu_item("Quit", (
                                          : quit_game:),
                              "q");
    goto_main_menu_item = new_menu_item("Return to main menu", toplevel, "m");

    main_seperator = new_seperator("--------------------------------------------------");

    // Add items to the toplevel (main) menu.
    add_menu_item(toplevel, main_seperator);
    add_menu_item(toplevel, new_menu_item("See who's on (who)",
                                          (
                                              : simple_cmd:),
                                          "w"));
    add_menu_item(toplevel, new_menu_item("Select character to play", (
                                                                          : select_char:),
                                          "s"));
    add_menu_item(toplevel, new_menu_item("Create new character", (
                                                                      : create_char:),
                                          "c"));
    add_menu_item(toplevel, new_menu_item("Enter the game", (
                                                                : enter_game:),
                                          "p"));
    add_menu_item(toplevel, quit_item);
}

void start_menu()
{
    init_menu_application(toplevel);
}