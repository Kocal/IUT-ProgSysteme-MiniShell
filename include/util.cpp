#include "util.h"

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <regex>

using std::string;

Util::Util() {

}

string & Util::ltrim(string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

string & Util::rtrim(string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

string & Util::trim(string &s) {
    return Util::ltrim(Util::rtrim(s));
}

 string Util::replaceSpacesBySpace(string s) {
    std::regex reg("( {2,})");

    return std::regex_replace(s, reg, " ");
 }
