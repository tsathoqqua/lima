/*
  
(Date unknown) - Jezu@Astaria
Added error_handler() during porting to FluffOS

July 10, 2022 - Jezu@Astaria
- Refactored error_handler().
- Fixed bug in error_handler() that was incorrectly applying verbose_errors.
- Changed debug class for errors generated by wizards to be ':<wiz name>'.  This is so wizzes can selectively enable which wizards they want to receive errors for.

July 03, 2023 - Jezu@SpaceMUD
- Refactored for LIMA

*/
#include <log.h>

#define DEBUG_BASE_CLASS     "debug"
#define DEBUG_CLASS(x)       DEBUG_BASE_CLASS + "/" + x
#define BORDER_CHARACTER     "-"
#define BORDER_WIDTH         60

#define HIW                  "%^BOLD%^%^WHITE%^"
#define HIR                  "%^BOLD%^%^RED%^"
#define NOR                  "%^RESET%^"

#define OUTPUT ([ \
  "file" : 0,\
  "line" : 1,\
  "program" : 2,\
  "trace" : 3,\
  "error" : 4,\
  "arguments" : 5,\
  "object" : 6,\
  "interactive" : 7,\
  "context" : 8,\
  "function" : 9,\
  "locals" : 10 \
])

class Error {
  string file;
  int line;
  string program;
  mixed *trace;
  string error;
  mixed *args;
  object ob;
  mixed interactive;
  string uid;
  string func;
  mixed *locals;
}

class Output {
  string file;
  string line;
  string program;
  string trace;
  string error;
  string arguments;
  string ob;
  string interactive;
  string context;
  string func;
  string locals;
}


protected string log_current_error(mapping err_map, int caught);

private nosave class Error error_data;
private nosave class Output output_data;

private nosave mapping errors = ([]);
private nosave int has_error = 0;
private nosave string last_error = "";


public string clear_last_error() {
  last_error = "";
}

public string get_last_error() {
  return last_error;
}

protected void error_handler(mapping error_map, int caught) {
  string str, debug_class, border, output, t;
  function f;
  mapping enabled_categories;
  
  string logfile = (caught ? LOG_FILE_CATCH : LOG_FILE_RUNTIME);
  string what = error_map["error"];
  string userid;
  
  //tell(find_user("jezu"), sprintf("%O", error_map));
  output_data = new(class Output);
  error_data = new(class Error);

  error_data.file        = (string)error_map["file"];
  error_data.line        = (int)error_map["line"];
  error_data.ob          = (object)error_map["object"];
  error_data.program     = (string)error_map["program"];
  error_data.trace       = (mixed*)error_map["trace"];
  if ( arrayp(error_data.trace) && sizeof(error_data.trace) && mapp(error_data.trace[<1]) )
  {
    error_data.func        = error_data.trace[<1]["function"];
    error_data.args        = error_data.trace[<1]["arguments"];
    error_data.locals      = error_data.trace[<1]["locals"];
  }
  error_data.error       = (string)error_map["error"];
  error_data.interactive = this_interactive() || this_player() || this_user();
  
  border = repeat_string(BORDER_CHARACTER, BORDER_WIDTH);
  
  str = sprintf("\n%s\n%s\n\n", border, ctime());
  
  if (caught)
  {
    str += "*Error caught\n\n";
  }
  f = function(string s, mixed m){
    if ( !stringp(m) )
      m = identify(m);
    return sprintf("\n%s%14s:%s %s", HIW, s, NOR, m);
  };

  str += (*f)("Error", error_data.error[0..-2]);
    
  output_data.ob = (*f)("Object", error_data.ob);
  output_data.program = (*f)("Program", error_data.program);
  output_data.func = (*f)("Function", error_data.func);
  output_data.arguments = (*f)("Arguments", error_data.args);
  output_data.file = (*f)("File", error_data.file);
  output_data.line = (*f)("Line", error_data.line);
 
  output_data.trace = sprintf("\n\n\n%sTrace:%s\n\n%s", HIW, NOR,
      implode( 
        map_array( 
          error_data.trace,
          (: sprintf("Line: %O File: %O Function: %O Object: %O Program: %O", 
               (int)$1["line"], 
               (string)$1["file"], 
               (string)$1["function"], 
               (object)$1["object"] || "No object", 
               (string)$1["program"] || "No program") :)
        ), "\n"));
        
  last_error = str;

  // if ( objectp(error_data.interactive) && wizardp(error_data.interactive) )
  // {
    // debug_class = DEBUG_CLASS(":"+error_data.uid);
    // write_file( sprintf("%sdebug.log", user_path(error_data.uid)), log_current_error(error_map, 1));

  // }  
  // else
  // {
    debug_class = DEBUG_CLASS("master");
    write_file(get_config(__DEBUG_LOG_FILE__), log_current_error(error_map, 0));	 

  // }
  f = function(int offset, int count) {
    string lines;
    string s = "";

    if ( !error_data.file || !sizeof(stat(error_data.file)) )  
      return s;

    lines = read_file( error_data.file, error_data.line+offset, count);
    if ( !stringp(lines) )
      return s;

    if ( !offset )
      s = sprintf("%s%d:%s  %s", HIR, error_data.line, NOR, lines);
    else
      foreach (string line in explode(lines, "\n"))
        s += sprintf("%d:  %s\n", error_data.line+offset++, line);

    return s; 
  };
  
  output_data.context  = sprintf("\n\n\n%sContext:%s\n\n%s%s%s", 
      HIW, 
      NOR,
      (*f)(-3, 3),
      (*f)(0, 1),
      (*f)(1, 3));


  // foreach(object wiz in filter_array(users(), (: wizardp :)))
  // {
    output = "";
    // enabled_categories = (mapping)wiz->query("debug_categories") || ([]);
    
    // foreach(string cat in (keys(enabled_categories)-({ "context", "trace" })))
    foreach(string cat in (keys(OUTPUT)-({ "context", "trace" })))
    {
      // if ( !(int)enabled_categories[cat] )
        // continue;
      
      t = fetch_class_member(output_data, OUTPUT[cat]);
      if ( stringp(t) )
        output += t;
    }
    
    // if ( (int)enabled_categories["trace"] )
      output += fetch_class_member(output_data, OUTPUT["trace"]);
    // if ( (int)enabled_categories["context"] )
      output += fetch_class_member(output_data, OUTPUT["context"]);
    
    // message(debug_class, sprintf("%s%s\n%s\n", str, output, border), wiz);
  // }
  

   
   if (this_user())
   {
      userid = this_user()->query_userid();
      if (!userid || userid == "")
         userid = "(none)";
      printf("%sTrace written to %s\n", what, logfile);
      errors[userid] = error_map;
   }
   else
      userid = "(none)";
   errors["last"] = error_map;
   
  tell(filter_array(users(), (: wizardp :)), sprintf("%s%s\n%s\n", str, output, border));
}

protected string log_current_error(mapping err_map, int caught)
{
  object ob;
  string prog, error_data, func, output, file;
  int line;
  
  file = file_name(err_map["object"]);
  line = err_map["line"];
  prog = err_map["program"];
  func = err_map["function"];
  if (caught)
  {
    error_data = replace_string(err_map["error"], "\n", " ");
    return sprintf("[%s] %s line %d: %s\n", ctime(time()), prog, line, error_data);
  }
  error_data = err_map["error"];
  output  = sprintf("%s\n", ctime(time()));
  output += error_data;
  output += sprintf("program: %s, object: %s line %d\n", prog, file, line);
  
 
 
  foreach (mapping new_map in err_map["trace"])
  {
    file = file_name(new_map["object"]);
    line = new_map["line"];
    prog = new_map["program"];
    func = new_map["function"];
    
    output += sprintf("'%15s' in ' %s' ('%s')line %d\n", func, prog, file, line);
  }
  return output;
}


mapping query_error(string name)
{
   /* This MUST be secure */
   if (!check_privilege(1))
      return 0;
   return errors[name];
}
