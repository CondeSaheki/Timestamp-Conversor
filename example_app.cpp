#include "ts_cnvr.h"

int main()
{
    namespace sch = std::chrono;
    using char_type = wchar_t;

    const std::basic_regex<char_type> ts_regex(L"\"?[[:d:]]{2,}\:[[:d:]][[:d:]]\:[[:d:]][[:d:]]{2,}\"?");
    
    auto file = ts_cnvr::input_file<char_type>(L"input.txt");
    ts_cnvr::regex_edit<char_type>(file, ts_regex, [](std::basic_string<char_type>& str)
        {
            ts_cnvr::str_keep<char_type>(str, L" 0123456789.");
            ts_cnvr::ts_cnvr_str<char_type, sch::milliseconds, sch::milliseconds, sch::seconds, sch::minutes>(str);
        }); 
    ts_cnvr::output_file<char_type>(L"output.txt", file);
}
