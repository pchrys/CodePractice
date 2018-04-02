import java.util.Arrays;

public class QueenPlacement {

  private final int N;
  private final char MARK = 'X';
  private final char UNMARK = '.';
  private char[][] grid;
  private boolean[][] visited;
  private boolean validPlacementFound = false;

  private void initialGrid() {
    grid = new char[N][N];

    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        grid[i][j] = UNMARK;
      }
    }

    visited = new boolean[N][N];
  }

  public QueenPlacement(int n) {
    N = n;
    initialGrid();
  }

  private boolean isValidCoordinate(int x, int y) {
    if (x < 0 || y < 0 || x >= N || y >= N) {
      return false;
    }

    return true;
  }

  private boolean isValidPlacement(int x, int y) {
    // check x row
    boolean xRowValid = true;
    for (int i = 0; i < N && i != x; i++) {
      if (grid[i][y] == MARK) {
        xRowValid = false;
      }
    }

    System.out.println("(" + x + ", " + y + "), "
                       + "row is valid: " + xRowValid);
    if (!xRowValid) {
      return xRowValid;
    }

    // check y column
    boolean yColumnValid = true;

    for (int i = 0; i < N && i != y; i++) {
      if (grid[x][i] == MARK) {
        yColumnValid = false;
      }
    }

    System.out.println("(" + x + ", " + y + "), "
                       + "column is valid: " + yColumnValid);

    if (!yColumnValid) {
      return yColumnValid;
    }

    // check primary diagnal
    boolean primaryDiagValid = true;
    int i = x - 1;
    int j = y - 1;
    while (primaryDiagValid) {
      if (isValidCoordinate(i, j)) {
        if (grid[i][j] == MARK) {
          primaryDiagValid = false;
          System.out.println("rule is violated at (" + i + ", " + j + ")");
          break;
        }

      } else {
        break;
      }

      i = i - 1;
      j = j - 1;
    }
    // check the other half
    i = x + 1;
    j = y + 1;
    while (primaryDiagValid) {
      if (isValidCoordinate(i, j)) {
        if (grid[i][j] == MARK) {
          System.out.println("rule is violated at (" + i + ", " + j + ")");
          primaryDiagValid = false;
          break;
        }

      } else {
        break;
      }

      i = i + 1;
      j = j + 1;
    }
    System.out.println("(" + x + ", " + y + "), "
                       + "primarydiagvalid is valid: " + primaryDiagValid);

    if (!primaryDiagValid) {
      return primaryDiagValid;
    }

    // check secondary diagnal
    boolean secondaryDiagValid = true;
    i = x - 1;
    j = y + 1;
    while (secondaryDiagValid) {
      if (isValidCoordinate(i, j)) {
        if (grid[i][j] == MARK) {
          secondaryDiagValid = false;
          break;
        }

      } else {
        break;
      }

      i = i - 1;
      j = j + 1;
    }
    System.out.println("(" + x + ", " + y + "), "
                       + "secondaryDiagValid is valid: " + secondaryDiagValid);

    // check the other half
    i = x + 1;
    j = y - 1;
    for (; secondaryDiagValid; i++, j--) {
      if (isValidCoordinate(i, j)) {
        if (grid[i][j] == MARK) {
          secondaryDiagValid = false;
          break;
        }

      } else {
        break;
      }
    }
    System.out.println("(" + x + ", " + y + "), "
                       + "secondaryDiagValid is valid: " + secondaryDiagValid);

    if (!secondaryDiagValid) {
      return secondaryDiagValid;
    }

    return true;
  }

  public void placeQueen() {
    int n = N;

    if (n < 4) {
      System.out.println("such placement does not exist \n");
      new Throwable().printStackTrace();

      System.exit(0);
    }

    int k = 0;
    int i = 1;
    int j = 0;

    int x = 1;
    int y = 0;

    // place first n/2 nodes without check
    while (isValidCoordinate(x, y)) {
      grid[x][y] = MARK;
      visited[x][y] = true;
      k++;

      x = x + 2;
      y = y + 1;
    }

    System.out.printf("pos1, x: %d, y: %d \n", x, y);
    // place the Queen on the boundary
    if (x == N) {
      x = N - 1;
      y = y + 1;
      grid[x][y] = MARK;
      visited[x][y] = true;
      k++;

      if (!isValidCoordinate(x, y)) {
        System.out.println("Something goes wrong inside placement \n");

        new Throwable().printStackTrace();

        System.exit(0);
      };
    }

    System.out.printf("pos2, x: %d, y: %d, k=%d \n", x, y, k);

    showPlacement();

    if (x == N + 1) {
      x = N - 1;
      y = y - 1;
    }

    DFS(x, y, k);

    showPlacement();
  }

  public void placeQueen2() {
    int n = N;

    if (n < 4) {
      System.out.println("such placement does not exist \n");
      new Throwable().printStackTrace();

      System.exit(0);
    }

    for (int i = 0; i < N; i++) {
      if (validPlacementFound) {
        System.out.printf("=================skip ******************\n");
        break;
      }
      int x = i;
      int y = 0;
      System.out.printf(
          "*******************start search at (%d, %d) ********************* \n",
          x, y);

      grid[x][y] = MARK;
      visited[x][y] = true;
      DFS(x, y, 0);

      showPlacement();

      if (!validPlacementFound) {

        grid[x][y] = UNMARK;
        visited[x][y] = false;
        int[][] neighbors = getNeighbors(x, y);

        showPlacement();

        for (int[] row : neighbors) {
          int s = row[0];
          int t = row[1];
          if (isValidCoordinate(s, t) && grid[s][t] != MARK) {
            visited[s][t] = false;
          }
        }
      }
      showPlacement();
    }
  }

  int[][] getNeighbors(int x, int y) {
    // return new int[][] {
    //     {x - 1, y + 2}, {x - 2, y + 1}, {x - 2, y - 1}, {x - 1, y - 2}};

    return new int[][] {{x - 1, y + 2}, {x - 2, y + 1}, {x - 2, y - 1},
                        {x - 1, y - 2}, {x + 1, y + 2}, {x + 2, y + 1},
                        {x + 2, y - 1}, {x + 1, y - 2}

    };
  }

  // DFS search
  private void DFS(int a, int b, int n) {
    System.out.printf("DFS(), pos3, a: %d, b: %d, n=%d \n", a, b, n);
    showPlacement();

    if (n == N || validPlacementFound) {
      validPlacementFound = true;
      return;
    }

    int[][] neighbors = getNeighbors(a, b);

    System.out.println(Arrays.deepToString(neighbors));
    // for (int i = 0; i < neighbors.length; k++) {
    for (int[] row : neighbors) {
      int x = row[0];
      int y = row[1];

      if (isValidCoordinate(x, y) && !visited[x][y]) {
        visited[x][y] = true;
        if (isValidPlacement(x, y)) {
          System.out.printf("pos4, x: %d, y: %d, n=%d \n", x, y, n);
          grid[x][y] = MARK;
          DFS(x, y, n+1);
        }
      }
    }

    grid[a][b] = UNMARK;
    for (int[] row : neighbors) {
      int x = row[0];
      int y = row[1];
      if (isValidCoordinate(x, y) && grid[x][y] != MARK) {
        visited[x][y] = false;
      }
    }
  }

  private void showPlacement() {
    System.out.println("==================================");
    for (char[] row : grid) {
      System.out.print('[');
      for (char c : row) {
        System.out.print(c + " ");
      }
      System.out.print("] \n");
    }
    System.out.println("==================================");
  }

  public static void main(String[] args) {

    int n = 8;
    QueenPlacement q = new QueenPlacement(n);

    q.placeQueen2();
  }
}
