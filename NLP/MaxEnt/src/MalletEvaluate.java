import java.io.IOException;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;

public class MalletEvaluate {

	/**
	 * @param args
	 * @throws IOException
	 */
	public static void main(String[] args) throws IOException {

		ArrayList<Character> predictedTags = new ArrayList<>();
		for (String tag : Files.readAllLines(
				Paths.get(Settings.mallet_results), Charset.defaultCharset())) {
			if(tag.length()==0) continue;
			predictedTags.add(tag.charAt(0));
		}

		ArrayList<Character> actualTags = new ArrayList<>();
		for (String line : Files.readAllLines(
				Paths.get(Settings.test_data_features),
				Charset.defaultCharset())) {
			String[] parts = line.split(" ");
			actualTags.add(parts[parts.length - 1].charAt(0));
		}

		System.out.println(actualTags);
		System.out.println(predictedTags);
		Summary summary = new Summary();
		Helper.calculateConfusionMatrix(actualTags, predictedTags, summary);
		System.out.println(summary.toString());
	}

}
