#include <iostream>
using namespace std;
#include "menu.h"




MenuPoker::MenuPoker() : Menu("Exemple de menu :")
{
    
    ajouterOption("aide", "Afficher de l'aide");
    ajouterOption("commencer", "Commencer la partie");    
    ajouterOption("quitter", "Quitter la partie");
}
