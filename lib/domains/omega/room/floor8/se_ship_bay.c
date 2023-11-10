inherit INDOOR_ROOM_DOCKING;

void setup()
{
   ::setup();
   set_area("omega_terminal", "bay_term");
   set_brief("Bay Terminal SE-1");
   set_long("A place where you can rent or buy bays to park your ship");
   set_exits((["northeast":"e_landing2", "southwest":"se_ship_bay1", ]));
}
