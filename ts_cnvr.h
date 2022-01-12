#pragma once
#include <sstream>
#include <fstream>
#include <filesystem>
#include <string>
#include <regex>
#include <chrono>
#include <vector>

#ifndef _HAS_NODISCARD
#ifndef __has_cpp_attribute
#define _HAS_NODISCARD 0
#elif __has_cpp_attribute(nodiscard) >= 201603L
#define _HAS_NODISCARD 1
#else
#define _HAS_NODISCARD 0
#endif
#endif
#if _HAS_NODISCARD
#define _nodiscard [[nodiscard]]
#else
#define _nodiscard
#endif

/* STATIC FUNCTIONS */

template<typename _Elem = char, typename final_dur, typename dur>
_nodiscard static inline constexpr final_dur func(std::basic_stringstream<_Elem>& ss) noexcept
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
        _nodiscard constexpr final_dur ts_cnvr_dur(const std::basic_string<_Elem>& str) noexcept
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

    /* INPUT E OUTPUT FILE */

    template<typename _Elem = char>
        _nodiscard std::basic_string<_Elem> input_file(const sfs::path& file_path) noexcept
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

/* WORKING DRAFT */

#if 0
void bom() // endianes
{
#if 0
    constexpr char32_t native_bom = U'\U0000FEFF';

    myifstream is("...");
    char32_t bom;
    is >> bom;
    if (native_bom == bom)
    {
        process_stream(is);
    }
    else
    {
        mystringstream char_stream;
        //Load the rest of `is` and endian-convert it into `char_stream`.
        process_stream(char_stream);
    }
#endif
}

static std::ostream& operator<<(std::ostream& Os, const sfs::perms& perms)
{
    return Os << static_cast<int>(perms);
}

static bool valid_file(const sfs::path& path)
{
    if (sfs::exists(path) && sfs::is_regular_file(path))
    {
        if (sfs::is_regular_file(path))
        {
            // perms check


        }
        else
        {
            std::wcout << L"Error";
            exit(1);
        }
    }
    else
    {
        std::wcout << L"Error";
        exit(1);
    }
}

void perms()
{
    sfs::path path1(L"C:\\Users\\anonimo\\Desktop\\ARQUIVO.txt");
    sfs::path path2(L"C:\\Users\\anonimo\\Desktop\\ARQUIVO.txt");

    if (sfs::exists(path1))
    {
        std::wcout << L"arquivo 1 existe" << std::endl;
    }
    if (sfs::exists(path2))
    {
        std::wcout << L"arquivo 2 existe" << std::endl;
    }

    if (sfs::equivalent(path1, path2))
    {
        std::wcout << L"sao o mesmo arquivo" << std::endl;
    }

    if (sfs::is_regular_file(path1))
    {
        std::wcout << L"arquivo 1 é um arquivo regular" << std::endl;
    }
    if (sfs::is_regular_file(path2))
    {
        std::wcout << L"arquivo 2 é um arquivo regular" << std::endl;
    }



    auto perms_path1 = sfs::status(path1).permissions();
    std::cout << perms_path1 << std::endl;

    //0511
    //0x00


    if ((perms_path1 & sfs::perms::owner_read) != sfs::perms::none || (perms_path1 & sfs::perms::group_read) != sfs::perms::none)
    {
        std::wcout << L"read file -> owner or group " << std::endl;
    }

    if ((perms_path1 & sfs::perms::owner_write) != sfs::perms::none || (perms_path1 & sfs::perms::group_write) != sfs::perms::none)
    {
        std::wcout << L"write file -> owner or group " << std::endl;
    }
    if ((perms_path1 & sfs::perms::owner_exec) != sfs::perms::none || (perms_path1 & sfs::perms::group_exec) != sfs::perms::none)
    {
        std::wcout << L"exec file -> owner or group " << std::endl;
    }




    /*
    sfs::perms::others_read;
    sfs::perms::others_write;
    sfs::perms::others_exec;
    */


    //std::wstring wstr(L"asdasdsadasd");
    //auto size_of_text = wstr.size() * sizeof(wchar_t);

    //app();
}

#endif
