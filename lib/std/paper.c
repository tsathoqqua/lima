/* Do not remove the headers from this file! see /USAGE for more info. */

// WIP

inherit OBJ;
inherit LIGHT_OBJECT;
inherit M_GETTABLE;
inherit M_READABLE;

int burnt, mangled;
float max_fuel;

void mudlib_setup()
{
   set_fuel(1);
   set_id("paper");
#ifdef USE_SIZE
   set_size(VERY_SMALL);
#endif
#ifdef USE_MASS
   set_mass(VERY_SMALL);
#endif
   set_light_level(0);
   set_die_msg("The $o crumbles into fine ash.");
   light_object::mudlib_setup();
}

string query_text()
{
   string r = ::query_text();
   if (mangled)
      return "It's somewhat burnt, but reads as follows:\n" + r;
   return r;
}

void mangle_text()
{
   string my_text = ::query_text();
   int text_size = sizeof(my_text);
   int i;
   mangled = 1;

   if (sizeof(my_text) > 0)
   {
      if (!burnt)
      {
         burnt = 1;
      }
      for (i = 0; i < text_size; i++)
         if (random(max_fuel) > query_num_decays())
            if (!(my_text[i] == ' ' && random(1)))
               my_text[i] = '?';
      set_text(my_text);
   }
}

mixed extinguish()
{
   mangle_text();
   return ::extinguish();
}

string read()
{
   if (is_lit() && !mangled)
      mangle_text();
   return ::read();
}

void set_fuel(int amount)
{
   if (!burnt)
      max_fuel = amount;
   ::set_fuel(amount);
}
