#include "Automate_dc.h"

Automate_dc::Automate_dc()
{

}

Automate_dc::Automate_dc(Automate af, char style)
{
    if (style == 'n') {
        nom_fichier_source = af.nom_fichier_source;
        nb_etats = af.nb_etats;
        nb_etats_superposes = (int*)malloc(nb_etats * sizeof(int));
        for (int i = 0; i < nb_etats; i++) {
            nb_etats_superposes[i] = 1;
        }
        etats = (int**)malloc(nb_etats * sizeof(int*));
        for (int i = 0; i < nb_etats; i++) {
            etats[i] = (int*)malloc(1 * sizeof(int));
            etats[i][0] = i;
        }
        nro_etat_initial = af.etats_initiaux[0];
        nb_etats_terminaux = af.nb_etats_terminaux;
        nro_etats_terminaux = (int*)malloc(nb_etats_terminaux * sizeof(int));
        for (int i = 0; i < nb_etats_terminaux; i++) {
            nro_etats_terminaux[i] = af.etats_terminaux[i];
        }
        transitions = (int**)malloc(nb_etats * sizeof(int*));
        for (int i = 0; i < nb_etats; i++) {
            transitions[i] = (int*)malloc(taille_alphabet * sizeof(int));
            for (int j = 0; j < taille_alphabet; j++) {
                transitions[i][j] = af.transitions[i][j];
            }
        }
    }
    if (style == 'c') {
        nom_fichier_source = af.nom_fichier_source;
        taille_alphabet = af.taille_alphabet;
        nb_etats = af.nb_etats + 1;
        nb_etats_superposes = (int*)malloc(nb_etats * sizeof(int));
        for (int i = 0; i < nb_etats; i++) {
            nb_etats_superposes[i] = 1;
        }
        etats = (int**)malloc(nb_etats * sizeof(int*));
        for (int i = 0; i < nb_etats; i++) {
            etats[i] = (int*)malloc(1 * sizeof(int));
            etats[i][0] = i;
        }
        nro_etat_initial = af.etats_initiaux[0];
        nb_etats_terminaux = af.nb_etats_terminaux;
        nro_etats_terminaux = (int*)malloc(nb_etats_terminaux * sizeof(int));
        for (int i = 0; i < nb_etats_terminaux; i++) {
            nro_etats_terminaux[i] = af.etats_terminaux[i];
        }
        transitions = (int**)malloc(nb_etats * sizeof(int*));
        for (int i = 0; i < nb_etats-1; i++) {
            transitions[i] = (int*)malloc(taille_alphabet * sizeof(int));
            for (int j = 0; j < taille_alphabet; j++) {
                transitions[i][j] = af.transitions[i][j];
            }
            transitions[taille_alphabet-1] = (int*)malloc(taille_alphabet * sizeof(int));
            for (int j = 0; j < taille_alphabet; j++) {
                transitions[i][j] = nb_etats-1;
            }
        }
    } else if (style == 'a') {

    } else { // style == 's'

    }
}

Automate_dc::~Automate_dc()
{
    free(nb_etats_superposes);
    for (int i = 0; i < nb_etats; i++) {
        free(etats[i]);
    }
    free(etats);
    free(nro_etats_terminaux);
    for (int i = 0; i < nb_etats; i++) {
        free(transitions[i]);
    }
    free(transitions);
}

void Automate_dc::afficher()
{
    cout << "-----------------------------------------------" << endl;
    for (int i = 0; i < nb_etats; i++) {cout << nb_etats_superposes[i] << endl;}
    cout << "automate deterministe complet issu du fichier : " << nom_fichier_source << endl << endl;
    cout << "etat initial : " << etats[nro_etat_initial][0];
    for (int i = 0; i < nb_etats_superposes[nro_etat_initial]; i++) cout << "," << etats[nro_etat_initial][i];
    cout << endl << "nombres d'etats terminaux : " << nb_etats_terminaux;
    for (int i = 0; i < nb_etats_terminaux; i++) {
        cout << endl << "        " << etats[nro_etats_terminaux[i]][0];
        for (int j = 0; j < nb_etats_superposes[nro_etats_terminaux[i]]; j++) cout << "," << etats[nro_etats_terminaux[i]][j];
    }
    cout << endl << "Table de transition :" << endl << endl;
    int maxi_superpo = 1;
    for (int i = 0; i < nb_etats; i++) {
        if (nb_etats_superposes[i] > maxi_superpo) maxi_superpo = nb_etats_superposes[i];
    }
    for (int c = 'a'; c < 'a' + taille_alphabet; c++) {
        for (int i = 0; i < maxi_superpo; i++) cout << " ";
        cout << c;
    }
    for (int e = 0; e < nb_etats; e++) {
        cout << endl << etats[e][0];
        for (int i = 1; i < nb_etats_superposes[e]; i++) cout << "," << etats[e][i];
        for (int i = 2 * nb_etats_superposes[e] - 1; i < 2 * maxi_superpo; i++) cout << " ";
        for (int j = 0; j < taille_alphabet; j++) {
            cout << etats[transitions[e][j]][0];
            for (int i = 1; i < nb_etats_superposes[transitions[e][j]]; i++) {
                cout << "," << etats[transitions[e][j]][i];
            }
            for (int i = 2 * nb_etats_superposes[transitions[e][j]] - 1; i < 2 * maxi_superpo; i++) cout << " ";
        }
    }
    cout << endl << "------------------------------------" << endl;
}
