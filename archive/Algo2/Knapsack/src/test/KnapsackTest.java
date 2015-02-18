package test;

import static org.junit.Assert.*;

import java.io.IOException;

import org.junit.Test;

import code.Knapsack;

public class KnapsackTest {

	@Test
	public void test1() throws IOException {
		Knapsack knapsack = new Knapsack("C:\\Dropbox\\workspace\\Algo2\\Knapsack\\test1.txt");
		
		long result = knapsack.calculateMaxValueOfKnapsack();
		
		assertTrue(result == 9);
	}

	@Test
	public void test2() throws IOException {
		Knapsack knapsack = new Knapsack("C:\\Dropbox\\workspace\\Algo2\\Knapsack\\test2.txt");
		
		long result = knapsack.calculateMaxValueOfKnapsack();
		
		assertTrue(result == 10);
	}
	
	@Test
	public void test3() throws IOException {
		Knapsack knapsack = new Knapsack("C:\\Dropbox\\workspace\\Algo2\\Knapsack\\test3.txt");
		
		long result = knapsack.calculateMaxValueOfKnapsack();
		
		assertTrue(result == 90);
	}

	@Test
	public void test4() throws IOException {
		Knapsack knapsack = new Knapsack("C:\\Dropbox\\workspace\\Algo2\\Knapsack\\test4.txt");
		
		long result = knapsack.calculateMaxValueOfKnapsack();
		
		assertTrue(result == 40);
	}
	
	@Test
	public void Assignment3_Question1() throws IOException {
		Knapsack knapsack = new Knapsack("C:\\Dropbox\\workspace\\Algo2\\Knapsack\\knapsack1.txt");
		
		long result = knapsack.calculateMaxValueOfKnapsack();
		
		assertTrue(result == 2493893);
	}
	
	@Test
	public void test5() throws IOException {
		Knapsack knapsack = new Knapsack("C:\\Dropbox\\workspace\\Algo2\\Knapsack\\test5.txt");
		
		long result = knapsack.calculateMaxValueOfKnapsack();
		
		assertTrue(result == 13549094);
	}
	
	@Test
	public void Assignment3_Question2() throws IOException {
		Knapsack knapsack = new Knapsack("C:\\Dropbox\\workspace\\Algo2\\Knapsack\\knapsack2.txt");
		
		long result = knapsack.calculateMaxValueOfKnapsack();
		
		assertTrue(result == 2595819);
	}

}
