import java.util.Collections;
import java.util.List;
import java.util.ArrayList;
import java.util.Comparator;

class ClosetPair {

  private static class Point {
    float x = 0;
    float y = 0;
  }

  private float distance(Point pt1, Point pt2) {
      return (float)Math.sqrt((pt1.x - pt2.x) * (pt1.x - pt2.x) +
                (pt1.y - pt2.y) * (pt1.y - pt2.y));
  }

  private ArrayList<Point> points = new ArrayList<>();

  float findClosestPair(List<Point> points) {

    if (points.size() <= 1) {
      return Float.MAX_VALUE;
    }

    if (points.size() == 2) {
	return distance(points.get(0), points.get(1));
    }

    // Collections.sort(points, new Comparator<Point>() {
    // 	    public int compare(Point a, Point b) { if( a.y < b.y){
    // 		    return -1;
    // 		}
    // 		else if(a.y > b.y){
    // 		    return 1;
    // 		}else{
    // 		    return 0;
    // 		}; }
    // });


    Collections.sort(points,
	    ( a,  b)-> { if( a.y < b.y){
		    return -1;
		}
		else if(a.y > b.y){
		    return 1;
		}else{
		    return 0;
		} }

    );


    int idx = points.size() / 2;

    List<Point> p1 = points.subList(0, idx);
    List<Point> p2 = points.subList(idx, points.size());

    float d1 = findClosestPair(p1);
    float d2 = findClosestPair(p2);
    float d = d1;
    if (d > d2) {
      d = d2;
    }

    int r = 0;
    for (Point pt : p1) {
	while (r < p2.size() && pt.y - p2.get(r).y > d) {
        r++;
      }

	while (r < p2.size() && Math.abs(p2.get(r).y - pt.y) <= d) {
	    if (d > distance(pt, p2.get(r))) {
		d = distance(pt, p2.get(r));
        }

        r++;
      }
    }

    return d;
  }
}
