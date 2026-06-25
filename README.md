# Polynome — Calculateur de Polynômes (C & Java)

Projet académique réalisé dans le cadre du cours d'**Algorithmique et Programmation Avancée**.  
Il implémente un calculateur complet de polynômes à une variable en **C** et en **Java**, avec parsing, opérations arithmétiques, division euclidienne, tri et un **ramasse-miettes manuel** (mark-and-sweep).

---

## Membres du groupe

| Nom | Prénom |
|-----|--------|
| DIOUF | Mohamed |
| FALL | Ramatoulaye |
| WADE | Mohamed |
| NDIAYE | Bintou |
| DAN AZOUMI | Ibrahim |

---

## Fonctionnalités

- **Parsing** d'expressions polynomiales textuelles (ex. `3*X^2 + 2*X - 5`)
- **Tri** des termes par degré décroissant avec fusion des monômes semblables
- **Opérations arithmétiques** : addition, soustraction, multiplication
- **Division euclidienne** : retourne le quotient et le reste
- **Évaluation** d'un polynôme en un point réel
- **Variantes récursives** de l'addition et de la soustraction
- **Ramasse-miettes manuel** (mark-and-sweep) pour la gestion mémoire

---

## Structure du projet

```
Polynome-c-java/
├── polynome.c          # Implémentation complète en C
└── version-java/       # Port Java du projet
    ├── Main.java       # Point d'entrée — saisie et démonstration
    ├── Polynome.java   # Classe principale (structure, opérations, GC)
    └── Maillon.java    # Nœud de la liste chaînée (terme du polynôme)
```

---

## Format d'entrée

Les polynômes sont saisis sous forme de chaîne de caractères, avec la syntaxe suivante :

```
3*X^2 + 2*X - 5
X^3 - 2*X^2 + 1
4.5
-X
```

**Règles :**
- Le coefficient et la variable sont séparés par `*`
- L'exposant est introduit par `^`
- Les termes sont séparés par `+` ou `-`
- Les espaces sont autorisés entre les termes
- Les coefficients peuvent être entiers ou décimaux

---

## Lancer le projet

### Version C

```bash
gcc -o polynome polynome.c -lm
./polynome
```

Entrer les deux polynômes lorsque demandé.

### Version Java

```bash
cd version-java
javac *.java
java Main
```

Entrer les deux polynômes lorsque demandé (lecture via `Scanner`).

---

## Exemple d'exécution

```
Entrez p1 : 3*X^2 + 2*X - 5
Entrez p2 : X^3 - 2*X^2 + 1

p1        = 3.0*X^2 + 2.0*X - 5.0
p2        = X^3 - 2.0*X^2 + 1.0

p1(2)     = 11.0

p1 + p2   = X^3 + X^2 + 2.0*X - 4.0
p1 - p2   = -X^3 + 5.0*X^2 + 2.0*X - 6.0
p1 * p2   = 3.0*X^5 - 4.0*X^4 - 7.0*X^3 + 7.0*X^2 + 2.0*X - 5.0
p1 / p2   → quotient : 3.0   reste : 8.0*X - 8.0
```

---

## Architecture interne

### Liste chaînée de monômes

Chaque polynôme est représenté par une **liste chaînée de `Maillon`** (nœuds), où chaque nœud stocke un terme :

```
Maillon {
    double coefficient
    int    exposant
    Maillon suivant      // prochain terme du polynôme
    Maillon general      // chaîne globale (pour le GC)
    boolean utile        // bit de marquage (GC)
}
```

Les termes sont toujours maintenus triés par **degré décroissant**.

### Ramasse-miettes (Mark-and-Sweep)

Un GC manuel à deux phases gère les nœuds temporaires créés lors des opérations :

1. **Marquage** (`marquer`) — parcourt tous les polynômes déclarés « utiles » et marque leurs nœuds.
2. **Balayage** (`balayer`) — libère tous les nœuds non marqués et reconstruit la liste globale.

```java
Polynome.enregistrer(p1, p2, ...); // déclarer les polynômes à conserver
Polynome.recycler();               // déclencher le GC
```

---

## Algorithmes clés

| Algorithme | Complexité | Usage |
|------------|-----------|-------|
| Tri par insertion | O(n²) | Tri des termes + fusion des doublons |
| Fusion de listes triées | O(n + m) | Addition / soustraction |
| Multiplication naïve | O(n × m) | Multiplication de polynômes |
| Division euclidienne | O(n²) approx. | Quotient et reste |
| Mark-and-Sweep | O(k·n + total) | Libération mémoire |

---

## Cours

**Algorithmique et Programmation Avancée**  
Implémentation réalisée en **C** (version initiale) puis portée en **Java**.
