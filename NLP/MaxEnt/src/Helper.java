import java.util.ArrayList;
import java.util.HashSet;

public class Helper {

	public static void calculateConfusionMatrix(
			ArrayList<Character> actualTags,
			ArrayList<Character> predictedTags, Summary summary) {

		HashSet<Chunk> actualChunks = new HashSet<>();
		HashSet<Chunk> predictedChunks = new HashSet<>();

		fillChunks(actualChunks, actualTags);
		fillChunks(predictedChunks, predictedTags);

		summary.predicted_positives = predictedChunks.size();
		summary.actual_positives = actualChunks.size();

		HashSet<Chunk> intersection = new HashSet<>(actualChunks);
		intersection.retainAll(predictedChunks);
		summary.true_positives = intersection.size();

		/*
		 * System.out.println("true positives: " +
		 * String.valueOf(true_positives));
		 * System.out.println("actual positives: " +
		 * String.valueOf(actual_positives));
		 * System.out.println("predicted_positives: " +
		 * String.valueOf(predicted_positives));
		 */
		summary.recall = summary.true_positives / summary.actual_positives;
		summary.precision = summary.true_positives
				/ summary.predicted_positives;
		/*
		 * System.out.println("Recall: " + String.valueOf(recall));
		 * System.out.println("Precision: " + String.valueOf(precision));
		 */
		summary.f1Score = 2 * (summary.precision * summary.recall)
				/ (summary.precision + summary.recall);
		/*
		 * System.out.println("F1 score: " + String.valueOf(f1_score));
		 */
	}

	public static void fillChunks(HashSet<Chunk> chunkSet,
			ArrayList<Character> tagList) {
		tagList.add('O');
		Character[] tagArray = tagList.toArray(new Character[tagList.size()]);

		for (int index = 0; index < tagArray.length; index++) {
			if (tagArray[index] == 'I' || tagArray[index] == 'B') {
				int start = index;
				do {
					index++;
				} while (tagArray[index] == 'I');
				chunkSet.add(new Chunk(start, index - 1));
				index--;
			}
		}
	}

}
