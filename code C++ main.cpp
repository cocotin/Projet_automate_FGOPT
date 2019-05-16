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

void menu() {
    printf("Quel automate voulez-vous utiliser?\nindiquez son numero(0 pour quitter) : ");
    char reponse[20] = "";
    scanf("%19s", reponse);
    if (reponse[0] == '0') {
        if (reponse[1] == '\0') {
            return;
        }
    }
    string nom_fichier = "L2-F1-" + (string)reponse + ".txt";
    cout << nom_fichier << endl;
    aff_fichier(nom_fichier);
    Automate toast(nom_fichier);
    toast.afficher();
    menu();
}

int main()
{
    menu();
    return 0;
}
