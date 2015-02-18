package code;

import java.util.Map;
import java.util.Arrays;
import java.util.Collections;

public class SearchTree
{
    Double cache[][];
    Map<Integer, Double> keyAndProbabilitySet;
    int numKeys;

    public SearchTree(Map<Integer, Double> keyAndProbabilitySet)
    {
	this.keyAndProbabilitySet = keyAndProbabilitySet;

	this.numKeys = this.keyAndProbabilitySet.size();

	cache = new Double[numKeys][numKeys];
    }

    public Double CalculateOptimizedAverageSearchTime()
    {
	for (int subsetSpan = 0; subsetSpan < numKeys; subsetSpan++)
	{
	    for (int startIndex = 0; startIndex < numKeys-subsetSpan; startIndex++)
	    {
		int endIndex = startIndex + subsetSpan;

		Double values[] = new Double[subsetSpan + 1];

		for (int chosenRootsIndex = startIndex; chosenRootsIndex <= endIndex; chosenRootsIndex++)
		{
		    values[chosenRootsIndex - startIndex] = Cost(startIndex,
			    chosenRootsIndex - 1)
			    + Cost(chosenRootsIndex + 1, endIndex);
		}

		cache[startIndex][endIndex] = SumProbabilities(startIndex,endIndex)
						+ Collections.min(Arrays.asList(values));
	    }
	    int a=0;
	}

	return cache[0][numKeys-1];
    }

    private Double SumProbabilities(int startIndex, int endIndex)
    {
	Double result = new Double(0);

	for (int index = startIndex; index <= endIndex; index++)
	{
	    result += keyAndProbabilitySet.get(new Integer(index+1));
	}

	return result;
    }

    private Double Cost(int row, int column)
    {
	if (row > column)
	    return new Double(0);
	return cache[row][column];
    }
}
