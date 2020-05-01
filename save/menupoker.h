#ifndef _MENUPOKER
#define _MENUPOKER

#include "menu.h"

class Semestre;

class MenuMoyenne : public Menu
{
    
public:
    MenuMoyenne(Semestre* semestre);
    void executerOption(const string& nom, bool& fin);
   
};


#endif
