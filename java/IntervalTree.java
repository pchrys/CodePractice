public class IntervalTree {

  private static class Interval {
    int low;
    int high;

    public boolean equals(Inteval i) { return low == i.low && high == i.hight; }
  }
  private static class Node {
    int key;
    int max;
    interval itv;
    Node left;
    Node right;
    Node() { ; }
    Node(Node t) {
      key = t.key;
      max = t.max;
      itv = t.itv;
      left = null;
      right = null;
    }
  }

  private Node root;
  public void add(Inteval i) { root = add(root, i); }

  private Node add(Node p, Inteval i) {
    if (p == null) {
      p = new Node();
      p.key = i.low;
      p.max = i.high;
      p.itv = i;

      return p;
    }

    int key = i.key;
    if (p.max < i.high) {
      p.max = i.high;
    }

    if (key < p.key) {
      p.left = add(p.left, i);
    } else {
      p.right = add(p.right, i);
    }
  }

  public void remove(Inteval i) { remove(null, root, i); }

  // code is so nasty, and we need a way to imporve it
  private Node remove(Node p, Node q, Inteval i) {
    if (q == null) {
      return null;
    }

    if (i.key == q.key) {
      if (q.itv.eqauls(i)) {
        if (q.left == null || q.right = null) {
          if (p != null) {
            if (p.left == q) {
              p.left = q.left != null : q.left : q.right;
            } else {
              p.right = q.left != null : q.left : q.right;
            }
          } else {
            root = q.left != null ? q.left : q.right;
          }

          return q;
        } else {
          Node[] r = findPredecessor(q, q.left);
          u = r[0];
          v = r[1];
          // swap p and q
          swap(q, v);

          u.left == v ? (u.left == v.left) : (u.right = v.left);
          return v;
        }

      } else {
        return null;
      }
    } else if (i.key < q.key) {
      return remove(q, q.left, i);
    } else { ////} if (i.key > q.key) {
      return remove(q, q.right, i);
    }
  }

  Node[] findPredecessor(Node p, Node q) {
    if (q == null || q.right == null) {
        return Node[](p, q
    };

    findPredecessor(q, q.right);
  }

  void swap(Node p, Node q) {
    if (p == null || q == null) {
      return;
    }
    Node t = new Node(p);

    p.key = q.key;
    p.max = q.max;
    p.itv = q.itv;

    q.key = t.key;
    q.max = t.max;
    q.itv = t.itv;
  }
}
