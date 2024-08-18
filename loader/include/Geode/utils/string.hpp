#pragma once

#include <Sapfire/DefaultInclude.hpp>
#include <functional>
#include <string>
#include <vector>

namespace sapfire::utils::string {
    /**
     * Convert std::wstring to std::string (UTF-8)
     * @param str String to convert
     * @returns std::string
     */
    SAPFIRE_DLL std::string wideToUtf8(std::wstring const& str);
    /**
     * Convert std::string (UTF-8) to std::wstring
     * @param str String to convert
     * @returns std::wstring
     */
    SAPFIRE_DLL std::wstring utf8ToWide(std::string const& str);

    SAPFIRE_DLL std::string& toLowerIP(std::string& str);

    SAPFIRE_DLL std::string toLower(std::string const& str);

    SAPFIRE_DLL std::string& toUpperIP(std::string& str);

    SAPFIRE_DLL std::string toUpper(std::string const& str);

    SAPFIRE_DLL std::string& replaceIP(
        std::string& str, std::string const& orig, std::string const& repl
    );

    SAPFIRE_DLL std::string replace(
        std::string const& str, std::string const& orig, std::string const& repl
    );

    SAPFIRE_DLL std::vector<std::string> split(std::string const& str, std::string const& split);

    SAPFIRE_DLL std::string join(std::vector<std::string> const& strs, std::string const& separator);

    SAPFIRE_DLL std::vector<char> split(std::string const& str);

    SAPFIRE_DLL bool contains(std::string const& str, std::string const& subs);

    SAPFIRE_DLL bool contains(std::string const& str, char c);

    SAPFIRE_DLL bool containsAny(std::string const& str, std::vector<std::string> const& subs);

    SAPFIRE_DLL bool containsAll(std::string const& str, std::vector<std::string> const& subs);

    SAPFIRE_DLL size_t count(std::string const& str, char c);

    SAPFIRE_DLL std::string& trimLeftIP(std::string& str);
    SAPFIRE_DLL std::string& trimRightIP(std::string& str);
    SAPFIRE_DLL std::string& trimIP(std::string& str);

    SAPFIRE_DLL std::string trimLeft(std::string const& str);
    SAPFIRE_DLL std::string trimRight(std::string const& str);
    SAPFIRE_DLL std::string trim(std::string const& str);

    SAPFIRE_DLL std::string& normalizeIP(std::string& str);
    SAPFIRE_DLL std::string normalize(std::string const& str);

    SAPFIRE_DLL bool startsWith(std::string const& str, std::string const& prefix);
    SAPFIRE_DLL bool endsWith(std::string const& str, std::string const& suffix);
}
