public class Summary {
	public String descriptor;

	public float accuracy;

	public float true_positives;
	public float predicted_positives;
	public float actual_positives;

	public float recall;
	public float precision;
	public float f1Score;

	@Override
	public String toString() {
		StringBuilder sbr = new StringBuilder();
		sbr.append("Recall: ");
		sbr.append(recall);
		sbr.append("\nPrecision: ");
		sbr.append(precision);
		sbr.append("\nF1Score: ");
		sbr.append(f1Score);
		sbr.append("\n");
		return sbr.toString();
	}
}
