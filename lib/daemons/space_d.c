/* Do not remove the headers from this file! see /USAGE for more info. */

//: MODULE
// Daemon for starsystem layout i 3D space.
// This daemon can generate and layout distances between starsystems, with a
// 7 light year average.

#define MAX_STARSYSTEMS 50
#define SYSTEM_SPREAD 3.0
#define MINIMUM_PLANETS 3
#define LAYER_DENSITY 5
#define EARTHS_WEIGHT 5.97
#define EARTHS_RADIUS 6378
#define PRIV_NEEDED "Mudlib:daemons"

inherit M_DAEMON_DATA;
inherit SPACE_CLASSES;

/* Saved variables*/
private
mapping starsystems = ([]);
private
mapping player_starsystem_names = ([]);
private
mapping player_planet_names = ([]);

/* No saving*/
private
nosave string *greek_alphabet = // Omega skipped since reserved
    ({"Alpha", "Beta", "Gamma",   "Delta", "Epsilon", "Zeta",  "Eta", "Theta",   "Iota", "Kappa", "Lambda", "Mu",
      "Nu",    "Xi",   "Omicron", "Pi",    "Rho",     "Sigma", "Tau", "Upsilon", "Phi",  "Chi",   "Psi"});

private
nosave mapping planet_types =
    (["Neptunian Planet":10,
               "Ice Planet":10, "Asteroid Belt":15, "Rock Planet":30, "Gas Giant":6, "Terrestial Planet":20]);

private // If you add a lot of starsystems, you may want to extend this array to allow more unique combinations.
nosave string *star_last_names = ({"", "", "Majoris", "A", "B", "Minor", "C", "D", "Aa", "Ab", "Bb", "Cc", "Cd"});

private // Small blackboard mapping for tracking temporary things for uniqueness.
nosave mapping temp_map = ([]);

// Return densite in kg/m3
// https://nssdc.gsfc.nasa.gov/planetary/factsheet/ (Losely)
private
int planet_density(string type)
{
   int dens;
   switch (type)
   {
   case "Neptunian Planet":
      dens = 1638;
      break;
   case "Ice Planet":
      dens = 1270;
      break;
   case "Asteroid Belt":
      dens = 125;
      break;
   case "Rock Planet":
      dens = 5243;
      break;
   case "Gas Giant":
      dens = 687;
      break;
   case "Terrestial Planet":
      dens = 5514;
      break;
   default:
      dens = 3934;
      break;
   }
   return dens + random(100);
}

class planet random_planet(string planet_name)
{
   class planet p = new (class planet);
   p->type = element_of_weighted(planet_types);
   p->name = planet_name;
   p->radius = 6214 + random(80000);
   p->mass = ((p->type == "Asteroid Belt" ? 0.25 : 4.0) / 3.0) * 3.14159265359 * pow(p->radius / 1000.0, 3) *
             (planet_density(p->type) / 1000000.0);
   p->surface = (p->type == "Asteroid Belt" ? 0.0 : 4.0 * 3.14159265359 * pow(p->radius, 2) / 1000000.0);
   p->gravity = (p->mass / EARTHS_WEIGHT) / pow((p->radius / EARTHS_RADIUS), 2);
   p->moons = random(to_int(p->mass / 100));
   switch (p->type)
   {
   case "Neptunian Planet":
      p->col = "<123>";
      break;
   case "Ice Planet":
      p->col = "<015>";
      break;
   case "Asteroid Belt":
      p->col = "<245>";
      break;
   case "Rock Planet":
      p->col = "<241>";
      break;
   case "Gas Giant":
      p->col = "<216>";
      break;
   case "Terrestial Planet":
      p->col = "<032>";
      break;
   default:
      p->col = "<252>";
      break;
   }
   return p;
}

class planet *query_planets(string ssn)
{
   class planet *planets = ({});
   class starsystem ss;
   int i = 0;
   // We have planets already.
   if (!starsystems[ssn])
      return 0;
   ss = starsystems[ssn];
   if (arrayp(ss->planets))
      return ss->planets;

   while (i < ss->planets_count)
   {
      planets += ({random_planet(ss->name + "-" + (i + 1))});
      i++;
   }
   starsystems[ssn]->planets = planets;
   save_me();
   return starsystems[ssn]->planets;
}

string generate_star_name()
{
   return trim(choice(greek_alphabet) + " " + choice(greek_alphabet) + " " + choice(star_last_names));
}

class star random_star()
{
   class star st = new (class star);
   int type = random(100);

   switch (type)
   {
   case 0..5: // Actually only 0.000003% chance
      st->spectral_type = "O";
      st->colour = "Blue-violet";
      st->col = "<069>";
      st->temperature_min = 30000;
      st->temperature_max = 1000000;
      break;
   case 6..12: // Actually only 0.13% chance
      st->spectral_type = "B";
      st->colour = "Blue-white";
      st->col = "<081>";
      st->temperature_min = 10000;
      st->temperature_max = 30000;
      break;
   case 13..22: // Actually 0.6% chance
      st->spectral_type = "A";
      st->colour = "White";
      st->col = "<015>";
      st->temperature_min = 7500;
      st->temperature_max = 10000;
      break;
   case 23..38: // %3 chance
      st->spectral_type = "F";
      st->colour = "Yellow-white";
      st->col = "<230>";
      st->temperature_min = 6000;
      st->temperature_max = 7500;
      break;
   case 39..56: // 7.6% chance of these
      st->spectral_type = "G";
      st->colour = "Yellow";
      st->col = "<227>";
      st->temperature_min = 5000;
      st->temperature_max = 6000;
      break;
   case 57..75: // 12.1% chance of these
      st->spectral_type = "K";
      st->colour = "Orange";
      st->col = "<214>";
      st->temperature_min = 3500;
      st->temperature_max = 5000;
      break;
   default: // About 76.5% chance of these
      st->spectral_type = "M";
      st->colour = "Red-orange";
      st->col = "<202>";
      st->temperature_min = 1000;
      st->temperature_max = 3500;
      break;
   }
   st->name = generate_star_name();
   return st;
}

string generate_starsystem_name()
{
   int randchar;
   string name = "";
   name += "" + chr(random(25) + 65) + chr(random(25) + 65) + chr(random(25) + 65);
   name = name + "-" + sprintf("%4.4d", random(9999));
   if (temp_map[name])
      return generate_starsystem_name();

   temp_map[name] = 1;
   return name;
}

mapping query_starsystems()
{
   return starsystems;
}

mapping query_ss_alt_names()
{
   return player_starsystem_names;
}

mapping query_planet_alt_names()
{
   return player_planet_names;
}

string starsystem_name(class starsystem ss)
{
   string s = ss->name;
   return player_starsystem_names[s] ? player_starsystem_names[s] + " [" + s + "]" : s;
}

int set_ss_alt_name(string s, string aname)
{
   if (player_starsystem_names[aname])
      return 0;
   if (player_starsystem_names[s])
      map_delete(player_starsystem_names, player_starsystem_names[s]);
   starsystems[s]->named_by_player = aname;
   player_starsystem_names[s] = aname;
   player_starsystem_names[aname] = s;
   save_me();
   return 1;
}

int set_planet_alt_name(string s, string p, string aname)
{
   class starsystem ss;
   class planet *planets;
   int i = 0;
   if (strlen(aname) > 25 || strlen(aname)<5)
      error("Planet alt name must be between 5-25 letters.");

   if (player_planet_names[aname])
      return 0;
   if (player_planet_names[p])
      map_delete(player_planet_names, player_planet_names[p]);
   ss = starsystems[s];
   planets = ss->planets;
   while (i < sizeof(planets))
   {
      if (planets[i]->name == p)
         planets[i]->player_name = aname;
      i++;
   }
   player_starsystem_names[s] = aname;
   player_starsystem_names[aname] = s;
   save_me();
   return 1;
}

varargs void generate_starsystems()
{
   class starsystem g;
   int ss_count;
   int layer = 0;
   int planets_count = random(5) + random(5) + random(5) - layer;
   if (planets_count < 0)
      planets_count = 0;

   if (!check_privilege(PRIV_NEEDED))
      error("illegal attempt to generate_starsystems()\n");

   if (!sizeof(keys(starsystems)))
   {
      g = new (class starsystem);
      g->name = "Omega";
      g->named_by_player = "Omega";
      g->coordinates = ({0, 0, 0});
      g->star = random_star();
      g->planets_count = MINIMUM_PLANETS + planets_count;
      starsystems[g->name] = g;
      ss_count++;
      layer++;
   }

   layer += sizeof(keys(starsystems)) / LAYER_DENSITY;
   ss_count = sizeof(keys(starsystems));

   while (ss_count < MAX_STARSYSTEMS)
   {
      planets_count = random(5) + random(5) + random(5) - layer;
      if (planets_count < 0)
         planets_count = 0;
      g = new (class starsystem);
      g->coordinates = ({(((random(1000) / 1000.0) * (SYSTEM_SPREAD * 2)) - (SYSTEM_SPREAD * layer)),
                         (((random(1000) / 1000.0) * (SYSTEM_SPREAD * 2)) - (SYSTEM_SPREAD * layer)),
                         (((random(1000) / 1000.0) * (SYSTEM_SPREAD * 2)) - (SYSTEM_SPREAD * layer))});
      g->name = generate_starsystem_name();
      g->star = random_star();
      g->planets_count = MINIMUM_PLANETS + planets_count;

      starsystems[g->name] = g;
      ss_count++;
      if (ss_count % LAYER_DENSITY == 0)
         layer++;
   }
   temp_map = ([]);
   save_me();
}

string coord_str(int *coords)
{
   return "<228>[" + to_int(coords[0]) + "," + to_int(coords[1]) + "," + to_int(coords[2]) + "]<res>";
}

// Calculate the distance between two starsystems given by name.
int distance(string p1, string p2)
{
   string s1 = player_starsystem_names[p1];
   string s2 = player_starsystem_names[p2];
   int *c1 = starsystems[p1] ? starsystems[p1]->coordinates : starsystems[s1]->coordinates;
   int *c2 = starsystems[p2] ? starsystems[p2]->coordinates : starsystems[s2]->coordinates;
   return sqrt(pow((c2[0] - c1[0]), 2) + pow((c2[1] - c1[1]), 2) + pow((c2[2] - c1[2]), 2));
}

void near(string p1, int lyears)
{
   class starsystem s = starsystems[p1] || starsystems[player_starsystem_names[p1]];
   foreach (string name, class starsystem ss in starsystems)
   {
      float dist = distance(s->name, ss->name);
      if (ss == s || dist > lyears)
         continue;
      printf("   Starsystem: <118>%-25.25s<res>@ %s is %s light years away\n", starsystem_name(ss),
             coord_str(ss->coordinates), pround(dist, 2));
      printf("   Planet Cnt: %-25.25sPlanets generated: %s", "" + ss->planets_count,
             (arrayp(ss->planets) ? "Yes" : "Not yet"));
      printf("         Star: <118>%-25.25s<res>Spectral Type: %-10.10s Kelvin: %s <res>\n\n", ss->star->name,
             ss->star->spectral_type, ss->star->col + ss->star->temperature_min + "-" + ss->star->temperature_max);
   }
}

class starsystem query_starsystem(string ssn)
{
   class starsystem ss = starsystems[ssn] || starsystems[player_starsystem_names[ssn]];
   return ss;
}

void ss_stat(string ssn)
{
   class starsystem ss = starsystems[ssn] || starsystems[player_starsystem_names[ssn]];
   class planet *planets;
   write("\n\n<bld>--Starsystem statistics------------------------------------------------------------<res>\n\n");
   printf("   Starsystem: %-25.25sPlanet Cnt: %d", ss->name, ss->planets_count);
   printf("         Star: <118>%-25.25s<res>Spectral Type: %-10.10s Kelvin: %s <res>\n\n", ss->star->name,
          ss->star->spectral_type, ss->star->col + ss->star->temperature_min + "-" + ss->star->temperature_max);
   planets = query_planets(ss->name);
   foreach (class planet p in planets)
   {
      printf("  Planet Name: %-25.25s<res>        Type: %-20.20s Gravity: %-20.20s", p->col + p->name, p->type,
             pround(p->gravity, 2) + " G");
      printf("    Radius in: %-25.25sSurface: %-20.20s    Mass: %-20.20s\n", pround(p->radius, 2) + " km",
             pround(p->surface, 2) + " km2", pround(p->mass, 2) + " SPT kg");
      if (p->moons)
         printf("        Moons: %-25.25s", "" + p->moons);
      write("\n\n");
   }
   write("\n  (1 SPT kg = 10^24 kg)");
   write("\n\n<bld>--Other systems within 5 light years-----------------------------------------------<res>\n\n");
   near(ss->name, 5);
}

void create()
{
   ::create();
   set_privilege("Mudlib:daemons");
   if (!sizeof(keys(starsystems)))
      generate_starsystems();
}