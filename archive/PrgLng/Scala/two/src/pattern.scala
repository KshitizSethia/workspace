object pattern {
  
	def sum(param:List[Int]):Int=
	{
	  param match
	  {
	    case List() => 0
	    case (x::xs) => x + sum(xs)
	  }
	}
	
	def main(args: Array[String])
	{
	  val l1 = List(1,2,3,4)
	  println(sum(l1))
	  val l2 = l1.map(x=> x*3)
	  println(l1)
	  println(l2)
	  
	  val filtered = l1.filter(p=> p>2)
	  println(filtered)
	}
}
