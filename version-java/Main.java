public class Main {
    public static void main(String[] args) {
        Polynome p = new Polynome();
        p.parser("-4.5*X^2 + 2*X + 3");
        p.trierParDegreDecroissant();

        System.out.println("Polynôme :");
        p.afficher();

        double x = 2.0;
        double resultat = p.evaluer(x);
        System.out.println("P(" + x + ") = " + resultat);
    }
}