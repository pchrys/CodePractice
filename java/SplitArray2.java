import java.util.List;
import java.util.LinkedList;
import java.util.Collections;

public class SplitArray2 {
  private static class SubArray {
    int i = 0;
    int j = 0;
    int sum = 0;

    SubArray(int a, int b, int s) {
      i = a;
      j = b;
      sum = s;
    }

    public String toString() {
      return String.format("[%d, %d, %d]", i, j, sum);
    }
  }

  public int splitArray(int[] nums, int m) {
    if (nums == null || nums.length < 1) {
      return -1;
    }

    int n = nums.length;

    if (m == 1) {
      int sum = 0;

      for (int e : nums) {
        sum += e;
      }
      return sum;
    }

    assert m <= n;

    // subarrays size is m
    List<SubArray> subArrays = new LinkedList<>();

    // subArray[i] + subarray[i+1]
    List<Integer> q = new LinkedList<>();

    int i = 0;
    for (i = 0; i < m; i++) {
      subArrays.add(new SubArray(i, i + 1, nums[i]));

      if (i > 0) {
        q.add(subArrays.get(i - 1).sum + subArrays.get(i).sum);
      }
    }

    // add new elements into subArrays, now we have m+1 subarrays, merge two
    // adjacent ones
    // and reduce the number of subArrays back to m

    for (; i < n; i++) {
      subArrays.add(new SubArray(i, i + 1, nums[i]));
      // System.out.println("subArrays:" + subArrays.toString() + ", i:" + i);
      q.add(subArrays.get(m - 1).sum + subArrays.get(m).sum);

      int k = 0;
      int min = q.get(0);
      for (int a = 1; a < q.size(); a++) {
        if (min > q.get(a)) {
          min = q.get(a);
          k = a;
        }
      }

      System.out.println("k: " + k + " at " +
                         Thread.currentThread().getStackTrace()[1]);
      int i1 = subArrays.get(k).i;
      int i2 = subArrays.get(k + 1).j;
      assert subArrays.get(k).j == subArrays.get(k + 1).i;
      int s = subArrays.get(k).sum + subArrays.get(k + 1).sum;
      subArrays.set(k, new SubArray(i1, i2, s));
      subArrays.remove(k + 1);

      // q.clear();
      // for (int a = 0; a < m - 1; a++) {
      //   q.add(subArrays.get(a).sum + subArrays.get(a + 1).sum);
      // }

      if (k + 1 < m) {
        q.remove(k + 1);
      } else {
        q.remove(q.size() - 1);
      }

      if (k > 0) {
        q.set(k - 1, subArrays.get(k - 1).sum + subArrays.get(k).sum);
      }
      if (k + 1 < m) {
        q.set(k, subArrays.get(k).sum + subArrays.get(k + 1).sum);
      }
    }

    SubArray max = Collections.max(subArrays, (a, b) -> a.sum - b.sum);

    return max.sum;
  }

  public static void main(String[] args) {

    int[] A = {7, 5, 2, 10, 8};
    int m = 2;

    int r = new SplitArray2().splitArray(A, m);

    System.out.println("r: " + r);
  }
}
