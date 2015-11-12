#include <iostream>
#include "include/minishell/minishell.h"

using namespace std;

int main() {
    MiniShell *shell = new MiniShell("(#%d, exit code: %d, last pid: %d) > ");

    shell->setUpHandler();
    shell->loop();

    return 0;
}

