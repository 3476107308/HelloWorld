// 教材 (1) 的原始版本：没有参数个数检查。用来演示"不带参数运行会抛什么异常"。
public class SayNoGuard {
    public static void main(String args[]) {
        String name = args[0];
        String word = args[1];
        System.out.println("我想对" + name + "悄悄地说:" + word);
    }
}
