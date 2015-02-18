
public interface Foo<T> {
	
	public int bar();
	
	public <E> boolean baz(Foo<E> other);

}
