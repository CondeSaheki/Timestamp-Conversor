#pragma once
#include "my.h"

// static functions
template<typename final_dur, typename dur> 
    _nodiscard static inline constexpr final_dur func(mystringstream& ss) noexcept
{
    auto num = dur().count();
    if (!((ss) >> num))
    {
        (ss).clear();
        my::log::error("error msg");
        return dur();
    }
    (ss).clear();
    return final_dur(dur(num));
}
template<typename final_dur, typename duration, typename duration2, typename... the_rest>
    static inline constexpr final_dur func(mystringstream& ss) noexcept
{
    return func<final_dur, duration>(ss) + func<final_dur, duration2, the_rest...>(ss);
}

namespace app
{
    template<typename final_dur, typename duration, typename ...other_dur> 
        _nodiscard constexpr final_dur convert_str_ts_in_duration(const mystring& str) noexcept
    {
        mystringstream ss(str);
        // flip template parameters
        final_dur temp_duration(func<final_dur, duration, other_dur...>(ss));
        ss.clear();
        return temp_duration;
    }

    template<typename final_dur, typename duration, typename ...other_dur> 
        void convert_str_ts_in_str_duration(mystring& str) noexcept
    {
        mystringstream ss(str);
        // flip template parameters
        final_dur temp_duration(func<final_dur, duration, other_dur...>(ss));
        ss.str(mystring());
        ss.clear();
        ss << temp_duration.count();
        str.clear();
        ss >> str;
    }

    void format_str(mystring& str, const mystring& safe = L" 0123456789.,") noexcept
    {
        bool ok;
        for (auto it = str.begin(); it != str.end(); ++it)
        {
            ok = false;
            for (auto it2 = safe.begin(); it2 != safe.end(); ++it2)
            {
                if (*it == *it2)
                {
                    ok = true;
                    break;
                }
            }
            if (!ok) { *it = ' '; }
        }
    }

    void regex_replace_edited(mystring& str, const mystring& rgx_str, void (*edit_lambda)(mystring& str)) noexcept
    {
        const std::basic_regex<mychar> rgx(rgx_str);
        if (!str.empty() && !rgx._Empty())
        {
            mystring::iterator match_it = str.begin();
            std::match_results<mystring::iterator> rgx_matchs;
            mystring sub_str;
            while (std::regex_search(match_it, str.end(), rgx_matchs, rgx))
            {
                sub_str = rgx_matchs.str();
                edit_lambda(sub_str);
                match_it = get<1>((rgx_matchs.prefix())) + sub_str.size();
                str.replace(get<1>((rgx_matchs.prefix())), get<0>((rgx_matchs.suffix())), sub_str.begin(), sub_str.end());
                sub_str.clear();
            }
        }
    }
}
