# MiniShell

MiniShell créé en C++ dans le cadre d'un projet en Programmation Système à l'IUT.

## Fonctionnalités
- [x] Définition simplifiéed de commandes propres au shell
- [x] Fonctions propres au shell : exit & kill
- [x] Utilisation de la variable $PATH
- [x] Support des commandes asynchrones (&)
- [ ] Support des couleurs

## Utilisation
```c++
#include "include/minishell/minishell.h"

int main() {
    // Initialise le shell en passant comme paramètre un semblant de prompt shell
    MiniShell *shell = new MiniShell("(#%d, exit code: %d, last pid: %d) > ");
    // Numéro de la commande -----------^              ^              ^
    // Code de sortie de la commande précédente -------|              |
    // PID de la dernière commande -----------------------------------|
    
    // Empêche le ^C :-)
    shell->setUpHandler();
    
    // Boucle principale permettant à l'utilisateur de rentrer des commandes
    shell->loop();
}
```
