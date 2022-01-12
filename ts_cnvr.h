#pragma once
#include <sstream>
#include <fstream>
#include <filesystem>
#include <string>
#include <regex>
#include <chrono>
#include <vector>

/* STATIC FUNCTIONS */

template<typename _Elem = char, typename final_dur, typename dur>
_NODISCARD static inline constexpr final_dur func(std::basic_stringstream<_Elem>& ss) noexcept
{
    auto num = dur().count();
    if (!((ss) >> num))
    {
        (ss).clear();
        return dur();
    }
    (ss).clear();
    return final_dur(dur(num));
}

template<typename _Elem = char, typename final_dur, typename duration, typename duration2, typename... the_rest>
static inline constexpr final_dur func(std::basic_stringstream<_Elem>& ss) noexcept
{
    return func<_Elem, final_dur, duration>(ss) + func<_Elem, final_dur, duration2, the_rest...>(ss);
}

namespace ts_cnvr 
{
    namespace sfs = std::filesystem;

    /* TS CNVR */

    template<typename _Elem = char, typename final_dur, typename duration, typename ...other_dur>
    _NODISCARD constexpr final_dur ts_cnvr_dur(const std::basic_string<_Elem>& str) noexcept
    {
        std::basic_stringstream<_Elem> ss(str);
        // to do flip template parameters
        final_dur temp_duration(func<final_dur, duration, other_dur...>(ss));
        ss.clear();
        return temp_duration;
    }

    template<typename _Elem = char, typename final_dur, typename duration, typename ...other_dur>
    void ts_cnvr_str(std::basic_string<_Elem>& str) noexcept
    {
        std::basic_stringstream<_Elem> ss(str);
        // to do flip template parameters
        final_dur temp_duration(func<_Elem, final_dur, duration, other_dur...>(ss));
        ss.str(std::basic_string<_Elem>());
        ss.clear();
        ss << temp_duration.count();
        str.clear();
        ss >> str;
    }

    /* REGEX E STR EDIT */

    template<typename _Elem = char>
    void str_keep(std::basic_string<_Elem>& str, const std::basic_string<_Elem>& safe) noexcept
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

    template<typename _Elem = char>
    void regex_edit(std::basic_string<_Elem>& str, const std::basic_string<_Elem>& rgx_str,
        void(*edit_lambda)(std::basic_string<_Elem>& str)) noexcept
    {
        const std::basic_regex<_Elem> rgx(rgx_str);
        if (!str.empty() && !rgx._Empty())
        {
            typename std::basic_string<_Elem>::iterator match_it = str.begin();
            std::match_results<std::basic_string<_Elem>::iterator> rgx_matchs;
            std::basic_string<_Elem> sub_str;
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

    template<typename _Elem = char>
    void regex_edit(std::basic_string<_Elem>& str, const std::basic_regex<_Elem>& rgx,
        void(*edit_lambda)(std::basic_string<_Elem>& str)) noexcept
    {
        if (!str.empty() && !rgx._Empty())
        {
            typename std::basic_string<_Elem>::iterator match_it = str.begin();
            std::match_results<std::basic_string<_Elem>::iterator> rgx_matchs;
            std::basic_string<_Elem> sub_str;
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

    template<typename _Elem = char>
    _NODISCARD constexpr std::vector<std::basic_string<_Elem>> regex_results_vec(std::basic_string<_Elem>& str,
        const std::basic_regex<_Elem>& rgx, const size_t& reverve = 10) noexcept
    {
        std::vector<std::basic_string<_Elem>> matches;
        if (!str.empty() && !rgx._Empty())
        {
            matches.reserve(reverve);
            typename std::basic_string<_Elem>::iterator match_it = str.begin();
            std::match_results<std::basic_string<_Elem>::iterator> rgx_matchs;
            while (std::regex_search(match_it, str.end(), rgx_matchs, rgx))
            {
                matches.emplace_back(rgx_matchs.str());
                match_it = get<1>((rgx_matchs.prefix())) + rgx_matchs.str().size();
            }
        }
        return matches;
    }

    template<typename _Elem = char>
    _NODISCARD constexpr std::vector<std::basic_string<_Elem>> regex_results_vec(std::basic_string<_Elem>& str,
        const std::basic_string<_Elem>& rgx_str, const size_t& reverve = 10) noexcept
    {
        const std::basic_regex<_Elem> rgx(rgx_str);
        std::vector<std::basic_string<_Elem>> matches;
        if (!str.empty() && !rgx._Empty())
        {
            matches.reserve(reverve);
            typename std::basic_string<_Elem>::iterator match_it = str.begin();
            std::match_results<std::basic_string<_Elem>::iterator> rgx_matchs;
            while (std::regex_search(match_it, str.end(), rgx_matchs, rgx))
            {
                matches.emplace_back(rgx_matchs.str());
                match_it = get<1>((rgx_matchs.prefix())) + rgx_matchs.str().size();
            }
        }
        return matches;
    }

    /* INPUT E OUTPUT FILE */

    template<typename _Elem = char>
    _NODISCARD std::basic_string<_Elem> input_file(const sfs::path& file_path) noexcept
    {
        std::basic_string<_Elem> file;
        std::basic_ifstream<_Elem> fs;
        _Elem caracter = 0;
        std::streampos end;
        fs.open(file_path);
        if (fs.is_open())
        {
            // alloc
            file.reserve(fs.gcount());
            fs.seekg(0, std::ios_base::end);
            end = fs.tellg();
            fs.seekg(0);
            fs.clear();
            while (fs.tellg() != end)
            {
                //fs >> caracter
                if (caracter = fs.get())
                {
                    file = file + caracter;
                }
                else
                {
                    exit(1);
                }
                caracter = 0;
            }
        }
        else
        {
            exit(1);
        }
        fs.close();
        fs.clear();
        return file;
    }

    template<typename _Elem = char>
    void output_file(const sfs::path& file_path, const std::basic_string<_Elem>& str) noexcept
    {
        std::basic_ofstream<_Elem> fs;
        fs.open(file_path);
        if (fs.is_open())
        {
            fs.seekp(0, std::ios_base::end);
            fs.clear();
            if (!(fs << str))
            {
                exit(1);
            }
        }
        else
        {
            exit(1);
        }
        fs.close();
        fs.clear();
    }

}
