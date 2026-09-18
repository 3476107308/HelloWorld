import java.io.*;

public class SimpleTypes {
    public static void main(String args[]) {
        byte b = 055;          // 八进制字面量（前导 0）→ 十进制 45
        short s = 0x55ff;      // 十六进制字面量 → 十进制 22015
        int i = 1000000;
        long l = 3615L;
        char c = 'c';          // ⚠️ 教材原文写的是 char c="c"; —— 双引号是 String，那行是编译错误
        float f = 0.23F;
        double d = 0.7E-3;
        boolean bool = true;

        System.out.println("b=" + b);
        System.out.println("s=" + s);
        System.out.println("i=" + i);
        System.out.println("l=" + l);
        System.out.println("c=" + c);
        System.out.println("f=" + f);
        System.out.println("d=" + d);
        System.out.println("bool=" + bool);
    }
}
