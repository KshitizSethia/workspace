abstract class Tree[T]

case class Leaf[T](value:T) extends Tree[T]{
  override def toString():String = "("+value+")"
}

case class Node[T](value:T, left:Tree[T], right:Tree[T]) extends Tree[T]{
  override def toString() = "(" + value.toString() + left.toString() + right.toString() + ")"
}

object GenericTreeRunner {
  def main(args: Array[String])
  {
    var myTree = Node(
						3,
						Node(2, Leaf(1), Leaf(5)),
						Leaf(6)
						)
	printTree(myTree)
	println()
	
	print(myTree)
	println()
	
	//Error: myTree can only take Tree[Int] now
	//myTree = Node(
	//					"3.0",
	//					Node("2.0", Leaf("1.0"), Leaf("5.0")),
	//					Leaf("6.0")
	//					)
	
	var myNewTree = Node(
						"3.0",
						Node("2.0", Leaf("1.0"), Leaf("5.0")),
						Leaf("6.0")
						)
	printTree(myNewTree)
  }
  
  def printTree[T](ip: Tree[T]):Unit
  ={
    ip match{
      case Node(value, left, right) 
      				=> print("("); 
      					print(value); 
      					printTree(left);
      					printTree(right);
      					print(")");
      case Leaf(value) 
      				=> print("(");
      					print(value); 
      					print(")");
    }
  }
}