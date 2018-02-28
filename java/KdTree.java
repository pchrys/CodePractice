import java.util.Arrays;
import java.util.ArrayList;
import java.util.Random;

public class KdTree {
  private static class Node {
    static final int DIM = 2;
    int[] coordinate = new int[DIM];
    Node left;
    Node right;
    int cd = 0;

    public String toString() {

      // System.out.println("my toString is called");
      String r = "(";

      for (int i = 0; i < DIM; i++) {
        r += String.valueOf(coordinate[i]) + ", ";
      }
      // remove last ,)
      int idx = r.lastIndexOf(',');
      r = r.substring(0, idx);

      r += ")";

      return r;
    }
  }

  public void insert(Node node) {
    if (root == null) {
      root = node;
      root.cd = 0;
      return;
    }
    Node p = root;
    insert(p, node, 0);
  }

  private void insert(Node p, Node c, int cd) {
    if (cd >= Node.DIM) {
      return;
    }

    if (p == null) {
      return;
    }
    if (c == null) {
      return;
    }

    int idx = (cd + 1) % Node.DIM;
    if (c.coordinate[cd] < p.coordinate[cd]) {
      if (p.left == null) {
        p.left = c;
        c.cd = idx;
        return;
      }
      insert(p.left, c, idx);

    } else if (c.coordinate[cd] > p.coordinate[cd]) {
      {
        if (p.right == null) {
          p.right = c;
          c.cd = idx;
          return;
        }

        insert(p.right, c, idx);
      }
    }
  }

  // public void remove(Node node) {;}

  public Node findMax(int dim) {
    if (root == null) {
      return null;
    }

    return findMax(root, dim);
  }

  private Node findMax(Node p, int dim) {
    if (p.cd != dim) { // search left and right subtree
      Node u1, u2, u3; //
      u1 = u2 = u3 = null;

      if (p.left != null) {
        u1 = findMax(p.left, dim);
        u3 = u1;
      }

      if (p.right != null) {
        u2 = findMax(p.right, dim);
      }

      if (u2 != null && u1.coordinate[dim] < u2.coordinate[dim]) {
        u3 = u2;
      }

      if (u3 != null && u3.coordinate[dim] < p.coordinate[dim]) {
        u3 = p;
      }
      return u3;

    } else {
      if (p.right != null) {
        Node u1 = findMax(p.right, dim);
        return u1;
      } else {
        return p;
      }
    }
  }

  public Node findMin(int dim) {
    if (root == null) {
      return null;
    }
    return findMin(root, dim);
  }

  private Node findMin(Node p, int dim) {
    assert p == null;
    if (p.cd == dim) {
      if (p.left != null) {
        return findMin(p.left, dim);
      } else {
        return p;
      }
    } else {
      Node u1, u2, u3;
      u1 = u2 = u3 = null;
      if (p.left != null) {
        u1 = findMin(p.left, dim);
        u3 = u1;
      }

      if (p.right != null) {
        u2 = findMin(p.right, dim);
      }

      if (u2 != null && u3 == null) {
        u3 = u2;
      }

      if (u1 != null && u2 != null && u1.coordinate[dim] > u2.coordinate[dim]) {
        u3 = u2;
      }

      return u3;
    }
  }

  public Node find(int[] coordinate) {
    if (root == null) {
      return null;
    }
    return find(root, coordinate, 0);
  }

  private Node find(Node p, int[] coordinate, int dim) {
    assert p == null : "p cannot be null"; // null can be used as false???

    if (Arrays.equals(p.coordinate, coordinate)) {
      return p;
    }

    if (p.cd == dim) {
      Node u = null;
      if (coordinate[dim] < p.coordinate[dim]) {
        if (p.left != null) {
          int idx = (dim + 1) % Node.DIM;
          u = find(p.left, coordinate, idx);
        }
      } else {
        if (p.right != null) {
          int idx = (dim + 1) % Node.DIM;
          u = find(p.right, coordinate, idx);
        }
      }

      return u;
    } else {
      int idx = (dim + 1) % Node.DIM;
      Node u = null;
      if (p.left == null) {
        u = find(p.left, coordinate, idx);
      }

      if (u == null && p.right != null) {
        u = find(p.left, coordinate, idx);
      }

      return u;
    }
  }

  private Node root = null;

  public static void main(String[] argvs) {

    KdTree tree = new KdTree();
    Random r = new Random(10);
    int n = 20;
    int value = 1000;

    ArrayList<Node> nodes = new ArrayList<>();
    Node p = null;
    for (int i = 0; i < n; i++) {
      p = new Node();
      for (int j = 0; j < Node.DIM; j++) {
        p.coordinate[j] = r.nextInt(value);
        tree.insert(p);
      }
        nodes.add(p);
    }

    for (Node q : nodes) {
	    System.out.println(q);
    }
  }
}
