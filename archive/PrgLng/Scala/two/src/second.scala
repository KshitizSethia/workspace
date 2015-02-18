//object second {
//	def main(args: Array[String])
//	{
//	  val a = new A(33, "Thirty three")
//	  println(a)
//	  
//	  val b = new B(22, "Twenty two", 222)
//	  f(b)
//	}
//	
//	def f(a: A) {println(a)}
//}
//
//class A(x: Int, y: String){
//  val value = x
//  val name = y
//  
//  override def toString(): String = value + name
//}
//
//class B(a:Int, b: String, c:Int) extends A(a,b){
//  val age = c
//  
//  override def toString(): String = age +(name+value+name)
//}