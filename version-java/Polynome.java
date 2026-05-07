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

    // ==============
    // PARSE NATUREL
    // ex: 5, 23, 100
    // ==============
    private int parseNaturel() {
        ignorerEspaces();
        String chiffres = "";
        
        // Tant que c'est un chiffre on lit
        while (Character.isDigit(caractereActuel())) {
            chiffres += caractereActuel();
            avancer();
        }

        if (chiffres.isEmpty())
            throw new RuntimeException("Erreur : chiffre attendu à position " + position);

        return Integer.parseInt(chiffres);
    }

    // ==============
    // PARSE NOMBRE
    // ex: 3, 4.5, 123.0
    // ==============
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

    // ====================
    // PARSE XPUISSANCE
    // ex: X, X^3, X^5
    // ====================
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

    // ==============
    // PARSE MONOME
    // ex: 3*X^2, X, 4.5, X^3
    // ==============
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

    // ====================
    // PARSE POLYNOME
    // ex: -3*X^2 + 2*X - 5
    // ====================
    public void parser(String textePolynome) {
        this.texte = textePolynome;
        this.position = 0;
        this.tete = null;

        ignorerEspaces();

        // Signe global au début (optionnel)
        double signeGlobal = 1.0;
        if (caractereActuel() == '-') {
            signeGlobal = -1.0;
            avancer();
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
            Maillon suivant = parseMonome();
            suivant.coefficient *= signe;
            ajouterMonome(suivant.coefficient, suivant.exposant);
        }
    }

    // Ajouter un monôme au début (de l'étape 1)
    public void ajouterMonome(double coeff, int exp) {
        Maillon nouveau = new Maillon(coeff, exp);
        nouveau.suivant = tete;
        tete = nouveau;
    }
}