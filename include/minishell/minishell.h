#ifndef MINISHELL_H
#define MINISHELL_H

#include "../util.h"
#include <string>

using std::string;

class MiniShell {
    public:
        MiniShell(string ps1 = "> ");

        void loop(void);
        string ask(void);
        arguments parseInput(string input);
        int exec(arguments args);

    private:
        string ps1;
};

#endif // MINISHELL_H
