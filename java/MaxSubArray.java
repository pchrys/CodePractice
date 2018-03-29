public class MaxSubArray{

    MaxSubArray(){

    }

    static public class Result{
	int i;
	int j;
	int sum;

	Result(int a, int b, int c){
	    i =a ;
	    j  = b;
	    sum = c;
	}
    }

    public static Result findMaxSubArray(int[] a){
	if(a == null || a.length == 0){
	    return Result(-1, -1, -1);
	}

	int partialSum = a[0];
	int p1, p2, p3 = 0;
	p1 = 0;
	p2 = 0;
	int maxSum = a[0];


	for(int i = 1; i < a.length; i++){

	    if(partialSum < 0){
		partialSum = a[i];
		p3 = i;
	    }
	    else{
		partialSum += a[i];
	    }

	    if(maxSum < partialSum){
		p1 = p3;
		p2 = i;
	    }
	}

	return new Result(p1, p2, maxSum);
    }

}
