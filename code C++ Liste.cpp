#include "Liste.h"

Liste::Liste(int* petat, int nb_etats, int ptaille_alphabet)
{
    taille_alphabet = ptaille_alphabet;
    taille_etat = nb_etats;
    etat = (int*)malloc(taille_etat * sizeof(int));
    for (int i = 0; i < taille_etat; i++) {
        etat[i] = petat[i];
    }
    transitions = (int**)malloc(taille_alphabet * sizeof(int*));
    nb_etats_superposes = (int*)malloc(taille_alphabet * sizeof(int));
    suivant = NULL;
}

Liste::~Liste()
{
    for (int i = 0; i < taille_alphabet; i++) {
        free(transitions[i]);
    }
    free(etat);
    free(nb_etats_superposes);
    free(transitions);
}

