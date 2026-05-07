public class Main {
    public static void main(String[] args) {
        Polynome p = new Polynome();
        p.parser("-4.5*X^5 + 2*X^4 + X^3 - X + 123.0");
        System.out.println("Polynôme parsé avec succès !");
    }
}