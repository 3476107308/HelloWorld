public class ConstDemo {
    public static void main(String[] args) {
        // 1) final 修饰"引用"，不修饰"对象内容"——这是和 C++ const 最大的区别
        final int[] a = {1, 2, 3};
        a[0] = 99;                        // 合法！数组内容可以改
        System.out.println("a[0] = " + a[0]);

        final StringBuilder sb = new StringBuilder("hi");
        sb.append("!");                   // 合法！对象内容可以改
        System.out.println("sb = " + sb);

        // 下面两行取消注释就会编译失败：final 冻结的是"这个变量不能再指向别的对象"
        // a  = new int[]{4, 5};
        // sb = new StringBuilder("x");
    }
}
