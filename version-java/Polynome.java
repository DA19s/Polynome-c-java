public class Polynome {
    Maillon tete;
    
    // Le texte du polynôme et la position actuelle
    private String texte;
    private int position;

    public Polynome() {
        this.tete = null;
    }


    // MÉTHODES UTILITAIRES
    

    // Donne le caractère actuel sans avancer
    private char caractereActuel() {
        if (position < texte.length())
            return texte.charAt(position);
        return '\0'; // caractère vide = fin du texte
    }

    // Avance d'un caractère
    private void avancer() {
        position++;
    }

    // Ignore les espaces
    private void ignorerEspaces() {
        while (position < texte.length() && texte.charAt(position) == ' ')
            position++;
    }

  
    // PARSE NATUREL
    // ex: 5, 23..
    
    private int parseNaturel() {
        ignorerEspaces();
        String chiffres = ""; // 
        
        // Tant que c'est un chiffre on lit
        while (Character.isDigit(caractereActuel())) {
            chiffres += caractereActuel();
            avancer();
        }

        if (chiffres.isEmpty())
            throw new RuntimeException("Erreur : chiffre attendu à position " + position);

        return Integer.parseInt(chiffres);
    }

    // PARSE NOMBRE
    // ex: 3, 4.5, 123.0
    
    private double parseNombre() {
        ignorerEspaces();
        String nb = "";

        // Partie entière
        while (Character.isDigit(caractereActuel())) {
            nb += caractereActuel();
            avancer();
        }

        // Partie décimale (optionnelle)
        if (caractereActuel() == '.') { 
            nb += '.';
            avancer();
            while (Character.isDigit(caractereActuel())) {
                nb += caractereActuel();
                avancer();
            }
        }

        if (nb.isEmpty())
            throw new RuntimeException("Erreur : nombre attendu à position " + position);

        return Double.parseDouble(nb);
    }

    // PARSE XPUISSANCE
    // ex: X, X^3, X^5
   
    private int parseXpuissance() {
        ignorerEspaces();

        // On attend forcément un X ici
        if (caractereActuel() != 'X')
            throw new RuntimeException("Erreur : X attendu à position " + position);
        avancer();

        // Si y'a un ^ on lit l'exposant
        if (caractereActuel() == '^') {
            avancer();
            return parseNaturel(); 
        }

        // Sinon exposant = 1 (juste X)
        return 1;
    }

    
    // PARSE MONOME
    // ex: 3*X^2, X, 4.5, X^3
    
    private Maillon parseMonome() {
        ignorerEspaces();
        double coeff = 1.0;
        int exp = 0;

        // Cas 1 : commence par un chiffre → nombre*X^n ou juste nombre
        if (Character.isDigit(caractereActuel())) {
            coeff = parseNombre();
            ignorerEspaces();

            if (caractereActuel() == '*') {
                avancer(); // on passe le *
                exp = parseXpuissance();
            }
            // sinon c'est juste un nombre, exposant reste 0
        }
        // Cas 2 : commence par X → X ou X^n (coeff = 1)
        else if (caractereActuel() == 'X') {
            exp = parseXpuissance();
        }

        return new Maillon(coeff, exp);
    }

   
    // PARSE POLYNOME
    // ex: -3*X^2 + 2*X - 5
   
    public void parser(String textePolynome) {
        this.texte = textePolynome;
        this.position = 0;
        this.tete = null;

        ignorerEspaces();

        // Signe global au début du polynôme
        double signeGlobal = 1.0; 
        if (caractereActuel() == '-') {
            signeGlobal = -1.0;
            avancer(); // on continue après le signe pour lire le premier monôme
        }

        // Premier monôme
        Maillon m = parseMonome();
        m.coefficient *= signeGlobal;
        ajouterMonome(m.coefficient, m.exposant);

        // Monômes suivants séparés par + ou -
        while (true) {
            ignorerEspaces();
            char op = caractereActuel();

            if (op != '+' && op != '-') break; // fin du polynôme 
            avancer();

            double signe = (op == '-') ? -1.0 : 1.0;
            Maillon suivant = parseMonome();// on lit le monôme suivant et and we do the same thing as the first one
            suivant.coefficient *= signe;
            ajouterMonome(suivant.coefficient, suivant.exposant);
        }
    }

    // Ajouter un monôme au début (de l'étape 1)
    public void ajouterMonome(double coeff, int exp) {
        Maillon nouveau = new Maillon(coeff, exp);// we insert the new monome at the beginning of the list
        nouveau.suivant = tete; // new monome points to the old head
        tete = nouveau; // head now points to the new monome
    }

    public void afficher() {
    if (tete == null) {
        System.out.println("0");
        return;
    }

    Maillon courant = tete;
    boolean estPremierMonome = true;

    while (courant != null) {
        double coeff = courant.coefficient;
        int exp = courant.exposant;

        // ---- GESTION DU SIGNE ----
        if (estPremierMonome) {
            if (coeff < 0) System.out.print("- ");
            estPremierMonome = false;
        } else {
            if (coeff < 0) System.out.print(" - ");
            else System.out.print(" + ");
        }

        double valeur = Math.abs(coeff); 

        // ---- GESTION DU COEFFICIENT ----
        if (exp == 0) {
            System.out.print(valeur);
        } else if (valeur == 1.0) {
            if (exp == 1) System.out.print("X");
            else System.out.print("X^" + exp);
        } else {
            if (exp == 1) System.out.print(valeur + "*X");
            else System.out.print(valeur + "*X^" + exp);
        }

        courant = courant.suivant;
    }

    System.out.println();
    
}
// Fonction 1 : insère un maillon au bon endroit dans la liste triée
private Maillon insererTrie(Maillon sorted, Maillon nouveau) {

    // Cas 1 : la liste est vide OU le nouveau a le plus grand exposant
    // → il devient le premier
    if (sorted == null || sorted.exposant < nouveau.exposant) {
        nouveau.suivant = sorted;
        return nouveau; 
    }

    // Cas 2 : même exposant → on additionne les coefficients
    // ex: 3*X^2 et 2*X^2 → 5*X^2
    if (sorted.exposant == nouveau.exposant) {
        sorted.coefficient += nouveau.coefficient;
        return sorted;
    }

    // Cas 3 : on cherche la bonne position dans la liste
    Maillon actuel = sorted;

    while (actuel.suivant != null && actuel.suivant.exposant > nouveau.exposant) {
        actuel = actuel.suivant; // on avance tant que l'exposant suivant est plus grand
    }

    // On vérifie encore si même exposant au point d'insertion
    if (actuel.suivant != null && actuel.suivant.exposant == nouveau.exposant) {
        actuel.suivant.coefficient += nouveau.coefficient;
        return sorted;
    }

    // On insère le nouveau maillon à la bonne place
    nouveau.suivant = actuel.suivant; 
    actuel.suivant = nouveau;

    return sorted;
}

// Fonction 2 : trie tout le polynôme
public void trierParDegreDecroissant() {
    Maillon sorted = null;  // liste triée vide au départ
    Maillon temp = tete;    // on le met au debut pour le parcout de la liste

    while (temp != null) {
        Maillon prochainMaillon = temp.suivant; // on sauvegarde le suivant
        temp.suivant = null;                    // on détache le maillon courant
        sorted = insererTrie(sorted, temp);     // on l'insère au bon endroit
        temp = prochainMaillon;                 // on passe au suivant
    }

    tete = sorted; // la liste triée devient notre polynôme
}
public double evaluer(double x) {
    Maillon courant = tete;
    double resultat = 0.0;

    while (courant != null) {
        // coefficient * x^exposant
        resultat += courant.coefficient * Math.pow(x, courant.exposant);
        courant = courant.suivant;
    }

    return resultat;
}
}