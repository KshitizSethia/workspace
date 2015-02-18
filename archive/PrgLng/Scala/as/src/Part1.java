import java.util.ArrayList;

class ComparableList<T extends Comparable<T>> extends ArrayList<T> implements
		Comparable<ComparableList<T>> {

	/**
	 * Why do we need this?
	 */
	private static final long serialVersionUID = 1L;

	@Override
	public int compareTo(ComparableList<T> other) {

		if (this.size() <= other.size()) {
			for (int index = 0; index < this.size(); index++) {
				if (this.get(index).compareTo(other.get(index)) < 1) {
					return -1;
				} else if (this.get(index).compareTo(other.get(index)) > 1) {
					return 1;
				}
			}

			if (this.size() < other.size())
				return -1;
			else
				return 0;
		} else
			return other.compareTo(this);
	} 

	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("[ ");
		for (T item : this) {
			sb.append(item + ", ");
		}
		sb.append("]");
		return sb.toString();
	}
}

class A implements Comparable<A> {
	Integer x;

	public A(Integer x) {
		this.x = x;
	}
	
	public Integer getVal()
	{ return x;}

	@Override
	public int compareTo(A other) {
		return Integer.signum(this.getVal() - other.getVal());
		
	}

	@Override
	public String toString() {
		return this.x.toString();
	}
}

class B extends A {
	Integer y;

	public B(Integer x, Integer y) {
		super(x);
		this.y = y;
	}
	
	@Override
	public Integer getVal()
	{ return x+y;}

	@Override
	public String toString() {
		return "{" + this.x + "," + this.y + "}";
	}
}

public class Part1 {
	public static void main(String[] args) {
		A a1 = new A(6);
		B b1 = new B(2, 4);
		B b2 = new B(3, 5);

		assert (a1.compareTo(b1) == 0); // returns 0, since 6 = (2+4)
		assert (a1.compareTo(b2) ==-1); // returns -1, since 6 < (3+5)
		assert (b1.compareTo(a1) == 0); // returns 0, since (2+4) = 6
		assert (b2.compareTo(a1) == 1); // returns 1, since (3+5) > 6
		assert (b1.compareTo(b2) ==-1); // returns -1, since (2+4) < (3+5)

		test();
		
		System.out.println("Success!");
	}

	// read this:
	// http://stackoverflow.com/questions/4409100/how-to-make-a-java-generic-method-static
	static <T extends Comparable<T>> void addToCList(T z, ComparableList<T> L) {
		L.add(z);
	}

	static void test() {
		ComparableList<A> c1 = new ComparableList<A>();
		ComparableList<A> c2 = new ComparableList<A>();
		for(int i = 0; i < 10; i++) {
			addToCList(new A(i), c1);
			addToCList(new A(i), c2);
		}
		
		addToCList(new A(12), c1);
		addToCList(new B(6,6), c2);
		addToCList(new B(7,11), c1);
		addToCList(new A(13), c2);
		
		System.out.print("c1: ");
		System.out.println(c1);
		System.out.print("c2: ");
		System.out.println(c2);
		switch (c1.compareTo(c2)) {
			case -1:
				System.out.println("c1 < c2");
				break;
			case 0:
				System.out.println("c1 = c2");
				break;
			case 1:
				System.out.println("c1 > c2");
				break;
			default:
				System.out.println("Uh Oh");
				break;
		}
		}
}
