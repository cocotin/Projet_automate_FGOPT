#include "Automate_dc.h"

int egaux(int* ta, int na, int* tb, int nb)
{
    if (na != nb) return 0;
    for (int i = 0; i < na; i++) {
        if (ta[i] != tb[i]) return 0;
    }
    return 1;
}

int correspondant(Liste* tete, int* etat, int netat)
{
    int n = 0;
    while (!egaux(tete->etat, tete->taille_etat, etat, netat)) {
        tete = tete->suivant;
        n++;
    }
    return n;
}

int contient(int* ter, int nter, int* etat, int netat)
{
    for (int i = 0; i < nter; i++) {
        for (int j = 0; j < netat; j++) {
            if (ter[i] == etat[j]) return 1;
        }
    }
    return 0;
}

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
        poubelle = 0;
        for (int j = 0; j < taille_alphabet; j++) {
            int* etats_presents = (int*)malloc(af.nb_etats * sizeof(int));
            for (int i = 0; i < af.nb_etats; i++) etats_presents[i] = 0;
            for (int i = 0; i < af.nb_etats_initiaux; i++) {
                for (int k = 0; k < af.nb_transit[af.etats_initiaux[i]][j]; k++) {
                    etats_presents[af.table_transitions[af.etats_initiaux[i]][j][k]] = 1;
                }
            }
            int n = 0;
            for (int i = 0; i < af.nb_etats; i++) {
                n += etats_presents[i];
            }
            if (n == 0) {
                tete->nb_etats_superposes[j] = 1;
                tete->transitions[j] = (int*)malloc(1 * sizeof(int));
                tete->transitions[j][0] = -1;
                poubelle = 1;
            } else {
                tete->nb_etats_superposes[j] = n;
                tete->transitions[j] = (int*)malloc(n * sizeof(int));
                n = 0;
                for (int i = 0; i < af.nb_etats; i++) {
                    if (etats_presents[i]) {
                        tete->transitions[j][n] = i;
                        n++;
                    }
                }
            }
        }
        Liste* traite = tete;
        Liste* que = tete;
        for (int i = 0; i < taille_alphabet; i++) {
            if (tete->transitions[i][0] != -1) {
                if (nouveau(tete, tete->transitions[i], tete->nb_etats_superposes[i])) {
                    que->suivant = new Liste(tete->transitions[i], tete->nb_etats_superposes[i], taille_alphabet);
                    que = que->suivant;
                }
            }
        }
        while (traite->suivant != NULL) {
            traite = traite->suivant;
            for (int j = 0; j < taille_alphabet; j++) {
                int* etats_presents = (int*)malloc(af.nb_etats * sizeof(int));
                for (int i = 0; i < af.nb_etats; i++) etats_presents[i] = 0;
                for (int i = 0; i < traite->taille_etat; i++) {
                    for (int k = 0; k < af.nb_transit[traite->etat[i]][j]; k++) {
                        etats_presents[af.table_transitions[traite->etat[i]][j][k]] = 1;
                    }
                }
                int n = 0;
                for (int i = 0; i < af.nb_etats; i++) {
                    if (etats_presents[i]) n++;
                }
                if (n == 0) {
                    poubelle = 1;
                    traite->nb_etats_superposes[j] = 1;
                    traite->transitions[j] = (int*)malloc(1 * sizeof(int));
                    traite->transitions[j][0] = -1;
                } else {
                    traite->nb_etats_superposes[j] = n;
                    traite->transitions[j] = (int*)malloc(n * sizeof(int));
                    n = 0;
                    for (int i = 0; i < af.nb_etats; i++) {
                        if (etats_presents[i]) {
                            traite->transitions[j][n] = i;
                            n++;
                        }
                    }
                    if (nouveau(tete, traite->transitions[j], traite->nb_etats_superposes[j])) {
                        que->suivant = new Liste(traite->transitions[j], traite->nb_etats_superposes[j], taille_alphabet);
                        que = que->suivant;
                    }
                }
            }
        }
        int n = 0;
        traite = tete;
        while (traite != NULL) {
            n++;
            traite = traite->suivant;
        }
        if (poubelle) {
            nb_etats = n+1;
        } else {
            nb_etats = n;
        }
        etats = (int**)malloc(nb_etats * sizeof(int*));
        nb_etats_superposes = (int*)malloc(nb_etats * sizeof(int));
        transitions = (int**)malloc(nb_etats * sizeof(int*));
        traite = tete;
        n = 0;
        int eta_ter = 0;
        while (traite != NULL) {
            nb_etats_superposes[n] = traite->taille_etat;
            etats[n] = (int*)malloc(nb_etats_superposes[n] * sizeof(int));
            for (int i = 0; i < nb_etats_superposes[n]; i++) etats[n][i] = traite->etat[i];
            transitions[n] = (int*)malloc(taille_alphabet * sizeof(int));
            if (contient(af.etats_initiaux, af.nb_etats_terminaux, etats[n], nb_etats_superposes[n])) {
                eta_ter++;
            }
            for (int j = 0; j < taille_alphabet; j++) {
                transitions[n][j] = correspondant(tete, traite->transitions[j], traite->nb_etats_superposes[j]);
            }
            traite = traite->suivant;
            n++;
        }
        if (poubelle) {
            for (int i = 0; i < taille_alphabet; i++) transitions[nb_etats-1][i] = -1;
        }
        nb_etats_terminaux = eta_ter;
        nro_etats_terminaux = (int*)malloc(nb_etats_terminaux * sizeof(int));
        eta_ter = 0;
        int nro = 0;
        traite = tete;
        while (traite != NULL) {
            if (contient(af.etats_initiaux, af.nb_etats_terminaux, etats[n], nb_etats_superposes[n])) {
                nro_etats_terminaux[eta_ter] = nro;
                eta_ter++;
            }
            traite = traite->suivant;
            nro++;
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
    for (int c = 'a'; c < 'a' + taille_alphabet; c++) {
        for (int i = 0; i < maxi_superpo; i++) cout << "  ";
        cout << (char)c;
    }
    for (int e = 0; e < nb_etats; e++) {
        cout << endl << etats[e][0];
        for (int i = 1; i < nb_etats_superposes[e]; i++) cout << "," << etats[e][i];
        for (int i = 2 * nb_etats_superposes[e] - 1; i < 2 * maxi_superpo; i++) cout << " ";
        for (int j = 0; j < taille_alphabet; j++) {
            if (transitions[e][j] == -1) {
                cout << 'P';
            } else {
                cout << etats[transitions[e][j]][0];
                for (int i = 1; i < nb_etats_superposes[transitions[e][j]]; i++) {
                    cout << "," << etats[transitions[e][j]][i];
                }
            }
            for (int i = 2 * nb_etats_superposes[transitions[e][j]] - 1; i < 2 * maxi_superpo; i++) cout << " ";
        }
    }
    cout << endl << "------------------------------------" << endl;
}
