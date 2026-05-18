public class Maillon {

    double coefficient;
    int exposant;
    Maillon suivant;

    // ---- Champs du garbage collector (étape 7) ----
    static Maillon tousLesMaillons = null; // tête de la liste globale de TOUS les maillons alloués
    Maillon general;  // chaîne ce maillon à la liste globale (indépendant de 'suivant')
    boolean utile;    // true = maillon encore utilisé par un polynôme actif

    public Maillon(double coefficient, int exposant) {
        this.coefficient = coefficient;
        this.exposant = exposant;
        this.suivant = null;
        this.utile = false;
        // Enregistrement automatique dans la liste globale dès l'allocation
        this.general = tousLesMaillons;
        tousLesMaillons = this;
    }
}
