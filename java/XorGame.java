
public class XorGame {

  public boolean playXorGame(int[] nums) {

    if (nums == null || nums.length == 0) {
      return true;
    }

    if (nums.length == 1) {
      if (nums[0] == 0) {
        return true;
      } else {
        return false;
      }
    }

    int n = nums.length;
    int s = 0;
    for (int i : nums) {
      s ^= i;
    }

    if (s == 0) {
      return true;
    }

    int remainingNum = n;
    boolean[] removed = new boolean[n];

    while (remainingNum >= 2) {
      // across two numbers
      if (s == 0) {
        return true;
      }
      int i = 0;
      for (i = 0; i < n; i++) {
	  if ((nums[i] ^ s) != 0) {
          removed[i] = true;
          s = s ^ nums[i];
          remainingNum--;
          break;
        }
      }
      // cannot find any number
      if (i == n) {
        return false;
      }

      for (i = 0; i < n; i++) {
	  if ((nums[i] ^ s) != 0) {
          removed[i] = true;
          s = s ^ nums[i];
          remainingNum--;
          break;
        }
      }

      if (i == n) {
        return true;
      }
    }

    if (remainingNum == 1) {
      for (int i = 0; i < n && !removed[i]; i++) {
        if (nums[i] == 0) {
          return true;
        } else {
          return false;
        }
      }
    }

    return true;
  }

  public static void main(String[] args) {
      int[] nums = {1, 1, 2};
      boolean flag = new XorGame().playXorGame(nums);

      System.out.println("flag: " + flag);
  }
}
