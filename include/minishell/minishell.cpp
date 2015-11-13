#include "minishell.h"
#include "../util.h"
#include <iostream>
#include <string>
#include <vector>

#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;

int _ms_exit(MiniShell *ms, arguments args);
int _ms_kill(MiniShell *ms, arguments args);

string _ms_internalCmdStr [] = {
  "exit", "kill"
};

int (*_ms_internalCmdFunc[]) (MiniShell *, arguments) = {
    &_ms_exit, &_ms_kill
};

int _ms_getInternalCommandSize() {
    return sizeof(_ms_internalCmdStr) / sizeof(_ms_internalCmdStr[0]);
}

MiniShell::MiniShell(string ps1) : ps1(ps1) {

}

void MiniShell::loop(void) {

    int lines = 0;
    int status = 0;
    string input;
    arguments args;

    do {
        do {
           printf(this->ps1.c_str(), lines++, status, this->lastPid);
           input = this->ask();
           input = Util::trim(input);
        } while (input.empty());

        args = this->parseInput(input);
        status = this->exec(args, this->lastPid);
    } while(true);
}

string MiniShell::ask(void) {
    string input;

    cin.clear();

    // Ctrl+D
    if(!getline(cin, input)) {
        cout << endl;
    }

    return Util::trim(input);
}

arguments MiniShell::parseInput(string input) {
    arguments args;
    string argument;
    vector<string> vec;

    unsigned int i;

    input = Util::replaceSpacesBySpace(input);

    // Séparation des arguments en un vecteur
    for(i = 0; i < input.length(); i++) {
        argument += input.at(i);

        if(input.at(i) == ' ' || i == input.length() - 1) {
            vec.push_back(Util::trim(argument));
            argument = "";
        }
    }

    i = 0;
    args.argc = vec.size();
    args.argv = new char*[args.argc + 1];

    // Conversion du vecteur en un tableau de string
    for(vector<string>::iterator it = vec.begin(); it != vec.end(); ++it) {
        args.argv[i] = new char[it->length() + 1];
        strcpy(args.argv[i++], it->c_str());
    }

    args.argv[i] = NULL;

    return args;
}

int MiniShell::exec(arguments args, int &lastPid) {

    pid_t pid, wpid;
    int status;

    bool async = this->isAsync(args);

    // On exécute la commande interne en priorité, puis on quitte
    for(int i = 0; i < _ms_getInternalCommandSize(); i++) {
        if(strcmp(_ms_internalCmdStr[i].c_str(), args.argv[0]) == 0) {
            return (*_ms_internalCmdFunc[i])(this, args);
        }
    }

    pid = fork();

    switch (pid) {
        // erreur
        case -1: {
            perror("MS");
            break;
        }

        // fils
        case 0: {
            if(async) {
                cout << "Started " << getpid() << " in asynchronous mode" << endl;
            }

            if(execvp(args.argv[0], args.argv) == -1) {
                perror("MS");
            }

            exit(EXIT_FAILURE);
        }

        // père
        default: {
            lastPid = pid;

            // On attend la fin du processus
            if(!async) {
                do {
                    wpid = waitpid(pid, &status, WUNTRACED);
                } while(!WIFEXITED(status) && !WIFSIGNALED(status));

                return WEXITSTATUS(status);
            }
        }
    }

    return 1;
}

bool MiniShell::isAsync(arguments &args) {
    char* lastArg = args.argv[args.argc - 1];

    // Si le dernier caractère du dernier argument est un '&'
    if(lastArg[strlen(lastArg) - 1] == '&') {
        lastArg[strlen(lastArg) - 1] = '\0';

        // On supprime le superflu
        if(strlen(lastArg) == 0) {
            delete args.argv[args.argc--];
            args.argv[args.argc] = NULL;
        }

        return true;
    }

    return false;
}

int _ms_exit(MiniShell *ms, arguments args) {
    cout << "ms: bye" << endl;
    exit(EXIT_SUCCESS);
}

int _ms_kill(MiniShell *ms, arguments args) {

    if(ms->getLastPid() != 0) {
        if(kill(ms->getLastPid(), SIGKILL) != 0) {
            perror("_ms_kill");
        } else {
            cout << "successfully killed " << ms->getLastPid() << endl;
        }

        ms->setLastPid(0);
        return EXIT_SUCCESS;
    }

    cout << "Impossible de tuer MiniShell :-)" << endl;
    return EXIT_FAILURE;
}

void handler(int sig) {
    //cout << "ms: signal " << sig << " received" << endl;
}

void MiniShell::setUpHandler(void) {
    signal(SIGINT, handler);
}
