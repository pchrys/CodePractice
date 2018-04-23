import java.util.Set;
import java.util.TreeSet;
import java.util.List;
import java.util.ArrayList;

public class TestBipartieGraph {
  int[][] graph = {{}};
  boolean[] visited = new boolean[graph.length];
  int[] nodes = new int[graph.length];

  TestBipartieGraph() {
    for (int i = 0; i < nodes.length; i++) {
      nodes[i] = i;
    }
  }

  @SuppressWarnings("Unchecked")
  boolean isBipartieGraph(int[][] graph, int numOfVisitedNodes) {
    if (graph == null || graph.length == 1) {
      return true;
    }

    if (numOfVisitedNodes == nodes.length) {
      return true;
    }

    List<Integer> currentQueue = new ArrayList<>();
    List<Integer> nextQueue = new ArrayList<>();

    // Object[] partitions = {new TreeSet<Integer>(), new TreeSet<Integer>()};
    List<TreeSet<Integer>> partitions = new ArrayList<TreeSet<Integer>>();
    partitions.add(new TreeSet<>());
    partitions.add(new TreeSet<>());

    // new TreeSet<Integer>(), new TreeSet<Integer>()};

    for (int i = 0; i < graph.length; i++) {
      if (visited[i] == false) {
        currentQueue.add(i);
        break;
      }
    }

    int visitedNodes = 0;
    int idx = 0;
    while (!currentQueue.isEmpty()) {
      for (Integer i : currentQueue) {
        visited[i] = true;
        visitedNodes++;

        //((TreeSet<Integer>)partitions[idx]).add(i);
        partitions.get(idx).add(i);

        for (Integer j : graph[i]) {
          if (visited[j] == false) {
            nextQueue.add(j);
          } else {
            if (partitions.get(idx).contains(j)) {
              return false;
            }
          }
        }
      }

      idx = (idx + 1) & 0x1;
      currentQueue = new ArrayList<Integer>(nextQueue);
      nextQueue.clear();
    }

    visitedNodes += numOfVisitedNodes;

    return isBipartieGraph(graph, visitedNodes);
  }

  public static void main(String[] args) {
    // System.out.println(c);

    // List<Integer>[] partitions = {new ArrayList<Integer>(),
    //                               new ArrayList<Integer>()}; // wrong
  }
}
