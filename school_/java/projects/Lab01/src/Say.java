public class Say {
    public static void main(String[] args) {
        if(args.length < 2)
        {
            System.out.println("需要2个命令行参数！");
            System.exit(1);
        }
        String name = args[0];
        String word = args[1];
        System.out.println("我想对" + name + "悄悄的说：" + word);
    }
}

