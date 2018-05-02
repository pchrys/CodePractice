import java.util.Arrays;

import java.lang.reflect.Method;
import java.util.logging.Logger;
import java.util.logging.Level;
import java.util.logging.Handler;
import java.util.logging.ConsoleHandler;

public class XorGame {

  Logger myLogger = Logger.getLogger(this.getClass().getName());
  Level myLogLevel = Level.FINE;
  public boolean playXorGame(int[] nums) {
    Logger.getLogger("").getHandlers()[0].setLevel(Level.INFO);
    myLogger.setLevel(myLogLevel);
    // myLogger.setUseParentHandlers(false);
    // Handler handler = new ConsoleHandler();
    // handler.setLevel(myLogLevel);
    // myLogger.addHandler(handler);

    if (nums == null || nums.length == 0) {
      return true;
    }

    int n = nums.length;
    int remainingNum = n;
    int s = 0;

    for (int i : nums) {
      s ^= i;
    }

    if (s == 0) {
      return true;
    }

    // int remainingNum = n;
    boolean[] visited = new boolean[n];

    while (remainingNum >= 2) {
      // across two numbers
      if (s == 0) {
        return true;
      }

      int i = 0;
      for (i = 0; i < n; i++) {
        if (!visited[i] && (nums[i] ^ s) != 0) {
          visited[i] = true;
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
        if (!visited[i] && (nums[i] ^ s) != 0) {
          visited[i] = true;
          s = s ^ nums[i];
          remainingNum--;
          break;
        }
      }

      if (i == n) {
        return true;
      }
    }

    // Class cls = new Object(){}.getClass();
    // Method m = cls.getEnclosingMethod();
    // String mn = m.getName();

    myLogger.logp(myLogLevel, this.getClass().getName(), new Object() {
    }.getClass().getEnclosingMethod().getName(), Arrays.toString(nums));

    myLogger.log(myLogLevel, Arrays.toString(visited) + " at " +
                                 Thread.currentThread().getStackTrace()[0]);
    myLogger.log(myLogLevel, "=====================");

    Handler[] handles = myLogger.getHandlers();
    for (Handler h : handles) {
      System.out.println("level: " + h.getLevel() + " at " +
                         Thread.currentThread().getStackTrace()[0]);
    }

    myLogger.log(myLogLevel, "=====================");

    // how to set log level for consoleHandler;
    if (remainingNum == 1) {
      for (int i = 0; i < n; i++) {
        if (!visited[i] && nums[i] == 0) {
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

    XorGame obj = new XorGame();

    // boolean flag = new XorGame().playXorGame(nums);
    boolean flag = obj.playXorGame(nums);

    System.out.println("flag: " + flag);
  }
}
