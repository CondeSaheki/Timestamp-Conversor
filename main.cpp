#include "app.h"

int main()
{
    namespace sch = std::chrono;

    auto file = ts_cnvr::input_file("input.txt");
    ts_cnvr::regex_edit(file, "\"?[[:d:]]{2,}\:[[:d:]][[:d:]]\:[[:d:]][[:d:]]{2,}\"?",
        [](ts_cnvr::mystring& str)
        {
            ts_cnvr::str_keep(str, " 0123456789.");
            ts_cnvr::ts_cnvr_str<sch::milliseconds, sch::milliseconds, sch::seconds, sch::minutes>(str);
        }); 
    ts_cnvr::output_file(L"output.txt", file);
}
