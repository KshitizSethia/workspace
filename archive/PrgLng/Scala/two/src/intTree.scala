abstract class intTree extends Ordered[intTree]{
  def sum():Int
  def add(x:Int)
}

case class intLeaf(in:Int) extends intTree{
  var value = in
  def sum() = value
  def add(x:Int) {value = value+x}
  override def compare(that: intTree) = 
    that match{
    case intLeaf(value) => math.signum(this.value - value)
    case intNode => -1
  }
}

case class intNode(in:Int, left:intTree, right:intTree) extends intTree{
  var value = in
  def sum() = value +left.sum() +right.sum()
  def add(x:Int) {value = value+x; left.add(x); right.add(x)}
  override def compare(that: intTree) =
  {
    that match
    {
      case intLeaf(value) => +1
      case intNode(value, left, right) => math.signum(this.value -value) * this.left.compare(left) * this.right.compare(right)
    }
  }
}

object IntTreeRunner {
  def main(args: Array[String])
  {
    val myTree = intNode(
						3,
						intNode(2, intLeaf(1), intLeaf(5)),
						intLeaf(6)
						)
	val myNewTree = intNode(
						4,
						intNode(2, intLeaf(1), intLeaf(5)),
						intLeaf(6)
						)
	printTree(myTree)
	println()
	myTree.add(1)
	printTree(myTree) //prints old tree!! why??
	println()
	print(myTree.sum())
	println()
	print(myTree.compare(myNewTree))
  }

  def printTree(ip: intTree):Unit
  ={
    ip match{
      case intNode(value, left, right) 
      				=> print("("); 
      					print(value); 
      					printTree(left);
      					printTree(right);
      					print(")");
      case intLeaf(value) 
      				=> print("(");
      					print(value); 
      					print(")");
    }
  }
}