import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;

class Test{
	public final Character[] sequence;
	public final int chunks;
	
	public Test(Character[] seq, int chunks){
		this.sequence = seq;
		this.chunks = chunks;
	}
}

public class FillChunkTester {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		
		ArrayList<Test> tests = new ArrayList<>();
		/*tests.add(new Test(new Character[]{'B','B','B'}, ));
		tests.add(new Test(new Character[]{'B','B','I'}, ));
		tests.add(new Test(new Character[]{'B','B','O'}, ));
		tests.add(new Test(new Character[]{'B','I','B'}, ));
		tests.add(new Test(new Character[]{'B','I','I'}, ));
		tests.add(new Test(new Character[]{'B','I','O'}, ));
		tests.add(new Test(new Character[]{'B','O','B'}, ));
		tests.add(new Test(new Character[]{'B','O','I'}, ));
		tests.add(new Test(new Character[]{'B','O','O'}, ));
		*/
		tests.add(new Test(new Character[]{'I','B','B'}, 3));
		tests.add(new Test(new Character[]{'I','B','I'}, 2));
		tests.add(new Test(new Character[]{'I','B','O'}, 2));
		tests.add(new Test(new Character[]{'I','I','B'}, 2));
		tests.add(new Test(new Character[]{'I','I','I'}, 1));
		tests.add(new Test(new Character[]{'I','I','O'}, 1));
		//tests.add(new Test(new Character[]{'I','O','B'}, ));
		tests.add(new Test(new Character[]{'I','O','I'}, 2));
		tests.add(new Test(new Character[]{'I','O','O'}, 1));
		//tests.add(new Test(new Character[]{'O','B','B'}, ));
		//tests.add(new Test(new Character[]{'O','B','I'}, ));
		//tests.add(new Test(new Character[]{'O','B','O'}, ));
		tests.add(new Test(new Character[]{'O','I','B'}, 2));
		tests.add(new Test(new Character[]{'O','I','I'}, 1));
		tests.add(new Test(new Character[]{'O','I','O'}, 1));
		//tests.add(new Test(new Character[]{'O','O','B'}, ));
		tests.add(new Test(new Character[]{'O','O','I'}, 1));
		tests.add(new Test(new Character[]{'O','O','O'}, 0));
		
		for( Test test :tests){
			HashSet<Chunk> chunks = new HashSet<>();
			Helper.fillChunks(chunks, new ArrayList<Character>(Arrays.asList(test.sequence)));
			assert(chunks.size()==test.chunks);
		}
		System.out.println("All tests passed.");
	}

}
