//For Q5.d in Homework2, Programming Languages

import java.util.ArrayList;
import java.util.List;

class Animal{
	void Breathe(){System.out.println("breathing");}
}

class Dog extends Animal
{
	void Bark() {System.out.println("woof woof!");}
}

class Cat extends Animal{
	void Meow(){System.out.println("Meow!");}
}
	
class Labrador extends Dog{
	void SthLabradorsDo(){System.out.println("I'm a labrador");}
}

public class CollectionVariance {
	
	public static void main(String[] args)
	{
		ArrayList<Dog> dogs = new ArrayList<Dog>();
		Cat cat = new Cat();
		cat.Meow();
		
		//5.d.i) Illustrating why List<Dog> is not <: List<Animal>
		//AddAnimal(dogs, cat);
		//Dog d = dogs.get(0);
		
		//5.d.ii) Example of generic version for collections?
		GenericAdd(dogs, new Labrador());
		Dog d = GenericGet(dogs);
		d.Bark();
	}
	
	public static void AddAnimal(List<Animal> L, Animal a){
		L.add(a);
	}
	
	public static <T> void GenericAdd(List<? super T> L, T a){
		L.add(a);
	}
	
	public static<T> T GenericGet(List<? extends T> L){ return L.get(0);}
}
