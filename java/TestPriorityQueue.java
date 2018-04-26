import java.util.PriorityQueue;

public class TestPriorityQueue {
  private static class Pair implements Comparable<Pair> {
    int first;
    int second;
    Pair(int a, int b) {
      first = a;
      second = b;
    }

    @Override
    public int compareTo(Pair a1) {
      return Double.compare((double)first / second,
                            (double)a1.first / a1.second);
    }
  }
  public static void testPriorityQueue() {
    PriorityQueue<Pair> q = new PriorityQueue<>();
    int[] a = {1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};

    int n = a.length;
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        Pair p = new Pair(a[i], a[j]);
        q.offer(p);
      }
    }

    int cnt = 0;
    while (!q.isEmpty()) {
      Pair p = q.poll();
      System.out.printf("%4d/%-4d  ", p.first, p.second);
      cnt++;
      if (cnt == 10) {
        System.out.printf("\n");
        cnt = 0;
      }
    }
  }

  public static void main(String[] args) {
	  testPriorityQueue();
  }
}
