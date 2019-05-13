#include "Automate.h"

aff_err(string nom_fichier_source, int nro_ligne, int nro_colonne, string message = "")
{
    cerr << "fichier " << nom_fichier_source << " non conforme: ligne " << nro_ligne << " colonne " << nro_colonne << " " << message << endl;
}

int c_un_nb(string nom_fichier_source, int nro_ligne, string ligne)
{
    for (int i = 0; i < ligne.length(); i++) {
        if (ligne[i] < '0' || ligne[i] > '9') {
            aff_err(nom_fichier_source, nro_ligne, i+1);
            return 0;
        }
    }
    return 1;
}

int conv_nb(string ligne)
{
    int n = 0;
    for (int i = 0; i < ligne.length(); i++) {
        n = 10 * n + (int)(ligne[i] - '0');
    }
    return n;
}

int nb_nbr(string nom_fichier_source, int nro_ligne, string ligne, int longeur, int depart)
{
    if (ligne[depart] < '0' || ligne[depart] > '9') aff_err(nom_fichier_source, nro_ligne, depart+1);
    depart++;
    while (depart < longeur) {
        if (ligne[depart] < '0' || ligne[depart] > '9') {
            if (ligne[depart] != ' ' || depart+1 == longeur) {
                aff_err(nom_fichier_source, nro_ligne, depart+1);
                return 0;
            }
            else return 1 + nb_nbr(nom_fichier_source, nro_ligne, ligne, longeur, depart+1);
        }
        depart++;
    }
    return 1;
}

int* decoupe(string nom_fichier_source, int nro_ligne, string ligne)
{
    int longeur = ligne.length();
    int nb = nb_nbr(nom_fichier_source, nro_ligne, ligne, longeur, 0);
    int* valeurs = (int*)malloc(nb * sizeof(int));
    int curseur = 0;
    for (int i = 0; i < nb-1; i++) {
        int n = 0;
        while (ligne[curseur] != ' ') {
            n = 10 * n + (int)(ligne[curseur] - '0');
            curseur++;
        }
        valeurs[i] = n;
        curseur++;
    }
    int n = 0;
    while (curseur < longeur) {
        n = 10 * n + (int)(ligne[curseur] - '0');
        curseur++;
    }
    valeurs[nb-1] = n;
    return valeurs;
}

int conforme_transition(string nom_fichier_source, int nro_ligne, string ligne, int taille_alphabet)
{
    int longeur = ligne.length();
    if (longeur < 3) {
        aff_err(nom_fichier_source, nro_ligne, 0, "pas assez de caracteres sur cette ligne");
        return 0;
    }
    if (ligne[0] < '0' || ligne[0] > '9') {
        aff_err(nom_fichier_source, nro_ligne, 1);
        return 0;
    }
    int curseur = 1;
    while (ligne[curseur] >= '0' && ligne[curseur] <= '9' && curseur < longeur-1) curseur++;
    if ((ligne[curseur] < 'a' || (int)ligne[curseur] >= (int)('a') + taille_alphabet) && ligne[curseur] != '*') {
        aff_err(nom_fichier_source, nro_ligne, curseur+1, "caractere de transition incorect");
        return 0;
    }
    curseur++;
    while (curseur < longeur) {
        if (ligne[curseur] < '0' || ligne[curseur] > '9') {
            aff_err(nom_fichier_source, nro_ligne, curseur+1);
            return 0;
        }
        curseur++;
    }
    return 1;
}

int* decoupe_transition(string nom_fichier_source, int nro_ligne, string ligne, int nb_etats)
{
    int* transition = (int*)malloc(3 * sizeof(int));
    int n = 0;
    int curseur = 0;
    while (ligne[curseur] >= '0' && ligne[curseur] <= '9') {
        n = 10 * n + (int)(ligne[curseur] - '0');
        curseur++;
    }
    if (n > nb_etats) aff_err(nom_fichier_source, nro_ligne, 0, "transition avec etat de depart inexistant");
    transition[0] = n;
    if (ligne[curseur] == '*') transition[1] = -1;
    else transition[1] = (int)(ligne[curseur] - 'a');
    n = 0;
    curseur++;
    while (curseur < ligne.length()) {
        n = 10 * n + (int)(ligne[curseur] - '0');
        curseur++;
    }
    if (n > nb_etats) aff_err(nom_fichier_source, nro_ligne, curseur, "transition avec etat d'arrivee inexistant");
    transition[2] = n;
    return transition;
}

Automate::Automate(string fichier_source)
{
    nom_fichier_source = fichier_source;
    ifstream fichier(fichier_source.c_str(), ios::in);
    if(fichier)
    {
        string ligne;
        getline(fichier, ligne);
        if (c_un_nb(fichier_source, 1, ligne)) taille_alphabet = conv_nb(ligne);
        getline(fichier, ligne);
        if (c_un_nb(fichier_source, 2, ligne)) nb_etats = conv_nb(ligne);
        getline(fichier, ligne);
        etats_initiaux = decoupe(fichier_source, 3, ligne);
        nb_etats_initiaux = etats_initiaux[0];
        getline(fichier, ligne);
        etats_terminaux = decoupe(fichier_source, 4, ligne);
        nb_etats_terminaux = etats_terminaux[0];
        getline(fichier, ligne);
        if (c_un_nb(fichier_source, 5, ligne)) nb_transitions = conv_nb(ligne);
        transitions = (int**)malloc(nb_transitions * sizeof(int*));
        for (int i = 0; i < nb_transitions; i++) {
            if(!getline(fichier, ligne)) aff_err(fichier_source, i+6, 0, "il manque cette ligne");
            else {
                if (conforme_transition(fichier_source, i+6, ligne, taille_alphabet)) {
                    transitions[i] = decoupe_transition(fichier_source, i+6, ligne, nb_etats);
                }
            }
        }
        fichier.close();
    } else {
        cerr << "Impossible d'ouvrir le fichier \"" << fichier_source << "\"" << endl;
    }
}

int Automate::est_un_automate_asynchrone()
{
    for (int i = 0; i < nb_transitions; i++) {
        if (transition[i][1] == -1) return 1;
    }
    return 0;
}

int Automate::est_un_automate_deterministe() // Sachant qu'il n'est pas asynchrone
{
    if (nb_transitions > nb_etats * taille_alphabet) return 0;
    int** tab = (int**)malloc(nb_etats * sizeof(int*));
    for (int i = 0; i < nb_etats; i++) {
        tab[i] = (int*)malloc(taille_alphabet * sizeof(int));
        for (int j = 0; j < taille_alphabet; j++) tab[i][j] = 0;
    }
    for (int i = 0; i < nb_transitions; i++) {
        if (tab[transitions[i][0]][transitions[i][1]]) return 0;
    }
    return 1;
}

int Automate::est_un_automate_complet() // Sachant qu'il est deterministe
{
    return (nb_transitions == nb_etats * taille_alphabet);
}

Automate::~Automate()
{
    free(etats_initiaux);
    free(etats_terminaux);
    for (int i = 0; i < nb_transitions; i++) free(transitions[i]);
    free(transitions);
}

void Automate::afficher()
{
    cout << taille_alphabet << endl << nb_etats << endl << nb_etats_initiaux;
    for (int i = 1; i <= nb_etats_initiaux; i++) cout << " " << etats_initiaux[i];
    cout << endl << nb_etats_terminaux;
    for (int i = 1; i <= nb_etats_terminaux; i++) cout << " " << etats_terminaux[i];
    cout << endl << nb_transitions << endl;
    for (int i = 0; i < nb_transitions; i++) {
        if (transitions[i][1] == -1) cout << transitions[i][0] << '*' << transitions[i][2] << endl;
        else cout << transitions[i][0] << (char)(transitions[i][1] + (int)'a') << transitions[i][2] << endl;
    }
}
