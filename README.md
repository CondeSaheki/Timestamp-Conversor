# Timestamp Conversor

This is a simple library written in C++ that allows you to convert any time stamp string into an ```std::chrono::duration``` object and it also have function based in ```std::basic_regex``` for you to find and edit them easily.

# Installation
Just add ```ts_cnvr.h``` in your project folder and include ```#include "ts_cnvr.h"```.

# Features and Usage

1 - Conversors:
```
ts_cnvr_str<char_type, final_duration, ...other_durations>(string);
duration = ts_cnvr_dur<char_type, final_duration, ...other_durations>(string);
```
Notes:
- The input string should contains only caracters convertible to the respective duration type in order
- First template argument is the string element always
- Second template argument is the target duration always
- The other template argument are the timestamp format but in reverse order (for now)

2 - Regex and formating:
```
regex_edit<char_type>(file, regex, [](std::basic_string<char_type>& str) { /* code */});
regex_edit<char_type>(file, regex_str, [](std::basic_string<char_type>& str) { /* code */});
```
```
vector = regex_results_vec<char_type>(file, regex);
vector = regex_results_vec<char_type>(file, regex_str);
```
```
str_keep(string, string_of_caracters_to_keep);
```

3 - Load and saving file:
```
string file = input_file<char_type>("...");
```
```
output_file<char_type>(L"...", file);
```
