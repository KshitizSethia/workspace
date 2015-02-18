class X{
	val x = 3
	override def toString() = x.toString
}

class Y extends X{
	val y=4
	override def toString() = x.toString +"," +y.toString
}

object Variance {

  def main(args: Array[String])
  {
    	
	val x = new X();
	val y = new Y();
	
	val xList = List(x);
	val yList = List(y);
	
	//-----List passing
	takesListX(xList);
	takesListX(yList);							//Works in Scala, not in Java
	//takesListX_AndBelow(yList);
	//takesListY(xList);						//type mismatch;  found: List[X]  required: List[Y]
	takesListY(yList);
	
	//-----List returning
	var xList2 = returnsListX();
	xList2 = returnsListY();					//Works in Scala, not in Java
	var yList2 = returnsListY();
	//yList2 = returnsListX();					//type mismatch;  found: List[X]  required: List[Y]

	
	//-----element passing
	takesX(x);
	takesX(y);
	//takesY(x);								//type mismatch;  found: X  required: Y
	takesY(y);
	
	//-----element returning
	var x2 = returnsX();
	x2 = returnsY();						
	var y2 = returnsY();
	//y2 = returnsX();							//type mismatch;  found: X  required: Y
	
	System.out.println("Executed");
  }
  
  def returnsListY(): List[Y] = List(new Y());

  def returnsListX(): List[X] = List(new X());

  def returnsX() =  new X()

  def returnsY() =  new Y()

  def takesX(obj: X) = println(obj.x)

  def takesY(obj: Y) = println(obj.y)
		
  def takesListX(obj: List[X]):Unit
  ={
	obj match
	{
	  case Nil => 
	  case (x::xs) => println(x); takesListX(xs)
	}
  }
  
  def takesListY(obj: List[Y]):Unit
  ={
	obj match
	{
	  case Nil => 
	  case (x::xs) => println(x); takesListX(xs)
	}
  }
}