import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;

import opennlp.maxent.BasicEventStream;
import opennlp.maxent.GIS;
import opennlp.maxent.GISModel;
import opennlp.maxent.PlainTextByLineDataStream;
import opennlp.maxent.io.GISModelWriter;
import opennlp.maxent.io.SuffixSensitiveGISModelReader;
import opennlp.maxent.io.SuffixSensitiveGISModelWriter;
import opennlp.model.EventStream;

class Chunk {
	int start, end;

	public Chunk(int start, int end) {
		this.start = start;
		this.end = end;
	}

	@Override
	public int hashCode() {
		String desc = String.valueOf(start) + "_" + String.valueOf(end);
		return desc.hashCode();
	}

	@Override
	public boolean equals(Object other) {
		Chunk oth = (Chunk) other;
		if (oth.start == this.start && oth.end == this.end)
			return true;
		return false;
	}
}

public class MaxEnt {

	/**
	 * @param args
	 * @throws IOException
	 */
	public static void main(String[] args) throws IOException {
		ArrayList<Summary> summaries = new ArrayList<>();
		for (int round = 1; round <= 4; round++) {

			if (round == 2)
				Settings.usePrevWord = true;
			if (round == 3)
				Settings.use2PrevWord = true;
			if (round == 4)
				Settings.useCapitalization = true;

			Summary summary = new Summary();

			summary.descriptor = roundDetails(round);

			CreateFeatures.main(null);
			trainModel();

			ArrayList<Character> actualTags = new ArrayList<>();
			// Arrays.asList('I','B', 'B','I', 'I'));
			ArrayList<Character> predictedTags = new ArrayList<>();
			// Arrays.asList('I','I', 'B','I','O'));

			testModel(actualTags, predictedTags, summary);

			Helper.calculateConfusionMatrix(actualTags, predictedTags, summary);

			summaries.add(summary);
		}

	}

	private static String roundDetails(int roundNumber) {
		StringBuilder sbr = new StringBuilder();
		sbr.append("Round ");
		sbr.append(roundNumber);
		sbr.append(": previous word=");
		sbr.append(Settings.usePrevWord);
		sbr.append(", 2 previous word=");
		sbr.append(Settings.use2PrevWord);
		sbr.append(", use capitalization info=");
		sbr.append(Settings.useCapitalization);
		return sbr.toString();
	}

	private static void testModel(ArrayList<Character> actualTags,
			ArrayList<Character> predictedTags, Summary summary)
			throws IOException {

		GISModel model = (GISModel) new SuffixSensitiveGISModelReader(new File(
				Settings.modelFile)).getModel();
		float mismatches = 0;
		float totalLines = 0;
		for (String line : Files.readAllLines(
				Paths.get(Settings.test_data_features),
				Charset.defaultCharset())) {
			totalLines++;
			// extract features
			String[] features = line.split(" ");

			// remove tag and store
			java.util.ArrayList<String> temp = new ArrayList<String>(
					Arrays.asList(features));
			char actual_tag = temp.remove(temp.size() - 1).charAt(0);
			actualTags.add(actual_tag);
			features = (String[]) temp.toArray(new String[temp.size()]);

			// predict tag and store
			char predicted_tag = model.getBestOutcome(model.eval(features))
					.charAt(0);
			predictedTags.add(predicted_tag);

			// update mismatches
			if (actual_tag != predicted_tag) {
				/*
				 * System.out.println("mismatch at " +
				 * String.valueOf(totalLines) + ", actual: " + actual_tag +
				 * ", predicted: " + predicted_tag);
				 */
				mismatches++;
			}
		}

		summary.accuracy = (mismatches / totalLines);
	}

	private static void trainModel() {
		try {
			FileReader featureReader = new FileReader(new File(
					Settings.training_data_features));
			EventStream eventStream = new BasicEventStream(
					new PlainTextByLineDataStream(featureReader));
			GISModel model = GIS.trainModel(eventStream, 100, 4);
			File outputFile = new File(Settings.modelFile);
			GISModelWriter modelWriter = new SuffixSensitiveGISModelWriter(
					model, outputFile);
			modelWriter.persist();

		} catch (Exception e) {
			System.out.println("Error in creating model: " + e.toString());
		}
	}

}
