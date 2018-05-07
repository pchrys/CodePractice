import java.util.TreeMap;
import java.util.Map;


public class P782TransformToChessBoard {
  public int movesToChessboard(int[][] board) {
    if (board == null || board.length <= 1) {
      return 0;
    }

    // check the input is valid
    if (board.length >= 2) {
      for (int i = 0; i < board.length; i++) {
        if (board[i] == null || board[i].length != board.length) {
          return -1;
        }
      }
    }

    // count number of 1 on each row and column;
    int n = board.length;
    int[] r = new int[n];
    int[] c = new int[n];

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        r[i] += board[i][j];
        c[j] += board[i][j];

        if (r[i] > n / 2 + 1 || c[j] > n / 2 + 1) {
          return -1;
        }
      }
    }

    if (n % 2 == 0) {
      for (int i = 0; i < n; i++) {
        if (c[i] != n / 2 || r[i] != n / 2) {
          return -1;
        }
      }
    } else {
      for (int i = 0; i < n; i++) {
        if (r[i] < n / 2 || c[i] < n / 2) {
          return -1;
        }
      }
    }

    //and we can only have two patterns, and the number of
    //#P1 ==  #P2 or abs(#P1 - #P2) == 1
    //convert each row as a string;
    TreeMap<String, Integer> strMap = new TreeMap<>();
    for(int i = 0; i < n; i++){
	String s="";
	for(int j = 0; j < n; j++){
	    s = s + board[i][j];
	}

	if(strMap.containsKey(s)){
	    strMap.put(s, strMap.get(s) + 1);

	}
	else{
	    strMap.put(s, 1);
	}
    }

    if(strMap.size() != 2){
	return -1;
    }

    Map.Entry<String, Integer> first = strMap.firstEntry();
    Map.Entry<String, Integer> last  = strMap.lastEntry();

    if(Math.abs(first.getValue() - last.getValue()) > 1){
	return -1;
    }

    // transform the board
    int swapNum = 0;
    for(int i = 0; i < n - 1; i++){
	int j = i;

	boolean flag = false;
	int[] p  = Arrays.copyOf(board[0], n);
	while( i+1 < n && p[i] == p[i+1]){
	    i++;
	    flag = true;
	}

	if(flag){
	    swapNum++;
	    int tmp = p[j+1];
	    p[j+1] = p[i+1];
	    p[i+1] = tmp;

	    i = j+1;
	//swap C_{j+1} and C_{i+1};
	}
    }

    for(int i = 0; i < n; i++){
	int j = i;
	boolean flag = false;
	int[] p = new int[n];
	for(int i = 0; i < n; i++){
	    p[i] = board[i][0];
	}

	while(i+1 < n && p[i] == p[i+1]){
	    i++;
	    flag = true;
	}

	if(flag){
	    swapNum++;

	    int tmp = p[j+1];
	    p[j+1] = p[i+1];
	    p[i+1] = tmp;

	    i = j+1;
	}
	//swap R_{j+1} and R_{i+1};
    }

    return swapNum;
  }
}
