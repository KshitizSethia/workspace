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
        internal const int _maxWaitPoolSize = 50;

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

            double numberOfFairAndSquareNumbers = 0;

            for (double iterator = problem.minNum; iterator <= problem.maxNum; iterator++)
            {
                if (IsPalindrome(iterator) && IsPerfectSquare(iterator) && IsPalindrome(Math.Sqrt(iterator)))
                    numberOfFairAndSquareNumbers++;
            }

            lock (results)
            {
                results.Add(problem.id, numberOfFairAndSquareNumbers);
            }

            problem.manualResetEvent.Set();
        }

        private static bool IsPerfectSquare(double iterator)
        {
            return (Math.Sqrt(iterator) % 1) == 0;
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
    }
}
