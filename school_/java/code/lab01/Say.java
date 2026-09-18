// 教材 (3) 的最终版本：先判断参数个数，再使用。
// 注意：教材第 (4) 步写成 public class say —— 那是笔误，类名必须和文件名一致，所以这里必须是 Say。
public class Say {
    public static void main(String args[]) {
        if (args.length < 2) {
            System.out.println("需要 2 个命令行参数!");
            System.exit(1);
        }
        String name = args[0];
        String word = args[1];
        System.out.println("我想对" + name + "悄悄地说:" + word);
    }
}
