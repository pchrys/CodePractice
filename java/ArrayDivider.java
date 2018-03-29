// check if it is possible to divide the array into two parts
// while the sum of each new array are the same

// basic idea, sorted the array, and then do DFS search

import java.util.Arrays;
import java.util.ArrayList;

class ArrayDivider {

  ArrayDivider() {}

  int n = 0;
  int hsum = 0;
  int[] a;
  ArrayList<ArrayList<Integer>> results = new ArrayList<ArrayList<Integer>>();
  ArrayList<Integer> r = new ArrayList<Integer>();

  public boolean splitArray(int[] a) {
    if (a == null || a.length == 0) {
      return false;
    }

    Arrays.sort(a);

    int sum = 0;
    for (int i = 0; i < a.length; i++) {
      sum += a[i];
    }

    if (sum % 2 != 0) {
      return false;
    }

    hsum = sum / 2;
    n = a.length;
    this.a = a;

    int pathSum = 0;

    DFSVisit(0, pathSum);

    return !results.isEmpty();
  }

  void DFSVisit(int i, int pathSum) {
      if(i >= n){
	  return;
      }
    pathSum += a[i];
    r.add(a[i]);
    System.out.printf("i: %d, pathSum: %d \n", i, pathSum);

    if (pathSum == hsum) {
            ArrayList<Integer> tmp = new ArrayList<Integer>(r);

	    for(Integer n : tmp){
		System.out.print(n + " " );
	    }
	    System.out.println("hsum: " + hsum);

            results.add(tmp);
	    r.remove(r.size() - 1);
            return;
    } else if (pathSum < hsum) {
      for (int j = i + 1; j < n; j++) {
        DFSVisit(j, pathSum);
      }
    }

    r.remove(r.size() -1);
  }

  public static void main(String[] args) {
      int[] a = {1, 2, 3, 4, 6};
      int[] a = {-3, -1, 1, 2, 3, 4, 6, 8};

    ArrayDivider dv = new ArrayDivider();
    dv.splitArray(a);

    if(!dv.results.isEmpty()){
	System.out.println( "=================================== ");
	for(ArrayList<Integer> nums: dv.results){
	    for(Integer n: nums){
		System.out.print(n + " ");
	    }

	    System.out.print("\n");
	}
    }
  }
}
