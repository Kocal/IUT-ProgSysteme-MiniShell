#include "minishell.h"
#include "../util.h"
#include <iostream>
#include <string>
#include <vector>

#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;

MiniShell::MiniShell(string ps1) : ps1(ps1) {

}

void MiniShell::loop(void) {

    int lines = 0;
    int status = 0;
    string input;
    arguments args;

    do {
        printf(this->ps1.c_str(), lines++, status);
        input = this->ask();
        args = this->parseInput(input);
        status = this->exec(args);
    } while(true);

}

string MiniShell::ask(void) {
    string input;

    cin.clear();

    // Ctrl+D
    if(!getline(cin, input)) {
        cout << endl;
        exit(EXIT_SUCCESS);
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

int MiniShell::exec(arguments args) {

    pid_t pid, wpid;
    int status;

    if(strcmp(args.argv[0], "exit") == 0) {
        cout << "Au revoir !" << endl;
        exit(EXIT_SUCCESS);
    }

    pid = fork();

    switch (pid) {
        // erreur
        case -1:
            perror("MS");
            break;

        // fils
        case 0:

            // vive le path
            if(execvp(args.argv[0], args.argv) == -1) {
                perror("MS");
            }

            exit(EXIT_FAILURE);

        // père
        default:

            // On attend la fin du processus
            do {
                wpid = waitpid(pid, &status, WUNTRACED);
            } while(!WIFEXITED(status) && !WIFSIGNALED(status));

            return WEXITSTATUS(status);
    }

    return 1;
}
