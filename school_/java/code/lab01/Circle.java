// P153 编程题第 1 题：计算半径为 3.0 的圆的周长和面积
public class Circle {
    public static void main(String args[]) {
        double r = 3.0;
        double pi = Math.PI;              // 3.141592653589793（也可以按教材写 3.14159）

        double circumference = 2 * pi * r;   // 周长 = 2πr
        double area = pi * r * r;            // 面积 = πr²

        System.out.println("半径 r = " + r);
        System.out.println("周长 = " + circumference);
        System.out.println("面积 = " + area);

        // 想要"小数点后固定几位"，用 printf（就是 C 的 printf）
        System.out.printf("周长 = %.4f%n", circumference);
        System.out.printf("面积 = %.4f%n", area);
    }
}
