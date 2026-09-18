public class HaltDemo {
    public static void main(String[] args) {
        Runtime.getRuntime().addShutdownHook(new Thread(
                () -> System.out.println("[关闭钩子] 运行了")
        ));

        try {
            System.out.println("A: 进入 try 块");
            System.out.println("   调用了 Runtime.halt(2) ...");
            Runtime.getRuntime().halt(2);     // 硬杀：连关闭钩子都不跑
        } finally {
            System.out.println("C: finally 块");
        }
    }
}
