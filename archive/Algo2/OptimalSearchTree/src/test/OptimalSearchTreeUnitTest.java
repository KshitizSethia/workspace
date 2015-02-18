package test;

import static org.junit.Assert.*;

import org.junit.Test;
import org.junit.experimental.theories.suppliers.TestedOn;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

import java.util.HashMap;

@RunWith(JUnit4.class)
public class OptimalSearchTreeUnitTest
{

    @Test
    public void ProblemSet3_Question5()
    {
	HashMap<Integer, Double> keyProbabilityPairs = new HashMap<Integer, Double>();
	
	keyProbabilityPairs.put(1, 0.05);
	keyProbabilityPairs.put(2,0.4);
	keyProbabilityPairs.put(3,0.08);
	keyProbabilityPairs.put(4,0.04);
	keyProbabilityPairs.put(5,0.1);
	keyProbabilityPairs.put(6,0.1);
	keyProbabilityPairs.put(7,0.23);
	
	code.SearchTree sTree = new code.SearchTree(keyProbabilityPairs);
	
	double result = sTree.CalculateOptimizedAverageSearchTime();
	
	assertTrue(result == 2.18);
    }

}
