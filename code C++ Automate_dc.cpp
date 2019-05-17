#include "Automate_dc.h"

int nouveau(Liste* l, int* tab, int t)
{
    if (l == NULL) return 1;
    if (l->taille_etat != t) return nouveau(l->suivant, tab, t);
    for (int i = 0; i < t; i++) {
        if (l->etat[i] != tab[i]) return nouveau(l->suivant, tab, t);
    }
    return 0;
}

Automate_dc::Automate_dc(Automate af, char style)
{
    if (style == 'n') {
        cout << 'n' << endl;
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
    } else if (style == 'c') {
        cout << "completion" << endl;
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
                if (af.nb_transit[i][j] == 1) {
                    transitions[i][j] = af.transitions[i][j];
                } else {
                    transitions[i][j] = nb_etats-1;
                }
            }
        }
        transitions[nb_etats-1] = (int*)malloc(taille_alphabet * sizeof(int));
        for (int j = 0; j < taille_alphabet; j++) {
            transitions[nb_etats-1][j] = nb_etats-1;
        }
    } else if (style == 's') {
        cout << "determinisation_et_completion_automate_synchrone" << endl;
        nom_fichier_source = af.nom_fichier_source;
        taille_alphabet = af.taille_alphabet;
        nro_etat_initial = 0;
        Liste* tete = new Liste(af.etats_initiaux, af.nb_etats_initiaux, taille_alphabet);
        for (int j = 0; j < taille_alphabet; j++) {
            int* etats = (int*)malloc(af.nb_etats * sizeof(int));
            for (int i = 0; i < af.nb_etats; i++) etats[i] = 0;
            for (int i = 0; i < af.nb_etats_initiaux; i++) {
                for (int k = 0; k < af.nb_transit[af.etats_initiaux[i]][j]; k++) {
                    etats[af.table_transitions[af.etats_initiaux[i]][j][k]] = 1;
                }
            }
            int n = 0;
            for (int i = 0; i < af.nb_etats; i++) {
                n += etats[i];
            }
            tete->nb_etats_superposes[j] = n;
            tete->transitions[j] = (int*)malloc(n * sizeof(int));
            n = 0;
            for (int i = 0; i < af.nb_etats; i++) {
                if (etats[i]) {
                    tete->transitions[j][n] = i;
                    n++;
                }
            }
        }
        Liste* traite = l;
        Liste* que = l;
        for (int i = 0; i < taille_alphabet; i++) {
            if (nouveau(tete, tete->transitions[i], tete->nb_etats_superposes[i])) {
                que->suivant = new Liste(tete->transitions[i], tete->nb_etats_superposes[i], taille_alphabet);
                que = que->suivant;
            }
        }
    } else {
        cout << "determinisation_et_completion_automate_asynchrone" << endl;
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
    cout << "automate deterministe complet issu du fichier : " << nom_fichier_source << endl << endl;
    cout << "etat initial : " << etats[nro_etat_initial][0];
    for (int i = 1; i < nb_etats_superposes[nro_etat_initial]; i++) cout << "," << etats[nro_etat_initial][i];
    cout << endl << "nombres d'etats terminaux : " << nb_etats_terminaux;
    for (int i = 0; i < nb_etats_terminaux; i++) {
        cout << endl << "        " << etats[nro_etats_terminaux[i]][0];
        for (int j = 1; j < nb_etats_superposes[nro_etats_terminaux[i]]; j++) cout << "," << etats[nro_etats_terminaux[i]][j];
    }
    cout << endl << "Table de transition :" << endl << endl;
    int maxi_superpo = 1;
    for (int i = 0; i < nb_etats; i++) {
        if (nb_etats_superposes[i] > maxi_superpo) maxi_superpo = nb_etats_superposes[i];
    }
    cout << " ";
    for (int c = 'a'; c < 'a' + taille_alphabet; c++) {
        for (int i = 0; i < maxi_superpo; i++) cout << " ";
        cout << (char)c;
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
