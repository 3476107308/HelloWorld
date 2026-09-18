// 这个文件是为了演示"教材原文一字不改会报什么错"，故意保留错误写法。
// 真正的错误在第 5 行：char 必须用单引号。
public class SimpleTypesBad {
    public static void main(String args[]) {
        char c = "c";
        System.out.println("c=" + c);
    }
}
