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
        else if (c=='X') erreur();
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

// TRI PAR INSERTION VERY VERY HARR!!!!
POINTEUR trier (POINTEUR sorted, POINTEUR nouveau_maillon) {
    if (sorted==NULL || sorted->exposant < nouveau_maillon->exposant) {
        nouveau_maillon->suivant = sorted;
        return nouveau_maillon;
    }

    if (sorted != NULL && sorted->exposant == nouveau_maillon->exposant) {
        sorted->coefficient += nouveau_maillon->coefficient;
        return sorted;
    }

    POINTEUR actuel = sorted;

    while (actuel->suivant != NULL && actuel->suivant->exposant > nouveau_maillon->exposant) {
        actuel = actuel->suivant;
    }

    if (actuel->suivant != NULL && actuel->suivant->exposant == nouveau_maillon->exposant) {
        actuel->suivant->coefficient += nouveau_maillon->coefficient;
        return sorted;
    }

    nouveau_maillon->suivant = actuel->suivant;
    actuel->suivant = nouveau_maillon;

    return sorted;
}

POINTEUR trier_polynome (POINTEUR p) {
    POINTEUR sorted = NULL;
    POINTEUR temp = p;

    while (temp!=NULL) {
        POINTEUR prochain_maillon = temp->suivant;
        temp->suivant = NULL;
        sorted = trier(sorted, temp);
        temp = prochain_maillon;
    }

    return sorted;
}

void afficher(POINTEUR p) {
    POINTEUR temp = p;
    int premier = 1;
    while (temp!=NULL) {
        if (temp->coefficient < 0) {
            if (temp->coefficient == -1) {
                if (temp->exposant == 0) {
                    printf(" - %g", -temp->coefficient);
                }
                else if (temp->exposant == 1) {
                    printf(" - X");
                }
                else {
                    printf(" - X^%d", temp->exposant);
                }
            }
            else {
                if (temp->exposant == 0) {
                    printf(" - %g", -temp->coefficient);
                }
                else if (temp->exposant == 1) {
                    printf(" - %gX", -temp->coefficient);
                }
                else {
                    printf(" - %gX^%d", -temp->coefficient, temp->exposant);
                }
            }
        }
        else {
            if (temp->coefficient == 1) {
                if (temp->exposant == 0) {
                    if (premier) printf("%g", temp->coefficient);
                    else printf(" + %g", temp->coefficient);
                }
                else if (temp->exposant == 1) {
                    if (premier) printf("X");
                    else printf(" + X");
                }
                else {
                    if (premier) printf("X^%d", temp->exposant);
                    else printf(" + X^%d", temp->exposant);
                }
            }
            else {
                if (temp->exposant == 0) {
                    if (premier) printf("%g", temp->coefficient);
                    else printf(" + %g", temp->coefficient);
                }
                else if (temp->exposant == 1) {
                    if (premier) printf("%gX", temp->coefficient);
                    else printf(" + %gX", temp->coefficient);
                }
                else {
                    if (premier) printf("%gX^%d", temp->coefficient, temp->exposant);
                    else printf(" + %gX^%d", temp->coefficient, temp->exposant);
                }
            }
        }
        if (premier) premier = 0;
        temp = temp->suivant;
    }
    printf("\n");
}


int main() {
    lire();
    ignorer_espaces();
    POINTEUR p = lire_polynome();
    printf("Syntaxiquement correct!\n");
    afficher(p);
    p = trier_polynome(p);
    printf("\nPolynome triée:\n");
    afficher(p);
}