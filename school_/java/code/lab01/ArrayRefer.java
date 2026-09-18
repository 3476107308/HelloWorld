public class ArrayRefer {
    public static void main(String args[]) {
        int i;
        int arrayA[] = new int[5];
        for (i = 0; i < 5; i++)
            arrayA[i] = i;
        for (i = 0; i < arrayA.length; i++)
            System.out.println("arrayA[" + i + "]=" + arrayA[i]);
    }
}
