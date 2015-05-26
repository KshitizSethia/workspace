import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.charset.Charset;
import java.nio.file.*;
import java.util.Formatter;
import java.util.HashMap;

import org.apache.commons.collections4.queue.*;

public class CreateMalletFeatures {

	/**
	 * @param args
	 * @throws IOException
	 */
	public static void main(String[] args) throws IOException {

		writeFeatures(Paths.get(Settings.training_data_raw), new Formatter(
				new File(Settings.training_data_features_mallet)));
		writeFeatures(Paths.get(Settings.test_data_raw), new Formatter(
				new File(Settings.test_data_features_mallet)));
	}

	private static void writeFeatures(Path input, Formatter out)
			throws FileNotFoundException, IOException {
		int contextSize = 3;
		CircularFifoQueue<String> lastFewWords = new CircularFifoQueue<>(contextSize);
		CircularFifoQueue<String> lastFewPOSs = new CircularFifoQueue<>(contextSize);
		CircularFifoQueue<String> lastFewTags = new CircularFifoQueue<>(contextSize);
		HashMap<String, String> features = new HashMap<>();

		for (String entry : Files.readAllLines(input, Charset.defaultCharset())) {

			// extract data
			String[] data = entry.split(" ");
			if (entry.length() == 0) {
				lastFewWords.clear();
				lastFewPOSs.clear();
				lastFewTags.clear();
				continue;
			}
			
			// keep few words before this one
			lastFewWords.add(data[0]);
			lastFewPOSs.add(data[1]);
			lastFewTags.add(data[2]);
			int memorySize = lastFewPOSs.size();

			// clear features to make new ones
			features.clear();

			// add features
			features.put("current_word", data[0]);
			features.put("current_POS", data[1]);

//			if (Settings.usePrevWord) {
				features.put("previous_word",
						memorySize >= 2 ? lastFewWords.get(memorySize - 2)
								: Settings.missing_word);
				features.put("previous_POS",
						memorySize >= 2 ? lastFewPOSs.get(memorySize - 2)
								: Settings.missing_PoS);
	//		}

		//	if (Settings.use2PrevWord) {
				features.put("2previous_word",
						memorySize >= 3 ? lastFewWords.get(memorySize - 3)
								: Settings.missing_word);
				features.put("2previous_POS",
						memorySize >= 3 ? lastFewPOSs.get(memorySize - 3)
								: Settings.missing_PoS);
			//}

			//if (Settings.useCapitalization) {
				features.put("starts_with_capital", String.valueOf(Character
						.isUpperCase(data[0].charAt(0))));
			//}
			//if(Settings.usePreviousTag){
			//	features.put("previous_tag", memorySize>=2? lastFewTags.get(memorySize-2):Settings.missing_tag);
			//}
			StringBuilder sbr = new StringBuilder();
			for (String key : features.keySet()) {
				sbr.append(features.get(key) + " ");
			}
			out.format("%s %s\n", sbr.toString(), data[2]);
			out.flush();
		}
		out.close();
	}

}
