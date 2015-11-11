#include <iostream>
#include "include/minishell/minishell.h"

using namespace std;

int main() {
    MiniShell *shell = new MiniShell("(#%d, exit code: %d) > ");

    shell->loop();

    return 0;
}

