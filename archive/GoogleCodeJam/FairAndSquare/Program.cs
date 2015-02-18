using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.IO;

namespace FairAndSquare
{
    class Program
    {
        internal const int _maxWaitPoolSize = 20;

        static Dictionary<int, double> results = new Dictionary<int, double>();
        static int numTestCases;

        static Dictionary<int, double> numberOfFairAndSquaresForNDigits = new Dictionary<int, double>();

        static void Main(string[] args)
        {
            StreamReader inputStream = new StreamReader(args[0]);

            numTestCases = int.Parse(inputStream.ReadLine());

            List<ManualResetEvent> events = new List<ManualResetEvent>(numTestCases);
            //make subProblems and queue in ThreadPool
            for (int iterator = 0; iterator < numTestCases; iterator++)
            {
                string[] problemSize = inputStream.ReadLine().Split(' ');

                double minNum = double.Parse(problemSize[0]);
                double maxNum = double.Parse(problemSize[1]);

                events.Add(new ManualResetEvent(false));
                FairAndSquareProblem problem = new FairAndSquareProblem(iterator + 1, minNum, maxNum, events[iterator]);
                
                ThreadPool.QueueUserWorkItem(new WaitCallback(CalculateFairAndSquareNumberCount), problem);
            }

            inputStream.Close();

            //Wait for all queued sub problems to finish
            int numEventSets = (int)Math.Ceiling(((double)numTestCases / _maxWaitPoolSize));
            ManualResetEvent[] eventSetEvents = new ManualResetEvent[numEventSets];

            for (int currentEventSetIndex = 0; currentEventSetIndex < numEventSets; currentEventSetIndex++)
            {
                eventSetEvents[currentEventSetIndex] = new ManualResetEvent(false);

                int numberOfElementsInCurrentSet = currentEventSetIndex == numEventSets - 1
                                                    ? (numTestCases % _maxWaitPoolSize) == 0
                                                        ? _maxWaitPoolSize
                                                        : numTestCases % _maxWaitPoolSize
                                                    : _maxWaitPoolSize;

                EventCascader eventCascader = new EventCascader(eventSetEvents[currentEventSetIndex], events.GetRange(currentEventSetIndex * _maxWaitPoolSize, numberOfElementsInCurrentSet).ToArray());
                ThreadPool.QueueUserWorkItem(new WaitCallback(WaitForEventSet), eventCascader);
            }

            WaitHandle.WaitAll(eventSetEvents);

            //write final output to file
            StreamWriter streamWriter = new StreamWriter(File.Create(args[1]));
            for (int iterator = 1; iterator < results.Count + 1; iterator++)
            {
                string message = "Case #" + iterator + ": " + results[iterator];
                streamWriter.WriteLine(message);
            }
            streamWriter.Flush();
            streamWriter.Close();
        }

        public static void WaitForEventSet(object obj)
        {
            EventCascader cascader = (EventCascader)obj;

            WaitHandle.WaitAll(cascader.eventsToWaitFor);
            cascader.eventToNotify.Set();
        }

        public static void CalculateFairAndSquareNumberCount(object obj)
        {
            FairAndSquareProblem problem = obj as FairAndSquareProblem;

            double minSqrtNum = Math.Ceiling(Math.Sqrt(problem.minNum));
            double maxSqrtNum = Math.Floor(Math.Sqrt(problem.maxNum));

            int minNumberOfDigits = GetNumberOfDigits(minSqrtNum);
            int maxNumberOfDigits = GetNumberOfDigits(maxSqrtNum);

            double numberOfFairAndSquareNumbers = 0;

            if (maxNumberOfDigits - minNumberOfDigits > 0)
            {
                numberOfFairAndSquareNumbers += GetCountBetweenForNDigits(minSqrtNum, Math.Pow(10, minNumberOfDigits) - 1, minNumberOfDigits);
                if(maxNumberOfDigits-minNumberOfDigits>1)
                    numberOfFairAndSquareNumbers += GetCountForFullSeries(minNumberOfDigits + 1, maxNumberOfDigits-1);
                numberOfFairAndSquareNumbers += GetCountBetweenForNDigits(Math.Pow(10, maxNumberOfDigits - 1), maxSqrtNum, maxNumberOfDigits);
            }
            else
            {
                numberOfFairAndSquareNumbers += GetCountBetweenForNDigits(minSqrtNum, maxSqrtNum, minNumberOfDigits);
            }

            lock (results)
            {
                results.Add(problem.id, numberOfFairAndSquareNumbers);
            }

            problem.manualResetEvent.Set();
        }

        private static double GetCountBetweenForNDigits(double startNum, double endNum, int numberOfDigits)
        {
            double result = 0;

            //double halfNumber = GetDominantHalf(startNum, numberOfDigits);

            //startNum = GetFullNumber(halfNumber, numberOfDigits);

            for (double number = startNum; number <= endNum; number++)
            {
                if (IsPalindrome(number) && IsPalindrome(Math.Pow(number, 2)))
                    result++;
            }

            return result;
        }

        private static bool IsPalindrome(double number)
        {
            string numberString = number.ToString();

            if (numberString.Contains('.'))
                numberString = numberString.Substring(0, numberString.IndexOf('.'));

            for (int i = 0; i < numberString.Length / 2; i++)
            {
                if (numberString[i] != numberString[numberString.Length - 1 - i]) return false;
            }
            return true;
        }

        private static double GetFullNumber(double halfNumber, int numberOfDigits)
        {
            if (numberOfDigits == 1)
                return halfNumber;

            double result = halfNumber * Math.Pow(10, (int)(numberOfDigits / 2));

            string secondHalf = halfNumber.ToString();
            if (((double)numberOfDigits / 2) != (numberOfDigits / 2))
                secondHalf = secondHalf.Substring(0, secondHalf.Length - 1);

            secondHalf = ReverseString(secondHalf);

            result += double.Parse(secondHalf);

            return result;
        }

        private static double GetDominantHalf(double number, int numberOfDigits)
        {
            string numberString = number.ToString().Substring(0, numberOfDigits);

            double firstHalf = double.Parse(numberString.Substring(0, (int)Math.Ceiling((double)numberOfDigits / 2)));

            double reversedSecondHalf = double.Parse(ReverseString(numberString.Substring(numberOfDigits / 2, (int)Math.Ceiling((double)numberOfDigits / 2))));

            return firstHalf > reversedSecondHalf ? firstHalf : reversedSecondHalf;
        }

        private static string ReverseString(string input)
        {
            char[] arr = input.ToCharArray();
            Array.Reverse(arr);
            return new string(arr);
        }

        private static double GetCountForFullSeries(int startNumDigits, int endNumDigits)
        {
            double result = 0;
            for (int numDigits = startNumDigits; numDigits <= endNumDigits; numDigits++)
            {
                if (!numberOfFairAndSquaresForNDigits.ContainsKey(numDigits))
                {
                    lock (numberOfFairAndSquaresForNDigits)
                    {
                        numberOfFairAndSquaresForNDigits[numDigits] = GetCountBetweenForNDigits(Math.Pow(10, numDigits - 1), Math.Pow(10, numDigits) - 1, numDigits);
                    }
                }
                result += numberOfFairAndSquaresForNDigits[numDigits];
            }

            return result;
        }

        private static int GetNumberOfDigits(double minSqrtNum)
        {
            double logVal = Math.Log10(minSqrtNum);

            return (int)logVal + 1;
        }
    }
}
