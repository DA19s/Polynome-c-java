# Le programme Java : manipulation de polynômes

## C'est quoi un polynôme ici ?

Un polynôme comme `3*X^2 + X - 5` est une suite de **monômes**.
Chaque monôme a deux informations : un **coefficient** et un **exposant**.

| Monôme | Coefficient | Exposant |
|--------|-------------|----------|
| `3*X^2` | 3.0 | 2 |
| `X` | 1.0 | 1 |
| `-5` | -5.0 | 0 |

---

## Comment c'est stocké en mémoire ?

On utilise une **liste chaînée** : chaque monôme est une boîte (`Maillon`) qui contient ses données et un lien vers le monôme suivant.

```
tete → [3.0 | exp:2 | →] → [1.0 | exp:1 | →] → [-5.0 | exp:0 | null]
          3*X²                    X                    -5
```

### Les 3 fichiers

| Fichier | Rôle |
|---------|------|
| `Maillon.java` | La boîte : stocke `coefficient`, `exposant`, `suivant`, et les champs du GC |
| `Polynome.java` | Toute la logique : lire, trier, afficher, calculer, GC |
| `Main.java` | Point d'entrée : lit deux polynômes depuis le clavier et teste les opérations |

---

## Ce que fait `Polynome.java` étape par étape

### 1. Lire un texte → construire la liste (`parser`)

On donne la chaîne `"3*X^2 + X - 5"` et le programme la lit caractère par caractère pour construire la liste chaînée.

Sous-fonctions du parser :
- `parseNaturel()` — lit un entier (`42`)
- `parseNombre()` — lit un décimal (`3.14`)
- `parseXpuissance()` — lit `X` ou `X^n` et retourne l'exposant
- `parseMonome()` — assemble un monôme complet (`3*X^2`, `X`, `-5`)
- `parser(texte)` — lit tout le polynôme et construit la liste

> La liste est construite **en tête** : le dernier monôme lu se retrouve en premier. Le tri corrige ça.

---

### 2. Trier par degré décroissant (`trierParDegreDecroissant`)

Après le parser, les monômes sont dans l'ordre inverse. On les **trie par exposant décroissant** (le terme de plus haut degré en premier) avec un **tri par insertion**.

Pour chaque maillon, `insererTrie()` le place au bon endroit dans la liste triée. Si un maillon de **même exposant** existe déjà, les coefficients sont **additionnés** (ex: `X + 2*X → 3*X`).

```
Avant tri : [-5 | X | 3*X²]
Après tri : [3*X² | X | -5]
```

---

### 3. Afficher (`afficher`)

Parcourt la liste et affiche chaque monôme avec les règles typographiques :

| Cas | Résultat affiché |
|-----|-----------------|
| Premier monôme positif | `3*X^2` (pas `+ 3*X^2`) |
| Coefficient = 1, exposant ≥ 2 | `X^2` (pas `1*X^2`) |
| Coefficient = -1, exposant = 1 | `- X` (pas `- 1*X`) |
| Exposant = 0 | `-5.0` (pas `-5.0*X^0`) |
| Monôme négatif suivant | ` - 3*X` (pas `+ -3*X`) |

---

### 4. Évaluer (`evaluer`)

Calcule la valeur numérique du polynôme pour une valeur `x` donnée.

```
p(x) = 3*x² + x - 5
p(2) = 3*4 + 2 - 5 = 9
```

Parcourt la liste et accumule : `coefficient * x^exposant` pour chaque monôme.

---

### 5. Addition et soustraction (`plus`, `moins`)

Les deux listes étant déjà triées, on les **fusionne** simultanément comme dans le tri-fusion :

- exposant de a > exposant de b → on prend le monôme de a, on avance dans a
- exposant de b > exposant de a → on prend le monôme de b, on avance dans b
- même exposant → on **additionne** les coefficients (et on ignore si le résultat est 0)

```
p1 = 3X² +  X  - 5
p2 =  X² - 2X  + 1
          ↓
   = 4X² -  X  - 4
```

> Règle importante du sujet : on ne modifie jamais un polynôme existant. On alloue toujours de nouveaux maillons.

---

### 6. Multiplication (`fois`)

**Double boucle** : chaque monôme de p1 est multiplié par chaque monôme de p2.

```
(3X² + X - 5) × (X² - 2X + 1)

3X² × X²  = 3X⁴       3X² × -2X = -6X³     3X² × 1 = 3X²
 X  × X²  =  X³        X  × -2X = -2X²      X   × 1 =  X
-5  × X²  = -5X²      -5  × -2X =  10X     -5   × 1 = -5

Regroupement :  3X⁴ - 5X³ - 4X² + 11X - 5
```

Les termes de même exposant sont **fusionnés automatiquement** via `insererTrie`.

---

### 7. Division euclidienne (`quotient`)

Même principe que la division de nombres entiers. On cherche Q et R tels que :
`this = b × Q + R`, avec deg(R) < deg(b)

À chaque tour :
1. On divise le terme dominant de R par le terme dominant de b → donne un terme `t`
2. On ajoute `t` au quotient
3. On soustrait `t × b` du reste R
4. On recommence jusqu'à ce que deg(R) < deg(b)

```
(X² - 1) ÷ (X - 1)

Tour 1 : X²/X = X      →  Q = X,     R = (X²-1) - X(X-1) = X-1
Tour 2 : X/X  = 1      →  Q = X+1,   R = (X-1) - 1(X-1)  = 0

Résultat : quotient = X+1,  reste = 0  ✓
```

---

### 8. Garbage collector manuel (`recycler`)

Chaque opération (`plus`, `fois`, etc.) crée des maillons temporaires qui ne sont plus utiles après le calcul. Le GC les détecte et les libère.

#### Comment `Maillon` s'enregistre automatiquement

Chaque `new Maillon(...)` s'ajoute tout seul à une liste globale `tousLesMaillons` via le champ `general` :

```
tousLesMaillons → [maillon_temp | general→] → [p1_X² | general→] → [p1_X | general→] → ...
                    (résidu de fois)              (polynôme actif)
```

Le champ `general` est indépendant de `suivant` — il sert uniquement au GC.

#### Phase 1 — Marquer (`marquer`)

On parcourt les polynômes enregistrés dans `polyUtile` (ceux déclarés "en vie" via `enregistrer(p)`) et on met `utile = true` sur leurs maillons.

```
p1 et p2 enregistrés → leurs 6 maillons sont marqués utile = true
les maillons temporaires restent à utile = false
```

#### Phase 2 — Balayer (`balayer`)

On parcourt toute la liste `tousLesMaillons` :
- `utile == true` → maillon utile : on le **garde**, on remet `utile = false` (reset pour le prochain cycle)
- `utile == false` → maillon inutile : on le **jette** (il sort de `tousLesMaillons`)

```
[GC] Avant : 25 maillons en mémoire
[GC] 19 maillon(s) libéré(s)
[GC] Après : 6 maillons en mémoire   ← 3 pour p1 + 3 pour p2
```

---

### 9. Versions récursives de `plus` et `moins` (`plusRecursif`, `moinsRecursif`)

Même logique que la version itérative, mais sans boucle `while` : la fonction s'appelle elle-même sur le reste de la liste.

```
fusionnerRecursif(a, b)
│
├── a et b vides         →  null
├── a vide               →  recopie b jusqu'à la fin
├── b vide               →  recopie a jusqu'à la fin
├── a.exp > b.exp        →  nœud(a) + appel récursif(a.suivant, b)
├── b.exp > a.exp        →  nœud(b) + appel récursif(a, b.suivant)
└── même exposant        →  nœud(a+b) + appel récursif(a.suivant, b.suivant)
                            (si la somme = 0, on saute le nœud et on retourne directement la suite)
```

La récursion est naturelle ici car une liste chaînée est elle-même une structure récursive : c'est soit `null`, soit un nœud suivi d'une autre liste.
