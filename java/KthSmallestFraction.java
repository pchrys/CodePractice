import java.util.List;
import java.util.ArrayList;
import java.util.Collections;
import java.util.NavigableSet;
import java.util.TreeSet;


public class KthSmallestFraction {

  private static class Pair implements Comparable<Pair> {
    int first;
    int second;

    Pair(int a, int b) {
      first = a;
      second = b;
    }
    public int compareTo(Pair p) {
      return Double.compare(first / (double)second, (double)p.first / p.second);
    }
  }

  int[] kthSmallestFraction(int[] input, int k) {
    if (input == null || input.length == 1) {
      System.out.println("Wrong input ");
      return null;
    }

    List<Pair> fractions = new ArrayList<Pair>();

    for (int i = 1; i < input.length; i++) {

      for (int j = 0; j < i; j++) {

        Pair tmp = new Pair(j, i);

        fractions.add(tmp);
      }
    }

    Collections.sort(fractions,
                     (a, b)
                         -> Double.compare(a.first / (double)a.second,
                                           b.first / (double)b.second));

    assert k >= 0 && k < fractions.size();

    return new int[] {fractions.get(k - 1).first, fractions.get(k - 1).second};
  }

  // List<Integer> merge(List<Integer> a, List<Integer> b) {
  //   if ((a == null || a.length == 0) && (b == null || b.length == 0)) {
  //     return new ArrayList<Integer>();
  //   }

  //   if ((a == null || a.length == 0) || (b == null || b.length == 0)) {
  //     return new ArrayList<Integer>((a == null || a.length == 0 ? b : a));
  //   }

  //   int i = 0;
  //   int j = 0;

  //   List<Integer> r = new ArrayList<Integer>();
  //   while (true) {
  //     if (a.get(i) < b.get(j)) {
  //       r.add(a.get(i));
  //       i++;
  //     } else {
  //       r.add(b.get(j));
  //       j++;
  //     }

  //     if (i == a.length) {
  //       List<Integer> t = b.subList(j, b.size());
  //       r.addAll(t);
  //       break;
  //     } else if (j == b.length) {
  //       r.addAll(a.subList(i, a.size()));
  //       break;
  //     }
  //   }

  //   return r;
  // }

  List<Pair> merge(List<Pair> a, List<Pair> b) {
    if ((a == null || a.size() == 0) && (b == null || b.size() == 0)) {
      return new ArrayList<Pair>();
    }

    if ((a == null || a.size() == 0) || (b == null || b.size() == 0)) {
      return new ArrayList<Pair>((a == null || a.size() == 0 ? b : a));
    }

    int i = 0;
    int j = 0;

    List<Pair> r = new ArrayList<Pair>();
    while (true) {
      if (a.get(i).first / (double)a.get(i).second <
          b.get(j).first / (double)b.get(j).second) {
        r.add(a.get(i));
        i++;
      } else {
        r.add(b.get(j));
        j++;
      }

      if (i == a.size()) {
        List<Pair> t = b.subList(j, b.size());
        r.addAll(t);
        break;
      } else if (j == b.size()) {
        r.addAll(a.subList(i, a.size()));
        break;
      }
    }

    return r;
  }

  int[] kthSmallestFraction2(int[] A, int k) {
    if (A == null || A.length == 1) {
      System.out.println("Wrong input ");
      return null;
    }

    List<Pair> fractions = new ArrayList<Pair>();
    Pair e = new Pair(1, A[1]);
    fractions.add(e);

    for (int i = 2; i < A.length; i++) {
      List<Pair> newItem = new ArrayList<Pair>();

      for (int j = 0; j < i; j++) {

        Pair tmp = new Pair(A[j], A[i]);
        newItem.add(tmp);
      }
      fractions = merge(fractions, newItem);
    }

    return new int[] {fractions.get(k - 1).first, fractions.get(k - 1).second};
  }

  int[] kthSmallestFraction3(int[] A, int k) {
    if (A == null || A.length <= 1) {
      System.out.println("Wrong input ");
      return null;
    }

    //no empty list;
    TreeSet<List<Pair>> numSet =
        new TreeSet((List<Pair> a, List<Pair> b) -> a.get(0).compareTo(b.get(0)));

    // build up map
    for (int i = 0; i < A.length - 1; i++) {
      List<Pair> tmp = new ArrayList<Pair>();
      for (int j = i + 1; j < A.length; j++) {
        tmp.add(new Pair(A[i], A[j]));
        Collections.reverse(tmp);
      }

      numSet.add(tmp);
    }

    int x = k;
    List<Pair> f1 = numSet.pollFirst();
    List<Pair> f2 = numSet.first();

    while (x > 0) {
      int y = Collections.binarySearch(f1, f2.get(0));

      assert y < -1;
      y = Math.abs(y + 1);

      assert y > 0;

      if (x <= y) {
        return new int[] {f1.get(x - 1).first, f1.get(x - 1).second};
      } else {
        x = x - y;
      }

      if (y < f1.size() - 1) {
        numSet.add(f1.subList(y + 1, f1.size()));
      }
    }

    return null;
  }
}
