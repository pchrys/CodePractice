import java.util.ArrayList;
import java.util.Scanner;
import java.util.logging.*;
import my.util.DebugPrint;

public class PhoneNumber2String {
  public PhoneNumber2String(String phoneNumber) {
    this.phoneNumber = phoneNumber;
    result = new StringBuilder(phoneNumber.length());
    result.setLength(phoneNumber.length());
  }

  public ArrayList<String> convert() {
    convert(phoneNumber, 0);

    for (String s : results) {
      System.out.println("num2string: " + s);
    }
    return results;
  }

  void convert(String phoneNumber, int i) {
    System.out.printf("phonenumber: %s, i:%d \n", phoneNumber, i);
    if (i == phoneNumber.length()) {
      results.add(result.toString());
      return;
    }

    int idx = phoneNumber.charAt(i) - '0';
    for (char c : num2String[idx].toCharArray()) {
      result.setCharAt(i, c);
      convert(phoneNumber, i + 1);
    }
  }
  private String phoneNumber;
  private StringBuilder result;
  private ArrayList<String> results = new ArrayList<>();
  private String[] num2String = {"", // 0
                                 "  ",
                                 "ABC",
                                 "DEF",
                                 "GHI", //4
                                 "JKL",
                                 "MNO", "PQRS",
                                 "TUV", "WXYZ"};

  public static void main(String[] args) {
    System.out.printf("Please input phone number: ");
    Scanner in = new Scanner(System.in);

    String phoneNumber = ""; // in.nextLine();
    phoneNumber = phoneNumber.trim();

    // key: have to remove 0 from the number first, 0 stops the iteration
    // String phoneNumber = new String("5082819918");
    String tmp = "";
    for (char c : phoneNumber.toCharArray()) {
      if (c != '0') {
        tmp += c;
      }
    }
    phoneNumber = tmp;

    phoneNumber = "123";
    PhoneNumber2String myConverter = new PhoneNumber2String(phoneNumber);

    myConverter.convert();

    final Logger LOGGER = Logger.getLogger( "good.test");
    LOGGER.setLevel(Level.INFO);
    LOGGER.log( Level.INFO, "processing entries in loop" );

    DebugPrint.debugPrint();
    DebugPrint.debugPrint("good test");

  }
}
