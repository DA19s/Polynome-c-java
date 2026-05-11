#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#define N 10

typedef struct Maillon {
    double coefficient;
    int exposant;
    struct Maillon* suivant;
    //question 7
    struct Maillon* general;
    int utile;
} Maillon;

typedef Maillon* POINTEUR;

char c;

POINTEUR tousLesMaillons;

POINTEUR polyUtile[N];

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
    m->utile = 0;
    m->general = tousLesMaillons;
    tousLesMaillons = m;

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

POINTEUR nettoyer(POINTEUR p) {
    POINTEUR temp = p;
    POINTEUR resultat = NULL;

    while (temp != NULL) {
        if (temp->coefficient != 0) {
            POINTEUR m = malloc(sizeof(Maillon));
            m->coefficient = temp->coefficient;
            m->exposant = temp->exposant;
            m->suivant = NULL;
            m->utile = 0;
            m->general = tousLesMaillons;
            tousLesMaillons = m;
            resultat = trier(resultat, m);
        }

        temp = temp->suivant;
    }

    return resultat;
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
    tete = nettoyer(tete);
    return tete;
}

void afficher(POINTEUR p) {
    if (p==NULL) {
        printf("0\n");
        return;
    }
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

double eval(POINTEUR p, double x) {
    POINTEUR temp = p;
    double resultat=0;
    
    while (temp!=NULL) {
        resultat += (temp->coefficient * pow(x, temp->exposant));
        temp = temp->suivant;
    }

    return resultat;
}

POINTEUR plus(POINTEUR a, POINTEUR b) {
    POINTEUR temp_a = a;
    POINTEUR temp_b = b;
    POINTEUR resultat = malloc(sizeof(Maillon));
    resultat->coefficient = temp_a->coefficient;
    resultat->exposant = temp_a->exposant;
    resultat->utile = 0;
    resultat->general = tousLesMaillons;
    tousLesMaillons = resultat;
    POINTEUR courant = resultat;
    courant->suivant = NULL;

    while (temp_a->suivant != NULL || temp_b != NULL) {
        if (temp_a->suivant != NULL) {
            
            courant->suivant = malloc(sizeof(Maillon));
            courant->suivant->coefficient = temp_a->suivant->coefficient;
            courant->suivant->exposant = temp_a->suivant->exposant;
            courant->suivant->utile = 0;
            courant->suivant->general = tousLesMaillons;
            tousLesMaillons = courant->suivant;

            courant = courant->suivant;
            temp_a = temp_a->suivant;
            courant->suivant = NULL;
        }
        else {
            courant->suivant = malloc(sizeof(Maillon));
            courant->suivant->coefficient = temp_b->coefficient;
            courant->suivant->exposant = temp_b->exposant;
            courant->suivant->utile = 0;
            courant->suivant->general = tousLesMaillons;
            tousLesMaillons = courant->suivant;

            courant = courant->suivant;
            temp_b = temp_b->suivant;  
            courant->suivant = NULL;
        }
    }

    return nettoyer(trier_polynome(resultat));
}

POINTEUR plus_rec(POINTEUR a, POINTEUR b) {
    if (a == NULL) return b;
    if (b == NULL) return a;

    if (a->exposant > b->exposant) {
        POINTEUR resultat = malloc(sizeof(Maillon));
        resultat->coefficient = a->coefficient;
        resultat->exposant = a->exposant;
        resultat->utile = 0;
        resultat->general = tousLesMaillons;
        tousLesMaillons = resultat;

        resultat->suivant = plus_rec(a->suivant,b);
        return nettoyer(resultat);
    }
    else if (a->exposant < b->exposant) {
        POINTEUR resultat = malloc(sizeof(Maillon));
        resultat->coefficient = b->coefficient;
        resultat->exposant = b->exposant;
        resultat->utile = 0;
        resultat->general = tousLesMaillons;
        tousLesMaillons = resultat;

        resultat->suivant = plus_rec(a,b->suivant);
        return nettoyer(resultat);
    }
    else if (a->exposant == b->exposant) {
        POINTEUR resultat = malloc(sizeof(Maillon));
        resultat->coefficient = (a->coefficient + b->coefficient);
        resultat->exposant = a->exposant;
        resultat->utile = 0;
        resultat->general = tousLesMaillons;
        tousLesMaillons = resultat;

        resultat->suivant = plus_rec(a->suivant,b->suivant);
        return nettoyer(resultat);
    }
}

POINTEUR moins(POINTEUR a, POINTEUR b) {
    POINTEUR temp_a = a;
    POINTEUR temp_b = b;
    POINTEUR resultat = malloc(sizeof(Maillon));
    resultat->coefficient = temp_a->coefficient;
    resultat->exposant = temp_a->exposant;
    resultat->utile = 0;
    resultat->general = tousLesMaillons;
    tousLesMaillons = resultat;
    POINTEUR courant = resultat;
    courant->suivant = NULL;

    while (temp_a->suivant != NULL || temp_b != NULL) {
        if (temp_a->suivant != NULL) {
            
            courant->suivant = malloc(sizeof(Maillon));
            courant->suivant->coefficient = temp_a->suivant->coefficient;
            courant->suivant->exposant = temp_a->suivant->exposant;
            courant->suivant->utile = 0;
            courant->suivant->general = tousLesMaillons;
            tousLesMaillons = courant->suivant;

            courant = courant->suivant;
            temp_a = temp_a->suivant;
            courant->suivant = NULL;
        }
        else {
            courant->suivant = malloc(sizeof(Maillon));
            courant->suivant->coefficient = -(temp_b->coefficient);
            courant->suivant->exposant = temp_b->exposant;
            courant->suivant->utile = 0;
            courant->suivant->general = tousLesMaillons;
            tousLesMaillons = courant->suivant;

            courant = courant->suivant;
            temp_b = temp_b->suivant;  
            courant->suivant = NULL;
        }
    }

    return nettoyer(trier_polynome(resultat));
}

POINTEUR negation(POINTEUR p) {

    if (p!=NULL) {
        POINTEUR temp = malloc(sizeof(Maillon));
        temp->coefficient = -p->coefficient;
        temp->exposant = p->exposant;
        temp->utile = 0;
        temp->general = tousLesMaillons;
        tousLesMaillons = temp;

        temp->suivant = negation(p->suivant);
        return temp;
    }

    return NULL;
}

POINTEUR moins_rec(POINTEUR a, POINTEUR b) {
    POINTEUR temp_b = b;
    if (a == NULL) return negation(temp_b);
    if (b == NULL) return a;

    if (a->exposant > b->exposant) {
        POINTEUR resultat = malloc(sizeof(Maillon));
        resultat->coefficient = a->coefficient;
        resultat->exposant = a->exposant;
        resultat->utile = 0;
        resultat->general = tousLesMaillons;
        tousLesMaillons = resultat;

        resultat->suivant = moins_rec(a->suivant,b);
        return nettoyer(resultat);
    }
    else if (a->exposant < b->exposant) {
        POINTEUR resultat = malloc(sizeof(Maillon));
        resultat->coefficient = -b->coefficient;
        resultat->exposant = b->exposant;
        resultat->utile = 0;
        resultat->general = tousLesMaillons;
        tousLesMaillons = resultat;

        resultat->suivant = moins_rec(a,b->suivant);
        return nettoyer(resultat);
    }
    else if (a->exposant == b->exposant) {
        POINTEUR resultat = malloc(sizeof(Maillon));
        resultat->coefficient = (a->coefficient - b->coefficient);
        resultat->exposant = a->exposant;
        resultat->utile = 0;
        resultat->general = tousLesMaillons;
        tousLesMaillons = resultat;

        resultat->suivant = moins_rec(a->suivant,b->suivant);
        return nettoyer(resultat);
    }
}

POINTEUR fois(POINTEUR a, POINTEUR b) {
    POINTEUR temp_a = a;
    POINTEUR temp_b = b;
    POINTEUR resultat = NULL;
    POINTEUR courant = resultat;

    while (temp_a != NULL) {
        while (temp_b != NULL) {
            courant = malloc(sizeof(Maillon));
            courant->coefficient = (temp_a->coefficient * temp_b->coefficient);
            courant->exposant = (temp_a->exposant + temp_b->exposant);
            courant->utile = 0;
            courant->general = tousLesMaillons;
            tousLesMaillons = courant;

            resultat = trier(resultat, courant);
            temp_b = temp_b->suivant;
        }
        if (temp_a->suivant != NULL) temp_b = b;
        temp_a = temp_a->suivant;
    }

    return nettoyer(trier_polynome(resultat));
}

POINTEUR quotient(POINTEUR a, POINTEUR  b, POINTEUR *reste) {
    POINTEUR temp_a = a;
    POINTEUR temp_b = b;
    POINTEUR resultat = NULL;
    POINTEUR courant = resultat;

    temp_a = trier_polynome(temp_a);
    temp_b = trier_polynome(temp_b);

    POINTEUR dividende = temp_a;

    while (dividende!=NULL && dividende->exposant >= temp_b->exposant) {
        courant = malloc(sizeof(Maillon));
        courant->coefficient = (dividende->coefficient / temp_b->coefficient);
        courant->exposant = (dividende->exposant - temp_b->exposant);
        courant->utile = 0;
        courant->general = tousLesMaillons;
        tousLesMaillons = courant;

        resultat = trier(resultat, courant);

        dividende = nettoyer(moins(dividende, fois(courant, temp_b)));
    }

    *reste = dividende;

    return nettoyer(resultat);
}

void marquer_utile() {
    for (int i = 0; i < N; i++) {
        POINTEUR temp = polyUtile[i];
        while (temp != NULL) {
            temp->utile = 1;
            temp = temp->suivant;
        } 
    }
}

void gerer_inutile() {
    POINTEUR temp = tousLesMaillons;
    POINTEUR precedent = NULL;
    while(temp!=NULL) {
        if (temp->utile == 1) {
            temp->utile = 0;
            precedent = temp;
            temp = temp->general;
        }
        else {
            if (precedent == NULL) {
                tousLesMaillons = temp->general;
                free(temp);
                temp = tousLesMaillons;
            }
            else {
                precedent->general = temp->general;
                free(temp);
                temp = precedent->general;
            }
        }
        
    }
}

int nombre_de_maillon() {
    POINTEUR temp = tousLesMaillons;
    int c=0;

    while (temp!=NULL) {
        c++;
        temp = temp->general;
    }
    return c;
}


int main() {
    lire();
    ignorer_espaces();
    POINTEUR p = lire_polynome();
    printf("Syntaxiquement correct!\n");
    afficher(p);
    printf("\n=========================================================\n");
    
    lire();
    ignorer_espaces();
    POINTEUR p2 = lire_polynome();
    printf("Syntaxiquement correct!\n");
    afficher(p2);
    printf("\n=========================================================\n");

    printf("Nombre total de maillon: %d", nombre_de_maillon());
    printf("\n=========================================================\n");
    p = trier_polynome(p);
    p2 = trier_polynome(p2);

    polyUtile[0] = p;
    polyUtile[1] = p2;

    printf("\nPolynomes triés:\n");
    printf("P: "); afficher(p);
    printf("P2: "); afficher(p2);
    printf("\n=========================================================\n");

    printf("Pour une valeur de X = 2:\n Résultat P = %g\n", eval(p, 2));
    printf("Pour une valeur de X = 2:\n Résultat P2 = %g\n", eval(p2, 2));
    printf("\n=========================================================\n");

    POINTEUR reste = NULL;
    printf("P + P2 = "); afficher(plus(p,p2));
    printf("P - P2 = "); afficher(moins(p,p2));
    printf("Recursif: P + P2 = "); afficher(plus_rec(p,p2));
    printf("Recursif: P - P2 = "); afficher(moins_rec(p,p2));
    printf("P x P2 = "); afficher(fois(p,p2));
    printf("P / P2 = "); afficher(quotient(p,p2,&reste)); 
    printf("Le reste est: "); afficher(reste);

    polyUtile[2] = reste;

    printf("\n=========================================================\n");
    printf("Nombre total de maillon: %d", nombre_de_maillon());
    printf("\n=========================================================\n");

    printf("La femme de menage passe:");
    marquer_utile();
    gerer_inutile();

        printf("Nombre total de maillon: %d", nombre_de_maillon());
    printf("\n=========================================================\n");
}