# Timestamp Conversor

This is a simple library written in C++ that convert any **timestamp string** into an ```std::chrono::duration``` object that also has tools for finding and manipulating them easily using **regex**, ```std::basic_regex```.

# Installation
Just add ```ts_cnvr.h``` in your project folder and include it ```#include "ts_cnvr.h"```.

# Features and Usage

**1 - Conversors:**

Timestamp string to ```std::chrono::duration``` object
```
duration = ts_cnvr_dur<char_type, final_duration, ...other_durations>(string);
```
Change the Timestamp string format to other
```
ts_cnvr_str<char_type, final_duration, ...other_durations>(string);
```

**Notes:**
- The input string should contains only caracters that are convertible to the respective duration
- First template parameter is the string element
- Second template parameter is the returned duration type or the type to convert in string
- The other template parameter are the timestamp durations **in reverse order (for now)** 

**2 - Regex and formating:**

Find and edit all timestamps
```
regex_edit<char_type>(file, regex, [](std::basic_string<char_type>& str) { /* code */});
regex_edit<char_type>(file, regex_str, [](std::basic_string<char_type>& str) { /* code */});
```
Return a vector containing only the timestamp string's
```
vector = regex_results_vec<char_type>(file, regex);
vector = regex_results_vec<char_type>(file, regex_str);
```
Simple formating that only keep the especified caracters
```
str_keep(string, string_of_caracters_to_keep);
```

**3 - Load and saving file:**

load a file
```
string file = input_file<char_type>("...");
```
save a file
```
output_file<char_type>(L"...", file);
```
