#include "Automate.h"

void aff_err(string nom_fichier_source, int nro_ligne, int nro_colonne, string message = "")
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
    int nb = nb_nbr(nom_fichier_source, nro_ligne, ligne, longeur, 0) - 1;
    int* valeurs = (int*)malloc((nb-1) * sizeof(int));
    int curseur = 0;
    while(ligne[curseur] != ' ') curseur++;
    curseur++;
    for (int i = 0; i < nb-1; i++) {
        int n = 0;
        while (ligne[curseur] != ' ') {
            n = 10 * n + (int)(ligne[curseur] - '0');
            curseur++;
        }
        valeurs[i] = n;
        curseur++;
    }
    valeurs[nb-1] = 0;
    while (curseur < longeur) {
        valeurs[nb-1] = 10 * valeurs[nb-1] + (int)(ligne[curseur] - '0');
        curseur++;
    }
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
    correct = 1;
    nom_fichier_source = fichier_source;
    ifstream fichier(fichier_source.c_str(), ios::in);
    if(fichier)
    {
        string ligne;
        getline(fichier, ligne);
        if (c_un_nb(fichier_source, 1, ligne)) taille_alphabet = conv_nb(ligne);
        else {correct = 0; return;}
        getline(fichier, ligne);
        if (c_un_nb(fichier_source, 2, ligne)) nb_etats = conv_nb(ligne);
        else {correct = 0; return;}
        getline(fichier, ligne);
        etats_initiaux = decoupe(fichier_source, 3, ligne);
        nb_etats_initiaux = nb_nbr(nom_fichier_source, 3, ligne, ligne.length(), 0) - 1;
        getline(fichier, ligne);
        etats_terminaux = decoupe(fichier_source, 4, ligne);
        nb_etats_terminaux = nb_nbr(nom_fichier_source, 4, ligne, ligne.length(), 0) - 1;
        getline(fichier, ligne);
        if (c_un_nb(fichier_source, 5, ligne)) nb_transitions = conv_nb(ligne);
        else correct = 0;
        transitions = (int**)malloc(nb_transitions * sizeof(int*));
        for (int i = 0; i < nb_transitions; i++) {
            if(!getline(fichier, ligne)) aff_err(fichier_source, i+6, 0, "il manque cette ligne");
            else {
                if (conforme_transition(fichier_source, i+6, ligne, taille_alphabet)) {
                    transitions[i] = decoupe_transition(fichier_source, i+6, ligne, nb_etats);
                } else correct = 0;
            }
        }
        table_transitions = (int***)malloc(nb_etats * sizeof(int**));
        nb_transit = (int**)malloc(nb_etats * sizeof(int*));
        for (int i = 0; i < nb_etats; i++) {
            table_transitions[i] = (int**)malloc((taille_alphabet + 1) * sizeof(int*));
            nb_transit[i] = (int*)malloc((taille_alphabet + 1) * sizeof(int));
        }
        for (int t = 0; t < nb_transitions; t++) {
            if (transitions[t][1] == -1) nb_transit[transitions[t][0]][taille_alphabet]++;
            else nb_transit[transitions[t][0]][transitions[t][1]]++;
        }
        for (int i = 0; i < nb_etats; i++) {
            for (int j = 0; j < taille_alphabet+1; j++) {
                table_transitions[i][j] = (int*)malloc(nb_transit[i][j] * sizeof(int));
                nb_transit[i][j] = 0;
            }
        }
        for (int t = 0; t < nb_transitions; t++) {
            table_transitions[transitions[t][0]][transitions[t][1]][nb_transit[transitions[t][0]][transitions[t][1]]] = transitions[t][2];
            nb_transit[transitions[t][0]][transitions[t][1]]++;
        }
        fichier.close();
    } else {
        cerr << "Impossible d'ouvrir le fichier \"" << fichier_source << "\"" << endl;
        correct = 0;
    }
}

int Automate::est_un_automate_asynchrone()
{
    for (int i = 0; i < nb_transitions; i++) {
        if (transitions[i][1] == -1) return 1;
    }
    return 0;
}

int Automate::est_un_automate_deterministe() // Sachant qu'il n'est pas asynchrone
{
    if (nb_etats_initiaux > 1) return 0;
    for (int e = 0; e < nb_etats; e++) {
        for (int c = 0; c < taille_alphabet; c++) {
            if (nb_transit[e][c] > 1) return 0;
        }
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
    for (int e = 0; e < nb_etats; e++) {
        for (int c = 0; c < taille_alphabet+1; c++) {
            free(table_transitions[e][c]);
        }
        free(table_transitions[e]);
        free(nb_transit[e]);
    }
    free(table_transitions);
    free(nb_transit);
}

void Automate::afficher()
{
    if (!correct) return;
    cout << "------------------------------------" << endl;
    cout << taille_alphabet << endl << nb_etats << endl << nb_etats_initiaux;
    for (int i = 0; i < nb_etats_initiaux; i++) cout << " " << etats_initiaux[i];
    cout << endl << nb_etats_terminaux;
    for (int i = 0; i < nb_etats_terminaux; i++) cout << " " << etats_terminaux[i];
    cout << endl << nb_transitions << endl;
    for (int i = 0; i < nb_transitions; i++) {
        if (transitions[i][1] == -1) cout << transitions[i][0] << '*' << transitions[i][2] << endl;
        else cout << transitions[i][0] << (char)(transitions[i][1] + (int)'a') << transitions[i][2] << endl;
    }
    cout << endl << "nombre d'etats initiaux : " << nb_etats_initiaux << endl;
    for (int i = 0; i < nb_etats_initiaux; i++) cout << "      " << etats_initiaux[i] << endl;
    cout << endl << "nombre d'etats terminaux : " << nb_etats_terminaux << endl;
    for (int i = 0; i < nb_etats_terminaux; i++) cout << "      " << etats_terminaux[i] << endl;
    cout << endl;
    int maxi = 1;
    for (int i = 0; i < nb_etats; i++) {
        for (int j = 0; j < taille_alphabet+1; j++) {
            if (maxi < nb_transit[i][j]) maxi = nb_transit[i][j];
        }
    }
    cout << "    ";
    for (int i = 'a'; i < 'a' + taille_alphabet; i++) {
        cout << (char)(i) << " ";
        for (int j = 1; j < maxi; j++) cout << "  ";
    }
    cout << "*";
    for (int e = 0; e < nb_etats; e++) {
        cout << endl << e;
        if (e < 10) cout << "   ";
        else cout << "  ";
        for (int i = 'a'; i < 'a' + taille_alphabet; i++) {
            for (int j = 0; j < nb_transit[e][i-'a']; j++) {
                cout << table_transitions[e][i-'a'][j];
                if (j < nb_transit[e][i-'a']-1) cout <<",";
            }
            if (nb_transit[e][i-'a'] == 1) {
                cout << " ";
                for (int j = 1; j < maxi; j++) cout << "  ";
            }
            else for (int k = nb_transit[e][i-'a']; k < maxi; k++) cout << "  ";
        }
    }
    cout << endl << "------------------------------------" << endl;
}

