class A(x: Int, y: String)
{
  val value = x
  val name = y
  
  override def toString():String = value+name
  }

class C(a:Int, b:String, c:Int) extends A(a,b){
  val age = c
  
  override def toString(): String = age + (value + name + name)
}

object Prog {
  def main(args: Array[String]){
    
  }
}