#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Maillon {
    double coefficient;
    int exposant;
    struct Maillon* suivant;
} Maillon;

typedef Maillon* POINTEUR;

char c;

void lire() {
    c = getchar();
}

void ignorer_espaces() {
    while (c == ' ') {
        lire();
    }
}

void erreur() {
    printf("Erreur syntaxique\n");
    exit(1);
}

int lire_naturel() {
    if (!isdigit(c)) erreur();
    int valeur = 0;
    while (isdigit(c)) {
        valeur = valeur * 10 + (c - '0');
        lire();
    }
    return valeur;
}

double lire_nombre() {
    double partie_entiere = lire_naturel();
    if (c!='.') return partie_entiere;
    lire();
    if (!isdigit(c)) erreur();
    double partie_decimale = 0;
    int diviseur = 10;
    while (isdigit(c)) {
        partie_decimale = partie_decimale + ((double)(c - '0')/diviseur);
        diviseur = diviseur * 10;
        lire();
    }
    double valeur = partie_entiere + partie_decimale;
    return valeur;
}

int lire_xpuissance() {
    if (c!='X') erreur();
    lire();
    if (c!='^') return 1;
    lire();
    int exposant = lire_naturel();
    return exposant;
}

POINTEUR lire_monome() {
    POINTEUR m = malloc(sizeof(Maillon));
    if (c!='X' && !isdigit(c)) erreur();
    if (c=='X') {
        m->coefficient = 1;
        m->exposant = lire_xpuissance();
        m->suivant = NULL;
    }
    else if (isdigit(c)) {
        double coefficient = lire_nombre();
        ignorer_espaces();
        if (c=='*'){
            lire();
            ignorer_espaces();
            if (c!='X') erreur();
            m->coefficient = coefficient;
            m->exposant = lire_xpuissance();
            m->suivant = NULL;
        }
        else {
            m->coefficient = coefficient;
            m->exposant = 0;
            m->suivant = NULL;
        }
    }
    return m;
}

POINTEUR lire_polynome() {
    int signe = 1;
    if (c!='-' && !isdigit(c) && c!='X') erreur();
    if (c=='-') {
        signe = -1;
        lire();
        ignorer_espaces();
    }
    ignorer_espaces();
    POINTEUR tete = lire_monome();
    tete->coefficient *=signe;
    ignorer_espaces();
    POINTEUR courant = tete;
    while (c=='+' || c=='-') {
        if (c=='-') {
            signe = -1;
        }
        else {
            signe = 1;
        }
        lire();
        ignorer_espaces();
        courant->suivant = lire_monome();
        courant->suivant->coefficient *= signe;
        courant = courant->suivant;
        ignorer_espaces();
    }
    return tete;
}

int main() {
    lire();
    lire_polynome();
    printf("Syntaxiquement correct!\n");
}