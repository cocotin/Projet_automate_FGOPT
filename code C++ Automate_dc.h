#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "Automate.h"
#include "Liste.h"

using namespace std;

class Automate_dc
{
public:
    Automate_dc(Automate af, char type);
    ~Automate_dc();
    void afficher();
    string nom_fichier_source;
    int taille_alphabet;
    int nb_etats;
    int* nb_etats_superposes;
    int** etats;
    int poubelle;
    int nro_etat_initial;
    int nb_etats_terminaux;
    int* nro_etats_terminaux;
    int** transitions;
};
