object Part2 {

  def main(args: Array[String]) {
    print("Starting!!\n")
    assert((new OInt(2).compare(new OInt(3))) == -1, "OInt broke!")

    test()
    
    print("Done!")
  }
  
  def compareTrees[T<:Ordered[T]](tree1:OTree[T], tree2:OTree[T])
  {
    tree1.compare(tree2) match
    {
      case -1 => println("Less")
      case 0 => println("Equal")
      case 1 => println("Greater")
    }
  }

  def test() {
    val tree1 = ONode(List(OLeaf(new OInt(6))))
    val tree2 = ONode(List(OLeaf(new OInt(3)),
      OLeaf(new OInt(4)),
      ONode(List(OLeaf(new OInt(5)))),
      ONode(List(OLeaf(new OInt(6)),
        OLeaf(new OInt(7))))));
    val treeTree1: OTree[OTree[OInt]] =
      ONode(List(OLeaf(OLeaf(new OInt(1)))))
    val treeTree2: OTree[OTree[OInt]] =
      ONode(List(OLeaf(OLeaf(new OInt(1))),
        OLeaf(ONode(List(OLeaf(new OInt(2)),
          OLeaf(new OInt(2)))))))
    print("tree1: ")
    println(tree1)
    print("tree2: ")
    println(tree2)
    print("treeTree1: ")
    println(treeTree1)
    print("treeTree2: ")
    println(treeTree2)
    print("Comparing tree1 and tree2: ")
    compareTrees(tree1, tree2)
    print("Comparing tree2 and tree2: ")
    compareTrees(tree2, tree2)
    print("Comparing tree2 and tree1: ")
    compareTrees(tree2, tree1)
    print("Comparing treeTree1 and treeTree2: ")
    compareTrees(treeTree1, treeTree2)
    print("Comparing treeTree2 and treeTree2: ")
    compareTrees(treeTree2, treeTree2)
    print("Comparing treeTree2 and treeTree1: ")
    compareTrees(treeTree2, treeTree1)
  }
}

abstract class OTree[T <: Ordered[T]] extends Ordered[OTree[T]] {
  def compareChildren(children1: List[OTree[T]], children2: List[OTree[T]]): Int =
    {
      children2 match {
        case List() => children1 match {
          case List() => 0
          case (x :: xs) => 1
        }
        case (c2 :: c2s) => children1 match {
          case List() => -1
          case (c1 :: c1s) => 	if (c2.compare(c1) == 0) 
          							{ compareChildren(c1s, c2s) }
          						else 
          							{ c1.compare(c2) }
        }
      }
    }

  override def compare(that: OTree[T]) :Int =
  {
    this match
    {
      case OLeaf(l1) => that match
      {
        case ONode(x::xs) => -1
        case OLeaf(l2) => l1.compare(l2)
      }
      case ONode(children1) => that match
      {
        case ONode(children2) => compareChildren(children1, children2)
        case OLeaf(l2) => 1
      }
    }
  }
}

case class OLeaf[T <: Ordered[T]](value: T) extends OTree[T] {
}

case class ONode[T <: Ordered[T]](children: List[OTree[T]]) extends OTree[T] {
}

class OInt(xIn: Int) extends Ordered[OInt] {
  var x: Int = xIn
  override def compare(that: OInt) = math.signum(this.x - that.x)
  override def toString() = x.toString
}
