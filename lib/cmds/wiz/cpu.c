/* Do not remove the headers from this file! see /USAGE for more info. */

// Yaynu @ red dragon Nov. 1995

//: COMMAND
// USAGE ``cpu``
//
// Shows cpu load generated by the mud (as a %)
//
// .. TAGS: RST

inherit CMD;

void main()
{
   mapping info;
   float x, fraction;

   info = rusage();
   // time is in msec, convert to sec
   x = (info["utime"] + info["stime"]) / 1000;
   fraction = x / uptime();
   outf("CPU usage: %2.2f%%\n", fraction * 100);
}
