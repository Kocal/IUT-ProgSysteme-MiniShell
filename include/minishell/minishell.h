#ifndef MINISHELL_H
#define MINISHELL_H

#include "../util.h"
#include <string>

using std::string;

class MiniShell {
    public:
        MiniShell(string ps1 = "> ");

        void setUpHandler(void);

        void loop(void);
        string ask(void);
        arguments parseInput(string input);
        int exec(arguments args, int &lastPid);
        bool isAsync(arguments &args);

        int getLastPid() { return this->lastPid; }
        void setLastPid(int pid) { this->lastPid = pid; }


    private:
        string ps1;
        int lastPid;
};

#endif // MINISHELL_H
