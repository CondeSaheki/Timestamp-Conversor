#include "app.h"

void example_app()
{
    my::log::benchmark bench_app(L"app start");
    my::log::info("loading file");
    mystring file = my::input_file(L"input.txt");
    bench_app.now();
    my::log::info("Regex edit");
    app::regex_replace_edited(file, L"\"?[[:d:]]{2,}\:[[:d:]][[:d:]]\:[[:d:]][[:d:]]{2,}\"?",
        [](mystring& str)
        {
            app::format_str(str);
            app::convert_str_ts_in_str_duration <sch::milliseconds, sch::milliseconds, sch::seconds, sch::minutes>(str);
        });
    bench_app.now();
    my::log::info("output file");
    my::output_file(L"output.txt", file);
    bench_app.now();
}

int main()
{
    example_app();
}
