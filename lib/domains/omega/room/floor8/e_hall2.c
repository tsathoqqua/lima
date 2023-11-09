inherit INDOOR_ROOM;

void setup()
{
   set_area("omega_terminal", "landing_term");
   // set_weather(1);
   set_light(1);
   set_brief("East Hallway 2");
   set_long("A hallway leading to the East Landing. You see the Stellar Bites café just north of here.");
   set_state_description("electronic_door_off", "closed");
   set_state_description("electronic_door_on", "open");
   set_exits((["east":"e_hall1", "west":"e_hall3", "north":"e_cafe1"]));
   set_exit_msg("north", "$N $vpass through the automatic doors into Stellar Bites.");
   set_listen("You hear people running through the terminal. " +
              "It sounds like a busy terminal with people coming and going");
}