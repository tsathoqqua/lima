inherit INDOOR_ROOM_DOCKING;

void setup()
{
   ::setup();
   set_area("omega_terminal", "bay_term");
   // set_weather(1);
   set_brief("Bay Terminal NE-2");
   set_long(
       "This docking bay is a spacious room with smooth, white walls and a high ceiling. " +
       "At one end of the room, there is a docking terminal and an airlock on the wall provides a buffer between the "
       "vacuum of space and the breathable " +
       "atmosphere of the station, ensuring the safety of those who come and go from the ships. " +
       "One of the most striking features of the docking bay is a large panoramic window.");
   set_exits((["northwest":"ne_ship_bay", "southeast":"ne_ship_bay2", ]));
}
