class One {
  val x = 26
  override def toString() = "One: " +x
}

class Two extends One {
  val y = 99
  override def toString() = "Two: " + x + ", " + y
}

class MyList[+T](lis: List[T]){
  val l = lis
  def get = l
  
  def cons[B>:T](x: B) = new MyList[B](x::lis)
  
  //Error
  //def cons(x: T) = new MyList[T](x::lis) 
  //covariant type T occurs in contravariant position in type T of value x	variant.scala	/two/src	line 17	Scala Problem

  def hd = l match{
    case (x::xs) => x
    case List() => throw new Exception
  }
  
  def tl = l match{
    case (x::xs) => xs
    case List() => throw new Exception
  }
  
  override def toString() = l + ""
}

object variant {

  def foo(l:MyList[One]){
    println(l)
  }
  
  def main(args: Array[String]){
    val ListOne = new MyList(List(new One()))
    val hdOne = ListOne.hd
   
    foo(ListOne)
    //List(One: 26)
    println(hdOne)
    
    val listTwo = new MyList(List(new Two()))
    val hdTwo = listTwo.hd
    val consList = ListOne.cons(hdTwo)
    println(consList)
    println(listTwo)
    println(hdTwo)
  }
}