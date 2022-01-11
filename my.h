#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <string>
#include <string_view>
#include <regex>
#include <chrono>
#include <vector>
#include <array>

#ifndef _HAS_NODISCARD
#ifndef __has_cpp_attribute
#define _HAS_NODISCARD 0
#elif __has_cpp_attribute(nodiscard) >= 201603L // TRANSITION, VSO#939899 (need toolset update)
#define _HAS_NODISCARD 1
#else
#define _HAS_NODISCARD 0
#endif
#endif // _HAS_NODISCARD

#if _HAS_NODISCARD
#define _nodiscard [[nodiscard]]
#else // ^^^ CAN HAZ [[nodiscard]] / NO CAN HAZ [[nodiscard]] vvv
#define _nodiscard
#endif // _HAS_NODISCARD

namespace sfs = std::filesystem;
namespace sch = std::chrono;

#define out std::wcout // configurable
#define in std::wcin // configurable
using mychar = wchar_t; // configurable

using myifstream = std::basic_ifstream<mychar, std::char_traits<mychar>>;
using myofstream = std::basic_ofstream<mychar, std::char_traits<mychar>>;
using myfstream = std::basic_fstream<mychar, std::char_traits<mychar>>;
using myistringstream = std::basic_istringstream<mychar, std::char_traits<mychar>>;
using myostringstream = std::basic_ostringstream<mychar, std::char_traits<mychar>>;
using mystringstream = std::basic_stringstream<mychar, std::char_traits<mychar>>;
using mystring = std::basic_string<mychar, std::char_traits<mychar>>;

using double_mili = sch::duration<double, std::ratio<1, 1000>>;

namespace my
{
    /* DEFINITIONS */

    // console
    template<typename t> inline void output(const t& msg) noexcept;
    template<typename t> inline void _output(const t& msg) noexcept;
    inline void endl() noexcept;
    template<typename t, typename ...Args> inline void output(const t& first, const Args& ...args) noexcept;
    template<typename t, typename ...Args> inline void _output(const t& first, const Args& ...args) noexcept;
    // file
    void output_file(const sfs::path& file_path, const mystring& str) noexcept;
    _nodiscard mystring input_file(const sfs::path& file_path) noexcept;

    class log;
    //other
    inline void pause();

    /* OPERATORS OVERLOADING */

    template<typename t> std::basic_ostream<wchar_t>& operator<<(std::basic_ostream<wchar_t>& Os, const std::vector<t>& vec)
    {
        if (!vec.empty())
        {
            Os << L"{";
            for (auto it = vec.begin(); it != std::prev(vec.end()); ++it)
            {
                Os << L"\"" << *it << L"\", ";
            }
            Os << L"\"" << *std::prev(vec.end()) << L"\"}";
            return Os;
        }
        Os << L"<empty vector>";
        return Os;
    }
    template<typename t> std::basic_ostream<char>& operator<<(std::basic_ostream<char>& Os, const std::vector<t>& vec)
    {
        if (!vec.empty())
        {
            Os << "{";
            for (auto it = vec.begin(); it != std::prev(vec.end()); ++it)
            {
                Os << "\"" << *it << "\", ";
            }
            Os << "\"" << *std::prev(vec.end()) << "\"}";
            return Os;
        }
        Os << "<empty vector>";
        return Os;
    }

    /* loging */

    class log
    {
    public:
        class benchmark
        {
        public: // construction
            benchmark(const mychar* name) : _name(name), _start(sch::system_clock::now())
            {
                output(L"benchmark"); output(_name); output(L"begin"); output(_start); endl();
            }
            ~benchmark()
            {
                output(L"benchmark"); output(_name); output(L"end"); output(double_mili(sch::system_clock::now() - _start)); endl();
            }
            void now()
            {
                output(L"benchmark"); output(_name); output(L"now"); output(double_mili(sch::system_clock::now() - _start)); endl();
            }
        private: // data
            const mychar* _name;
            sch::time_point<sch::system_clock> _start;
        };

    /*
    public: // level
        static const enum class level
        {
            FATAL, ERROR, WARN, INFO, DEBUG
        };
    */
    public: // func
        template<typename t, typename ...Args> inline static void fatal(const t& first, const Args& ...args)
        {
            output(L"fatal"); output(first); output(args...); endl();
        }
        template<typename t, typename ...Args> inline static void fatal(const t& first)
        {
            output(L"fatal"); output(first); endl();
        }
        template<typename t, typename ...Args> inline static void error(const t& first, const Args& ...args)
        {
            output(L"error"); output(first); output(args...); endl();
        }
        template<typename t, typename ...Args> inline static void error(const t& first)
        {
            output(L"error"); output(first); endl();
        }
        template<typename t, typename ...Args> inline static void warning(const t& first, const Args& ...args)
        {
            output(L"warning"); output(first); output(args...); endl();
        }
        template<typename t, typename ...Args> inline static void warning(const t& first)
        {
            output(L"warning"); output(first); endl();
        }
        template<typename t, typename ...Args> inline static void info(const t& first, const Args& ...args)
        {
            output(L"info"); output(first); output(args...); endl();
        }
        template<typename t, typename ...Args> inline static void info(const t& first)
        {
            output(L"info"); output(first); endl();
        }
        template<typename t, typename ...Args> inline static void debug(const t& first, const Args& ...args)
        {
            output(L"debug"); output(first); output(args...); endl();
        }
        template<typename t, typename ...Args> inline static void debug(const t& first)
        {
            output(L"debug"); output(first); endl();
        }
    public: // constructors
        log() = delete;
    private:
        template<typename t> inline static void output(const t& msg) noexcept
        {
            out << '<' << msg << '>';
        }
        inline static void endl() noexcept
        {
            out << std::endl;
        }
        template<typename t, typename ...Args> inline static void output(const t& first, const Args& ...args) noexcept
        {
            output(first); output(args...);
        }
    };

    /* INPUT E OUTPUT FUNCTIONS */

    template<typename t> inline void output(const t& msg) noexcept
    {
        out << msg;
    }
    template<typename t> inline void _output(const t& msg) noexcept
    {
        out << msg << std::endl;
    }
    inline void endl() noexcept
    {
        out << std::endl;
    }
    template<typename t, typename ...Args> inline void output(const t& first, const Args& ...args) noexcept
    {
        output(first); output(args...);
    }
    template<typename t, typename ...Args> inline void _output(const t& first, const Args& ...args) noexcept
    {
        output(first); output(args...); endl();
    }
     
    /* OTHER */
    
    inline void pause()
    {
        output("<Press any key to continue>");
        in.get(); 
    }
    
    /* INPUT E OUTPUT FILE */

    _nodiscard mystring input_file(const sfs::path& file_path) noexcept
    {
        // alloc
        static mystring file;
        static myifstream fs;
        static mychar caracter = 0;
        static std::streampos end;
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
                    std::wcout << L"Error";
                    exit(1);
                }
                caracter = 0;
            }
        }
        else
        {
            std::cout << "Error";
            exit(1);
        }
        fs.close();
        fs.clear();
        return file;
    }

    void output_file(const sfs::path& file_path, const mystring& str) noexcept
    {
        // alloc
        static myofstream fs;
        fs.open(file_path);
        if (fs.is_open())
        {
            fs.seekp(0, std::ios_base::end);
            fs.clear();
            if (!(fs << str))
            {
                std::cout << "Error";
                exit(1);
            }
        }
        else
        {
            std::cout << "Error";
            exit(1);
        }
        fs.close();
        fs.clear();
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
}

#undef in
#undef out
