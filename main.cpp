#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <string_view>
#include <sstream>
#include <fstream>
#include <regex>

//#include "my.h"
//#include "json.h"

#define _nodiscard [[nodiscard]]

// declarations
void str_to_mili(std::wstring* str) noexcept;
void format_str(std::wstring* str) noexcept;
void regex_gamers(std::wstring* s) noexcept;
_nodiscard std::wstring load_file(const std::wstring& file_path) noexcept;
void write_file(std::wstring_view str, const std::wstring& path) noexcept;
void app();

int main()
{
    app();
}

void app()
{
    //load file
    std::wstring file = load_file(L"C:\\Users\\anonimo\\Desktop\\Animemap.cs");
    regex_gamers(&file);
    write_file(file, L"C:\\Users\\anonimo\\Desktop\\Animemap2222.cs");
}

// wstring with mileseconds
void str_to_mili(std::wstring* str) noexcept
{
    //vars
    static std::wstringstream temp_ss;
    static unsigned int _minutos;
    static unsigned int _segundos;
    static unsigned int _milisegundos;
    std::wstring temp_str;
    
    // spliting
    temp_ss << *str;
    temp_ss >> _minutos;
    temp_ss >> _segundos;
    temp_ss >> _milisegundos;

    temp_ss.str(std::wstring());
    temp_ss.clear();

    temp_ss << std::chrono::milliseconds(std::chrono::milliseconds(_milisegundos) + std::chrono::seconds(_segundos) + std::chrono::minutes(_minutos)).count();
    temp_ss >> temp_str;
    
    _minutos = 0;
    _segundos = 0;
    _milisegundos = 0;
    temp_ss.str(std::wstring());
    temp_ss.clear();
    
    (*str).clear();
    *str = temp_str;
}

// remove bad caracters
void format_str(std::wstring* str) noexcept
{
    std::vector<char> delete_chars = { ':','"' };

    for (auto it = delete_chars.begin(); it != delete_chars.end(); ++it)
    {
        for (auto i = 0; i != (*str).size(); ++i)
        {
            if ((*str)[i] == *it)
            {
                (*str)[i] = ' ';
            }
        }   
    }
}

// regex search
void regex_gamers(std::wstring* str) noexcept
{
    std::wsmatch rgx_matchs;
    //std::wregex rgx(L"[[:d:]]{2,}\:[[:d:]][[:d:]]\:[[:d:]][[:d:]]{2,}");
    std::wregex rgx(L"\"?[[:d:]]{2,}\:[[:d:]][[:d:]]\:[[:d:]][[:d:]]{2,}\"?");

    while (std::regex_search((*str), rgx_matchs, rgx))
    {
        // sub str creation and edittion
        std::wstring sub_str(rgx_matchs.str());
        format_str(&sub_str);
        str_to_mili(&sub_str);

        // replace 

        auto tempstr = rgx_matchs.prefix().str() + sub_str + rgx_matchs.suffix().str();
        (*str) = tempstr;
    }
}

// load any file
_nodiscard std::wstring load_file(const std::wstring& file_path) noexcept
{
    // alloc
    static std::wstring file;
    static std::wfstream fs;
    static wchar_t caracter = 0;
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

void write_file(std::wstring_view str, const std::wstring& path) noexcept
{
    // alloc
    static std::wfstream fs;
    fs.open(path);
    if (fs.is_open())
    {
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
