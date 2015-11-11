#ifndef UTIL_H
#define UTIL_H

#include <string>

using std::string;

typedef struct arguments {
    int argc;
    char **argv;
} arguments;

class Util {
    public:
        Util();

        static string & ltrim(string &s);
        static string & rtrim(string &s);
        static string & trim(string &s);

        static string replaceSpacesBySpace(string s);
};

#endif // UTIL_H
