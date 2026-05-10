public class Main {
    public static void main(String[] args) {

        Polynome p1 = new Polynome();
        p1.parser("3*X^2 + X - 5");
        p1.trierParDegreDecroissant();

        Polynome p2 = new Polynome();
        p2.parser("X^2 - 2*X + 1");
        p2.trierParDegreDecroissant();

        System.out.print("p1 = "); p1.afficher();
        System.out.print("p2 = "); p2.afficher();

        System.out.println("\n-- Itératif (étape 6) --");
        System.out.print("p1 + p2 = "); p1.plus(p2).afficher();
        System.out.print("p1 - p2 = "); p1.moins(p2).afficher();

        System.out.println("\n-- Récursif (étape 8) --");
        System.out.print("p1 + p2 = "); p1.plusRecursif(p2).afficher();
        System.out.print("p1 - p2 = "); p1.moinsRecursif(p2).afficher();

        // Cas particulier : termes qui s'annulent complètement
        System.out.println("\n-- Annulation complète --");
        System.out.print("p1 - p1 = "); p1.moinsRecursif(p1).afficher();
    }
}
