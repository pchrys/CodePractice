import java.util.Arrays;
import java.util.List;
import java.util.LinkedList;
// import java.util.Arrays;
// import java.util.Arrays;

public class TestEqualEverage {

  // observation sum(a_i)/n = target, then find
  // other wise, find subarray inside A - {a_i}
  boolean splitArrayWithEqualAverage(List<Integer> li, double s, double target) {
    if (li.size() == 1 ) {
      return false;
    }

    int n = li.size();
    final double EPS = 1e-6;

    for (int i = 0; i < li.size(); i++) {

      double s2 = s - li.get(i);
      double avg = t / (n - 1);

      if (Math.abs(avg - target) < EPS) {
        return true;
      } else if (avg + EPS < target) {
        continue;
      }

      List<Integer> li2 = new LinkedList<>(li);
      li2.remove(i)

      if (splitArrayWithEqualAverage(li2, , s2, target)) {
        return true;
      }

    }

    return false;
  }

  boolean splitArrayWithEqualAverage(int[] nums) {
    if (nums == null || nums.length < = 1) {
      return false;
    }

    double s = 0.0;
    for (int n : nums) {
      s += n;
    }

    final double target = s / nums.length;

    Arrays.sort(nums);
    List<Integer> li = new LinkedList<>();
    for (int n : nums) {
      li.add(n);
    }

    boolean[] visited = new boolean[n];
    // invariant  s the sum of all numbers stored inside li
    // target is the average
    return splitArrayWithEqualAverage(li, 1, visited, s, target);
  }

  public static void main(String[] args) {}
}
