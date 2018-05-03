import java.util.PriorityQueue;
import java.util.TreeSet;
import java.util.Collections;

public class SplitArray {

  int splitArray(int[] A, int m) {
    int n = A.length;

    if (m == 1) {
      int s = 0;
      for (int i : A) {
        s += i;
      }
      return s;
    }

    TreeSet<Integer> q = new TreeSet<>();
    PriorityQueue<Integer> q2 = new PriorityQueue<>(Collections.reverseOrder());
    splitArray(A, 0, n, m, q, q2);

    return q2.peek();
  }

  // split Array A into m parts such that the max sum is minimized
  // q stores the sum of each subarray after partitioning the whole
  // array into m subarrays
  void splitArray(int[] A, int i, int j, int m, TreeSet<Integer> q,
                  PriorityQueue<Integer> q2) {
    if (j - i < m) {
      return;
    } else if (j - i == m) {
      for (int k = i; k < j; k++) {
        q.add(A[k]);
      }
      q2.add(q.last());
      for (int k = i; k < j; k++) {
        q.remove(A[k]);
      }

      return;
    }

    System.out.println("q: " + q);
    System.out.println("q2: " + q2);
    if (m == 1) {
      int s = 0;
      for (int k = i; k < j; k++) {
        s += A[i];
      }
      q.add(s);
      q2.offer(q.last());

      // backtracing
      q.remove(s);

    } else {
      int ps = A[i];
      for (int k = i + 1; k < j; k++) {
        q.add(ps);
        splitArray(A, k, j, m - 1, q, q2);

        q.remove(ps);
        ps += A[k];
        // remove qs from the
      }
    }
  }

  public static void main(String[] args) {
    int[] A = {7, 2, 5, 10, 8};
    int m = 2;
    int s = new SplitArray().splitArray(A, m);

    System.out.println("s: " + s);
  }
}
