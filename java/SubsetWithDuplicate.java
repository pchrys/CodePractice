import java.util.ArrayList;
import java.util.List;
import java.util.Arrays;

public class SubsetWithDuplicate {

  public List<List<Integer>> subSets(int[] nums) {
    List<Integer> t = new ArrayList<Integer>();
    List<List<Integer>> result = new ArrayList<List<Integer>>();

    result.add(t);
    if (nums == null || nums.length == 0) {
      return result;
    }

    Arrays.sort(nums);

    List<List<Integer>> newSubSets = null;
    for (int i = 0; i < nums.length; i++) {
      List<List<Integer>> tmp = new ArrayList<List<Integer>>(result);
      List<List<Integer>> tmp2 = new ArrayList<List<Integer>>();

      if (i == 0 || nums[i] != nums[i - 1]) {
        newSubSets = result;
      }

      for (List<Integer> li : newSubSets) {
        List<Integer> newList = new ArrayList<Integer>(li);
        newList.add(nums[i]);
        tmp2.add(newList);
      }

      tmp.addAll(tmp2);
      result = tmp;

      newSubSets = tmp2;
    }

    return result;
  }

  public static void main(String[] args) {
    int[] nums = {1, 1, 2, 2};

    List<List<Integer>> r = new SubsetWithDuplicate().subSets(nums);
    System.out.println(r);
  }
}
