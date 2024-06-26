/* Do not remove the headers from this file! see /USAGE for more info. */

#include <daemons.h>
inherit CMD;

#define MIN_LEN 3

//: COMMAND
// Usage
//    ``apropos <string>``
//
// Returns information on which mudlib functions contain the
// keyword passed, including a short description.
//
// .. TAGS: RST

// Generate a category for a given file.
string generate_category(string file)
{
   string *st = explode(file, "/");
   return implode(st[1..sizeof(st)-2], "/");
}

mixed apropos(string s)
{
   mapping filer = ([]);
   mapping topics;
   string output = "";
   string pwd;

   topics = HELP_D->query_topics();
   if (!topics)
   {
      error("Apropos whines about help_d not giving any info out.\n");
      return;
   }
   foreach (string key, string * files in topics)
   {
      if (strsrch(key, s) != -1)
         foreach (string f in files)
         {
            pwd = generate_category(f);
            if (!arrayp(filer[pwd]))
               filer[pwd] = ({});
            filer[pwd] += ({key});
         }
   }
   foreach (string key in sort_array(keys(filer), 1))
   {
      output += iwrap("[" + key + "]:  " + implode(filer[key], ", ") + "\n");
   }
   output += "\n";
   return output;
}

private
void main(string s)
{
   string yt;
   if (!s)
   {
      write("apropos <string>\n\n"
            "Returns information on which mudlib functions contain the\n"
            "keyword passed, including a short description.\n");
      return;
   }
   if (strlen(s) < MIN_LEN)
   {
      write("Please find a bigger search criteria (min. " + MIN_LEN + " letters).\n");
      return;
   }
   yt = apropos(s);
   if (yt == "\n")
      write("No help files match your word.\n");
   out(yt);
}
