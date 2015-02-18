package plassignment
 
 
class A{
  
  val x=10;
}
 
class B extends A{
  
  val y=20;
}
 
object SubtypingOfFunctionsInScala {
 
  def Gb (b:B):Int={
    return (b.y);
  }
  
  def Ga (a:A):Int={
    return (a.x);
  }
  
  def fa (g:A=>Int):Int={
    
    val x: A=  new A();
    return g(x);
  }
  
  def fb(g:B=>Int):Int={
    
    val b:B= new B();
    return g(b);
  }
  
 def main(args: Array[String]): Unit = {
   
   SubtypingOfFunctionsInScala.fa { Ga };
   //SubtypingOfFunctionsInScala.fa { Gb }; 
   //Error as we are trying to pass A in Gb(b:B) and since all B's are A but not vice versa this cannot happen and we get an error
   
   SubtypingOfFunctionsInScala.fb { Gb };
   SubtypingOfFunctionsInScala.fb { Ga };
   
       } 
 } 