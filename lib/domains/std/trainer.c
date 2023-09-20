inherit ADVERSARY;
inherit M_TRAINER;

void setup()
{
   int skill = 1000;

   set_name("Tara");
   set_gender(1);
   set_id("tara", "human", "trainer");
   set_proper_name("Tara");
   set_in_room_desc("Tara, a confident looking female fighter");
   set_long("Tara ...");
   set_skill("combat/defense/disarm", skill);
   set_skill("combat/defense/dodge", skill);
   set_skill("combat/melee", skill);
   set_skill("combat/melee/blade", skill);
   set_skill("combat/melee/club", skill);
   set_skill("combat/melee/improv", skill);
   set_skill("combat/melee/unarmed", skill);

   set_options((["hello":"Hi! Can you tell how skills work?",
             "potential":"What do you mean potential?", "trainpts":"How do I see how many training points I have?",
              "whatrank":"What are skill ranks?", "skillrank":"Why does YOUR skill rank matter to me?",
              "trainers":"How do I find trainers?"]));

   set_responses((
       ["hello":"Hello there! Everything you do on "+mud_name()+" trains your skills. Do a thing more and you gain more skills "
                "doing that. But I'm willing to train you further - if you have the potential?@@potential,whatrank",
           "potential":"You need to gather training points as part of practicing your 'skills'. If you have these, I "
                       "can train you.@@trainpts",
            "trainpts":"You use the \"skills\" command. The numbers at the end are your "
                       "training points for a certain skill. ",
            "whatrank":"All skills begin at no rank and then goes from rank 1 to 15. The skill ranks are always shown "
                       "as numbers in brackets, that is [1], [4] etc.@@skillrank",
           "skillrank":"I cannot train you above my own skill rank, so finding new trainer will become important to "
                       "you.@@trainers",
            "trainers":"You need to find trainers in the world that can train you in other skills than I, but also at "
                       "a higher rank. Best of luck!",
   ]));

   set_start(({"hello"}));
   setup_trainer_conversation(skill);
}

string query_hint()
{
   return "Tara is a skill trainer, use 'talk to tara' to learn about skills";
}