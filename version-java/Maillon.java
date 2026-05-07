public class Maillon {
    
    // Les deux informations d'un monôme
    double coefficient;  // ex: 3.0 dans 3*X^2
    int exposant;        // ex: 2 dans 3*X^2
    
    // Le lien vers le monôme suivant dans la liste
    Maillon suivant;

    // Constructeur : fabrique un maillon avec ses valeurs
    public Maillon(double coefficient, int exposant) {
        this.coefficient = coefficient;
        this.exposant = exposant;
        this.suivant = null; // pas encore de voisin
    }
}