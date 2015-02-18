class Co_variantList[+T](list: List[T]){
  def getList() = list
  def getFirst():T= list(0)
  //def addFirst(t: T):Unit = list.:::(t::Nil)	//covariant type T occurs in contravariant position in type T of value t
  //def addList(L: List[T]):Unit = list.:::(L)	//covariant type T occurs in contravariant position in type List[T] of value L
}

class Contra_variantList[-T](list: List[T]){
  //def getFirst():T= list(0)					//contravariant type T occurs in covariant position in type ()T of method getFirst
  //def getList() = list						//contravariant type T occurs in covariant position in type ()List[T] of method getList
  
  def addFirst(t: T):Unit = list.:::(t::Nil)
  def addList(L: List[T]):Unit = list.:::(L)
}

object CollectionVariance {
	def main(args: Array[String]){
	  
	var x = new X()
	var y = new Y()
	
	//-----Generic Examples
	//-----Covariant List
	var genListX = new Co_variantList(List(x));
	var genListY = new Co_variantList(List(y));
	
	var x2 = genListX.getFirst()
	x2 = genListY.getFirst()
	var y2 = genListY.getFirst()
	//y2 = genListX.getFirst()									//type mismatch;  found   : X  required: Y
	
	takesCOVariantListX(genListX)
	takesCOVariantListX(genListY)
	//takesCOVariantListY(genListX)								//type mismatch;  found   : Co_variantList[X]  required: Co_variantList[Y]
	takesCOVariantListY(genListY)
	
	//genListX = genListX.getList()								//type mismatch;  found   : List[X]  required: Co_variantList[X]
	//genListY = genListX.getList()									//type mismatch;  found   : List[X]  required: Co_variantList[Y]
	
	//-----Contravariant List
	//genListX = new Contra_variantList(List(x));				//type mismatch;  found   : Contra_variantList[Y]  required: Co_variantList[Y]
	var contraVariantListX = new Contra_variantList(List(x));
	var contraVariantListY = new Contra_variantList(List(y));
	
	contraVariantListX.addFirst(x)
	contraVariantListX.addFirst(y)
	//contraVariantListY.addFirst(x)							//type mismatch;  found   : X  required: Y
	contraVariantListY.addFirst(y)
	
	contraVariantListX.addList(List(x))
	contraVariantListX.addList(List(y))
	//contraVariantListY.addList(List(x))						//type mismatch;  found   : X  required: Y
	contraVariantListY.addList(List(y))
	
	contraVariantListX = returnsCONTRAVariantListX()
	contraVariantListY = returnsCONTRAVariantListX()
	//contraVariantListX = returnsCONTRAVariantListY()			//type mismatch;  found   : Contra_variantList[Y]  required: Contra_variantList[X]
	contraVariantListY = returnsCONTRAVariantListY()
	}
	
  def takesCOVariantListX(obj: Co_variantList[X]):Unit ={ }
  
  def takesCOVariantListY(obj: Co_variantList[Y]):Unit ={ }
  
  def returnsCONTRAVariantListX():Contra_variantList[X]
  ={ new Contra_variantList(List(new X())) }
  
  def returnsCONTRAVariantListY():Contra_variantList[Y]
  ={ new Contra_variantList(List(new Y())) }
}
