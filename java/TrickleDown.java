import java.util.ArrayList;
import java.util.TreeSet;
public class TrickleDown{
    static private class Node{
	static final int CAPACITY = 10;
	double storedVolumn = 0;

	//left and right neighbor
	Node left, right;
    }


    TrickleDown(int level){
	this.level = level;
	buildGraph();
    }

    int level;
    Node root;

    private void buildGraph(){
	//build the graphc level by level
	if(root == null){
	    root = new Node();
	}
	int cnt = 1;
	ArrayList<Node> currLevel = new ArrayList<Node>();
	ArrayList<Node> nextLevel = new ArrayList<Node>();

	currLevel.add(root);

	while(cnt < level){
	    System.out.print("level: " + cnt + "\n");
	    System.out.flush();
	    nodesInLevel.add(currLevel);

	    TreeSet<Node>  nodeSet = new TreeSet<Node>();
	    for(Node p: currLevel){
		if(nextLevel.isEmpty()){
		    p.left = new Node();
		    nextLevel.add(p.left);
		}else{
		    p.left = nextLevel.get(nextLevel.size() - 1 );
		}

		p.right = new Node();
		nextLevel.add(p.right);

	    }

	    currLevel = nextLevel;
	    nextLevel = new ArrayList<Node>();
	    cnt++;

	}
    }

    public void pour(double v){
	pour(root, v);
    }

    public void pour(Node p, double v){
	final double EPS = 1e-6;
	if(p == null || Math.abs(v) < EPS ){
	    return;
	}

       	if(p.storedVolumn + v <= Node.CAPACITY){
	    p.storedVolumn += v;
	}
	else{
	    p.storedVolumn = Node.CAPACITY;
	    double s = v - (Node.CAPACITY - p.storedVolumn);

	    pour(p.left, s/2);
	    pour(p.right, s/2);
	}
    }

    public double query(int i, int j){
	return nodesInLevel.get(i).get(j).storedVolumn;
    }

    ArrayList<ArrayList<Node>>  nodesInLevel = new ArrayList<ArrayList<Node>>();

    // private void arrangeNodeInLevel(){
    // 	//store node in the graphics level by level
    // 	ArrayList<Node> currLevel = new ArrayList<Node>();
    // 	ArrayList<Node> nextLevel = new ArrayList<Node>();

    // 	currLevel.add(root)
    // 	    while(!currLevel.empty()){
    // 		nodesInLevel.add(currLevel);
    // 		TreeSet<Node>  nodeSet = new TreeSet<Node>;

    // 		for(Node p: currLevel){
    // 		    if(p == null){
    // 			continue;
    // 		    }
    // 		    if(!nodeSet.contains(p.left)){
    // 			nodeSet.add(p.left);
    // 			nextLevel.add(p.left)
    // 			    }
    // 		    if(!nodeSet.contains(p.right)){
    // 			nodeSet.add(p.right);
    // 			nextLevel.add(p.right);
    // 		    }
    // 		}

    // 		currLevel = nextLevel;
    // 		nextLevel = new ArrayList<Node>();
    // 	    }
    // }


    public static void main(String[] args){
	TrickleDown graph = new TrickleDown(100);
	graph.pour(200);

	for(ArrayList<Node> level: graph.nodesInLevel){
	    for(Node p: level){
		System.out.print( p.storedVolumn + " ");

	    }
	    System.out.println();
	}

	graph.query(0, 2);
    }
}
