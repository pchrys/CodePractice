package my.util;

import java.util.logging.Logger;
import java.util.logging.Level;

public class DebugPrint {

  public DebugPrint() { myLogger.setLevel(Level.INFO); }

  public static void debugPrint(String msg) {
    int idx = 2;
    if (msg.length() > 0) {
      // System.out.println(msg);
      idx = 1;
    }

    Throwable t = new Throwable();

    String file = t.getStackTrace()[idx].getFileName();
    String clsname = t.getStackTrace()[idx].getClassName();
    String method = t.getStackTrace()[idx].getMethodName();
    int line = t.getStackTrace()[idx].getLineNumber();

    String debugMsg = String.format("%s.%s() is called at %s:%d \n", clsname,
                                    method, file, line);

    if (msg.length() > 0) {
      debugMsg = msg + ", " + debugMsg;
    }
    myLogger.log(Level.INFO, debugMsg);

    // System.out.printf("%s.%s() is called at %s:%d \n", clsname, method, file,
    // line);
  }

  public static void debugPrint() { debugPrint(""); }

  private final static Logger myLogger =
      Logger.getLogger(DebugPrint.class.getName());
}
