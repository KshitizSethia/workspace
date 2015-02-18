import java.util.ArrayList;

class X{
	int x=3;
	@Override public String toString()
	{ return String.valueOf(x);}
}

class Y extends X{
	int y=4;
	@Override public String toString()
	{ return String.valueOf(x) +"," +String.valueOf(y);}
}

public class Invariance {

	public static void main(String[] args)
	{
		ArrayList<X> xList = new ArrayList<X>();
		ArrayList<Y> yList = new ArrayList<Y>();
		
		X x = new X();
		Y y = new Y();
		
		xList.add(x);
		yList.add(y);
		
		//-----List passing
		takesListX(xList);
		//takesListX(yList);					//The method takesListX(ArrayList<X>) in the type Invariance is not applicable for the arguments (ArrayList<Y>)
		takesListX_AndBelow(yList);
		//takesListY(xList);					//The method takesListY(ArrayList<Y>) in the type Invariance is not applicable for the arguments (ArrayList<X>)
		takesListY(yList);
		
		//-----List returning
		ArrayList<X> xList2 = returnsListX();
		//xList2 = returnsListY();				//cannot convert from ArrayList<Y> to ArrayList<X>
		ArrayList<Y> yList2 = returnsListY();
		//yList2 = returnListX();				//The method returnListX() is undefined for the type Invariance

		
		//-----element passing
		takesX(x);
		takesX(y);
		//takesY(x);							//The method takesY(Y) in the type Invariance is not applicable for the arguments (X)
		takesY(y);
		
		//-----element returning
		X x2 = returnsX();
		x2 = returnsY();
		Y y2 = returnsY();
		//y2 = returnsX();						//cannot convert from X to Y
		
		System.out.println("Hello");
	}
	
	private static ArrayList<Y> returnsListY() {
		return new ArrayList<Y>();
	}

	private static ArrayList<X> returnsListX() {
		return new ArrayList<X>();
	}

	private static X returnsX() {
		return new X();
	}

	private static Y returnsY() {
		return new Y();
	}
	
	private static void takesX(X obj)
	{
		System.out.println(obj.x);
	}
	
	private static void takesY(Y obj)
	{
		System.out.println(obj.y);
	}
	
	private static void takesListX(ArrayList<X> list)
	{
		for (X x : list)
		{
			System.out.println(x.x);
		}
			
	}

	private static <T extends X> void takesListX_AndBelow(ArrayList<T> list)
	{
		for (X x : list)
		{
			System.out.println(x.x);
		}
			
	}
	
	private static void takesListY(ArrayList<Y> list)
	{
		for(Y y :list)
		{
			System.out.println(y.y);
		}
	}
}
