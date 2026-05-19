import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.print("Entrez p1 : ");
        Polynome p1 = new Polynome();
        p1.parser(sc.nextLine());
        p1.trierParDegreDecroissant();
        System.out.print("p1 = "); p1.afficher();

        System.out.print("Entrez p2 : ");
        Polynome p2 = new Polynome();
        p2.parser(sc.nextLine());
        p2.trierParDegreDecroissant();
        System.out.print("p2 = "); p2.afficher();

        sc.close();

        // On enregistre les polynômes qu'on veut garder en vie
        Polynome.enregistrer(p1);
        Polynome.enregistrer(p2);

        System.out.println("p1(2) = " + p1.evaluer(2));
        System.out.println("p2(2) = " + p2.evaluer(2));

        // Ces opérations créent des maillons temporaires (résultats intermédiaires)
        System.out.println("\np1 + p2 = "); p1.plus(p2).afficher();
        System.out.println("p1 * p2 = "); p1.fois(p2).afficher();

        // Le GC libère tous les maillons temporaires non enregistrés
        System.out.println();
        Polynome.recycler();

        // On peut continuer à utiliser p1 et p2 normalement
        System.out.println();
        Polynome resultat = p1.moins(p2);
        Polynome.enregistrer(resultat);
        System.out.print("p1 - p2 = "); resultat.afficher();


        Polynome[] reste = new Polynome[1];
        Polynome q = p1.quotient(p2, reste);
        Polynome.enregistrer(q);
        Polynome.enregistrer(reste[0]);
        System.out.print("p1 / p2 = "); q.afficher();
        System.out.print("reste   = "); reste[0].afficher();

        System.out.print("p1 + p2 (récursif) = "); p1.plusRecursif(p2).afficher();
        System.out.print("p1 - p2 (récursif) = "); p1.moinsRecursif(p2).afficher();

        System.out.println();
        Polynome.recycler();
    }
}
