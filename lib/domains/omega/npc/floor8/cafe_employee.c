/* Do not remove the headers from this file! see /USAGE for more info. */

inherit LIVING;
inherit M_VENDOR;

mapping employee_names = (["girls":({"Emma",   "Olivia",   "Sophia", "Ava",     "Isabella", "Mia",       "Charlotte",
                                     "Amelia", "Harper",   "Evelyn", "Abigail", "Emily",    "Elizabeth", "Sofia",
                                     "Avery",  "Scarlett", "Lily",   "Grace",   "Victoria", "Stella"}),
                            "boys":({"Noah",      "Liam",    "William", "Benjamin", "James",       "Oliver",  "Henry",
                                     "Alexander", "Ethan",   "Michael", "Daniel",   "Samuel",      "Matthew", "David",
                                     "Joseph",    "Jackson", "Anthony", "Andrew",   "Christopher", "Gabriel"})]);

void setup()
{
   int gender = random(2);
   string name = choice(employee_names[gender ? "girls" : "boys"]);
   set_gender(gender);
   set_name(name);
   add_id("employee");
   set_proper_name(name);
   set_in_room_desc(name+" the Stellar Bites employee is smiling at you from behind the counter.");
   set_long(name+" wears that typically fake smile today.");

   /* Determines what type of currency we'll use */
   set_currency_type("credit");

   /* Tells vendor we will sell things */
   set_for_sale(1);

   /* Tells vendor we will buy things */
   set_will_buy(1);

   /* itemization of what we sell initially
      filename : #
      if # is set to -1, it will sell an infinite # of that item
   */
   set_sell((["^std/apple":-1, "^std/weapon/sword":3, "^std/ale":-1, ]));

   /* We would set this so that the vendor stores weaps/armour in a storage
      room.  See help on set_unique_inventory.  storage.c in the rooms subdir has
      been set up to work.
       set_unique_inventory(__DIR__ "rooms/storage");
   */

   /* We would set this if we wanted EVERYTHING to be stored in a storage
      room.
       set_all_unique(1);
   */
}