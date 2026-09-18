public class ExitDemo {
    public static void main(String[] args) {
        // 注册一个"JVM 关闭钩子"，用来观察 JVM 关闭时到底执行了什么
        Runtime.getRuntime().addShutdownHook(new Thread(
                () -> System.out.println("[关闭钩子] 运行了")
        ));

        try {
            System.out.println("A: 进入 try 块");
            if (args.length > 0) {
                System.out.println("   调用了 System.exit(1) ...");
                System.exit(1);
            }
            System.out.println("B: 没调用 exit，继续往下走");
        } finally {
            System.out.println("C: finally 块");
        }
        System.out.println("D: main 正常结束");
    }
}
