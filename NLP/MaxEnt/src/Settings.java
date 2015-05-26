public class Settings {
	public final static String training_data_raw = "training.txt";
	public final static String training_data_features = "training_features.dat";

	public final static String modelFile = "model.dat";

	public final static String test_data_raw = "test.txt";
	public final static String test_data_features = "test_features.dat";

	public final static String missing_word = "__NONE_Word__";
	public final static String missing_PoS = "__NONE_PoS__";
	//public final static String missing_tag = "X";
	
	public final static String training_data_features_mallet = "mallet/training_features_mallet.dat";
	public final static String test_data_features_mallet = "mallet/test_features_mallet.dat";
	public final static String mallet_results = "mallet/mallet_test_output.txt";
	
	public static boolean usePrevWord = false;
	public static boolean use2PrevWord = false;
	public static boolean useCapitalization = false;
	//public static boolean usePreviousTag = false;
	
}
