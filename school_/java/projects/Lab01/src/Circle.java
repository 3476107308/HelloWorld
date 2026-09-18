public class Circle {
    public static final double PI = 3.14;

    public static void main(String[] args) {
        double r = 3.0;
        double l = 2 * PI * r;
        double s = PI * r * r;
        System.out.printf("周长: %.2f 面积: %.2f%n",l,s);
    }
}
