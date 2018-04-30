public class P808SoupServing {
  double[] getProbability(double[][] p1, double[][] p2, i, int j) {
    int[][] op = {{4, 0}, {3, 1}, {2, 2}, {1, 3}};
    double[] q = {1.0 / 4, 1.0 / 4, 1.0 / 4, 1.0 / 4};

    // A empty before B;
    double s1 = 0.0;
    double s2 = 0.0;
    for (int k = 0; k < op.length; k++) {
      int a1 = i - op[k][0];
      int a2 = j = op[k][1];
      if (a1 > 0 && a2 > 0) {
        s1 += q[k] * p1[a1][a2];
        s2 += q[k] * p2[a1][a2];
      } else if (a1 <= 0 && a2 > 0) {
        s1 += q[k]; // A empty first;
      } else if (a1 <= 0 && a2 <= 0) {
        s2 += q[k]; // A and B empty at the same time;
      }else{  //B empty first
	  ;
      }
    }

    return new double[] {s1, s2};
  }

  double soupServing(int N) {
    int n = (N + 24) / 25;

    double[][] p1 =
        new int[n + 1][n + 1]; // p1[i][j] denotes the probability we empty A
                               // before  B
    double[][] p2 =
        new int[n + 1][n + 1]; // p2[i][j] denotes the probability we empty A
                               // and  B at the same time

    for (int i = 1; i < n + 1; i++) {
      for (int j = 1; j <= i; j++) {
        p1[i][j] = getProbability(p1, p2, i, j)[0];
        p2[i][j] = getProbability(p1, p2, i, j)[1];

	p1[j][i] = getProbability(p1, p2, j, i)[0];
        p2[j][i] = getProbability(p1, p2, j, i)[1];
      }
      }
    }

    return p1[n][n] + p2[n][n] / 2;
  }

}
