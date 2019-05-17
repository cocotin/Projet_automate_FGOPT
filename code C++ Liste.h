#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class Liste
{
public:
    Liste(int* petat, int nb_etats, int ptaille_alphabet);
    ~Liste();
    int taille_alphabet;
    int taille_etat;
    int* etat;
    int* nb_etats_superposes;
    int** transitions;
    Liste* suivant;
};

