/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** timer.c
**
** Simple interface to the timer daemon
**
** 04-Feb-95. Deathblade. Created.
*/

//: COMMAND
//$$ see: channels, chan
// USAGE:  ``timer [-r] number [string  number]``
//
// Starts a timer for some number of seconds (given by the first number).
// Only one timer is allowed per person (at the moment, simply due to
// implementation decisions).
//
// If -r is provided, the timer will repeat until stopped.
//
// If a string and number is given, then this is for displaying
// the timer notifications over a channel (rather than just to you).
// The number specifies how often a notice should be given.  The
// channel name should be the internal channel name or a player
// channel name (plyr_XXX provided by the "chan" command).
//
// The timer may be stopped by passing 0 for the timer (e.g. "timer 0").
//
// Examples:
//> ``timer 600 clock 60``	-- 10 minute timer, notify on "clock" channel
//			   every minute
//> ``timer -r 300``		-- repeating 5 minute timer; notifies you
//			   directly.
//> ``timer 30 wiz_wiz 5``	-- 30 second timer, notifying every 5 seconds
//			   on the wizard channel
//
// .. TAGS: RST

inherit CMD;

private
void main(mixed *args, mapping flags)
{
   string channel_name;

   channel_name = args[1];
   if (channel_name)
   {
      string user_name = CHANNEL_D->user_channel_name(channel_name);
      if (channel_name == user_name)
         channel_name = "plyr_" + channel_name;
   }

   out(TIMER_D->add_timer(args[0], flags["r"], channel_name, args[2]));
}
