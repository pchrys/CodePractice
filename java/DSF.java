
import java.util.ArrayList;
import java.util.ArrayDeque;

class Node {
  Node() {
    visited = false;
    adjList = new ArrayList<Node>();
  }

  ArrayList<Node> adjList;
  boolean visited;
}

class DSF {
  public void depthSearchFirst(Node v) {
    if (!v) {
      return;
    }

    for (Node child : v.adjList()) {
      if (!child.visited) {
        depthSearchFirst(child)
      }
    }

    child.visited = true;
  }
}

class BSF {
  public void breadthSearchFirst(Node v) {
    if (!v) {
      return;
    }

    ArrayDeque<Node> queue;
    queue.push_back(v);

    while (!queue.empty()) {
      Node v = queue.pop();
      v.visited = true;

      for (Node c : v.adjList()) {
        if (!c.visited) {
          // push adjlist of c into queue
          queue.addLast(c);
        }
      }
    }
  }
}
