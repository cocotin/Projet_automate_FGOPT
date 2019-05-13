#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "Automate.h"

using namespace std;

void aff_fichier(string fichier_source) {
    ifstream fichier(fichier_source.c_str(), ios::in);
    if(fichier)
    {
        string ligne;
        while(getline(fichier, ligne))
        {
            cout << ligne << endl;
        }
        fichier.close();
    } else {
        cerr << "Impossible d'ouvrir le fichier \"" << fichier_source << "\"" << endl;
    }
}

int main()
{
    aff_fichier("Test.txt");
    Automate toast("Test.txt");
    toast.afficher();
    return 0;
}
