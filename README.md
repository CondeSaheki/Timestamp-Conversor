# Timestamp Conversor

This is a simple library written in C++ that allows you to convert any time stamp string into an ```std::chrono::duration``` object and it also have function based in ```std::basic_regex``` for you to find and edit them easily.

# Installation
Just add ```ts_cnvr.h``` in your project folder and include ```#include "ts_cnvr.h"```.

# Features and Usage

1 - Convert any time stamp:
```
//final_dur obj = ts_cnvr_dur<final_dur, ...other_dur>(const mystring&);
//convert_str_ts_in_str_duration<final_dur, ...other_dur>(const mystring&);

mystring example_str = "01 01 01"; // minutes seconds milliseconds

sch::milliseconds chrono_obj = app::convert_str_ts_in_duration <sch::milliseconds, sch::milliseconds, sch::seconds, sch::minutes>(example_str);

// or

app::convert_str_ts_in_str_duration <sch::milliseconds, sch::milliseconds, sch::seconds, sch::minutes>(example_str);

// now "example_str" contains the converted timestamp
```
Notes:
- The input string should contains only caracters convertible to the respective duration type in order
- First template argument is the target duration always
- The other template argument are the timestamp format but in reverse order

2 - Regex to find and edit:
```
//regex__edit(mystring& str, const mystring& rgx_str, void (*edit_lambda)(mystring& str));

mystring file = L"..."
app::regex_edit(file, L"\"?[[:d:]]{2,}\:[[:d:]][[:d:]]\:[[:d:]][[:d:]]{2,}\"?",
   [](mystring& str)
   {
      ts_cnvr::format_str(str);
      ts_cnvr::convert_str_ts_in_str_duration <sch::milliseconds, sch::milliseconds, sch::seconds, sch::minutes>(str);
   });
```

3 - Load file:
```
//_nodiscard mystring input_file(const sfs::path& file_path) noexcept

mystring file = input_file("...");
```

4 - Save file:
```
//void output_file(const sfs::path& file_path, const mystring& str);

mystring file = L"...";
ts_cnvr::output_file(L"...", file);
```
