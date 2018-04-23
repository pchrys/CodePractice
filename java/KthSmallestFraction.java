import java.util.List;
import java.util.ArrayList;

public class KthSmallestFraction{

    private static class Pair{
	int first;
	int second;

	Pair(int a, int b){
	    first =a ;
	    second = b;
	}
    }

    int[] kthSmallestFraction(int[] input, int k){
	if(input == null ||  input.length == 1){
	    System.out.println("Wrong input ");
	    return null;
	}

	List<Pair> fractions = new ArrayList<Pair>();

	for(int i = 1; i < input.length; i++){

	    for(int j = 0; j < i; j++){

		Pair tmp = new Pair(j, i);

		fractions.add(tmp);
	    }
	}


	Collections.sort(fractions, (a, b) -> return Double.compares(a.first/(double)a.second,
								     b.first/(double).b.second));


	assert k >= 0 && k < fractions.length;

	return new int[2]{fractions.get(k-1).first, fractions.get(k-1).second};
    }

}
