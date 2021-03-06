import Jet.*;
import Jet.Tipster.*;
import java.util.Vector;
import java.io.IOException;

public class jet {

	/**
	 * @param args
	 * @throws IOException 
	 */
	public static void main(String[] args) throws IOException {
		
		System.out.println("Starting JET");
		
		JetTest.initializeFromConfig("C:/Cloud/Copy/devtools/JET/props/tinyParse.jet");
		
		String text = "<TEXT>the dog chased the cat</TEXT>";
		Document doc = new Document(text);
		Control.processDocument(doc, null, true, 1);
		Vector<Annotation> vec = doc.annotationsOfType("constit");
		
		for(int index=0; index<vec.size();index++)
		{
			Annotation annotation = vec.get(index);
			System.out.println("Constit " 
								+annotation.get("cat")
								+" over "
								+doc.text(annotation));
		}
	}

}
