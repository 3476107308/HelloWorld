/*
 * HelloWorld.java —— 环境自检用示例（不是作业，跑通即可删）
 *
 * 用法（在终端里，先 cd 到本文件所在目录）：
 *   javac HelloWorld.java     编译，生成 HelloWorld.class
 *   java HelloWorld           运行（注意：不要写 .class 后缀）
 */

public class HelloWorld {
    public static void main(String[] args) {
        System.out.println("Hello, Java 25!");

        // 想打印中文就把下面这行的注释去掉。
        // 如果 javac 报"编码GBK的不可映射字符"，改用：
        //   javac -encoding UTF-8 HelloWorld.java
        // 如果运行时中文是乱码，先执行：chcp 65001
        // System.out.println("你好，Java 25！");

        // 顺手看看 JVM 报的是哪个版本，截图给实验报告用正好
        System.out.println("java.version = " + System.getProperty("java.version"));
        System.out.println("java.home    = " + System.getProperty("java.home"));
    }
}
