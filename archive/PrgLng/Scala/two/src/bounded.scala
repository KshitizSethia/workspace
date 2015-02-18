class A{
  val x=3
}

class B extends A{
  val y=5
}

object bounded {
	def id[T <: A](arg: T) = arg
	
	def update[E](array: Array[E], index:Int, value: E){
	  array.update(index, value)
	}
	
	def updateA(a: Array[_ >: A], index:Int, value:A){
	  a.update(index, value)
	}
	
	def updateB(b: Array[_>: B], index:Int, value:B){
	  b.update(index, value)
	}
	
	def main(args:Array[String])
	{
	  val ListA: Array[A] = new Array[A](20)
	  val ListB: Array[B] = new Array[B](20)
	  
	  val a = new A()
	  val b = new B()
	  
	  update(ListA, 1, a) //
	  update(ListA, 1, b) //
	//update(ListB, 1, a) // can't put A in place of B-|-Contravariance
	  update(ListB, 1, b) //                           | on input
	                      //                           | parameters
	  updateA(ListA, 1, a)//                           |
	  updateA(ListA, 1, b)//                           |
	//updateA(ListB, 1, a)// i/p takes super of A-|------Restriction placed
	//updateA(ListB, 1, b)// i/p takes super of A-|    | by us
	                      //                           |
	//updateB(ListA, 1, a)// can't put A in place of B-|
	  updateB(ListA, 1, b)//                           |
	//updateB(ListB, 1, a)// can't put A in place of B-| 
	  updateB(ListB, 1, b)//
	  
	}
}