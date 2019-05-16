#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class Automate
{
public:
    Automate(string fichier_source);
    int est_un_automate_asynchrone();
    int est_un_automate_deterministe();
    int est_un_automate_complet();
    ~Automate();
    void afficher();
private:
    string nom_fichier_source;
    int correct;
    int taille_alphabet;
    int nb_etats;
    int nb_etats_initiaux;
    int* etats_initiaux;
    int nb_etats_terminaux;
    int* etats_terminaux;
    int nb_transitions;
    int** transitions;
    int** nb_transit;
    int*** table_transitions;
};
