object Counter
{ 
	var count:Int = 0;
	def increment { count+=1}
	def getCount = count
	def apply(f: Int => Unit) = f(count)
  }

class Foo {
  val x=2
  var y:Int = 20
  def func(z:Int) = x+y+z
  def +(other:Foo) = y+other.y
  def setY(value: Int) { 
    Counter.increment 
    y=value}
}

object first {
  def main(args: Array[String])
  {
    val o = new Foo()
    o.setY(17)
    val p = new Foo()
    p.setY(25)
    
    print (o+p)
    print("Counter is now: " +Counter.getCount)
    
    //lambda
    print( Counter.apply((x: Int) => println(x*1000) ))
  }
  
  def printArg(x:Int) {print("Got the value: " +x)}  
}