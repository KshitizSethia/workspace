using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading;

namespace Lawnmower
{
    class Program
    {
        internal const int _maxWaitPoolSize = 50;
        internal const int _problemSetSize = 25;

        static Dictionary<int, string> results = new Dictionary<int, string>();
        static int numTestCases;

        static void Main(string[] args)
        {
            StreamReader inputStream = new StreamReader(args[0]);

            numTestCases = int.Parse(inputStream.ReadLine());
            int numProblemSets = (numTestCases / _problemSetSize) + 1;
            List<ManualResetEvent> events = new List<ManualResetEvent>(numProblemSets);

            for (int problemSetIterator = 0; problemSetIterator < numProblemSets; problemSetIterator++)
            {
                int numberOfElementsInCurrentSet = problemSetIterator == numProblemSets - 1
                                                    ? (numTestCases % _problemSetSize) == 0
                                                        ? _problemSetSize
                                                        : 0//numTestCases % _problemSetSize
                                                    : _problemSetSize;
                double[,] problemSet = new double[2, numberOfElementsInCurrentSet];

                for (int iterator = 0; iterator < numberOfElementsInCurrentSet; iterator++)
                {
                    string[] startAndPaint = inputStream.ReadLine().Split(' ');

                    problemSet[0, iterator] = double.Parse(startAndPaint[0]);
                    problemSet[1, iterator] = double.Parse(startAndPaint[1]);
                }

                events.Add(new ManualResetEvent(false));
                ProblemSet pSet = new ProblemSet(problemSetIterator + 1, problemSet, events[problemSetIterator]);

                ThreadPool.QueueUserWorkItem(new WaitCallback(CalculateAndStoreResult), pSet);
            }

            inputStream.Close();

            int numEventSets = (int)Math.Ceiling(((double)numTestCases / _maxWaitPoolSize));
            ManualResetEvent[] eventSetEvents = new ManualResetEvent[numEventSets];

            for (int currentEventSetIndex = 0; currentEventSetIndex < numEventSets; currentEventSetIndex++)
            {
                eventSetEvents[currentEventSetIndex] = new ManualResetEvent(false);

                int numberOfElementsInCurrentSet = currentEventSetIndex == numEventSets - 1
                                                    ? (numProblemSets % _maxWaitPoolSize) == 0
                                                        ? _maxWaitPoolSize
                                                        : 0//numProblemSets % _maxWaitPoolSize
                                                    : _maxWaitPoolSize;

                EventCascader eventCascader = new EventCascader(eventSetEvents[currentEventSetIndex], events.GetRange(currentEventSetIndex * _maxWaitPoolSize, numberOfElementsInCurrentSet).ToArray());
                ThreadPool.QueueUserWorkItem(new WaitCallback(WaitForEventSet), eventCascader);
            }

            WaitHandle.WaitAll(eventSetEvents);

            StreamWriter streamWriter = new StreamWriter(File.Create(args[1]));
            for (int setIterator = 1; setIterator < results.Count + 1; setIterator++)
            {
                streamWriter.Write(results[setIterator]);
            }
            streamWriter.Flush();
            streamWriter.Close();
        }

        private static void CalculateAndStoreResult(object state)
        {
            ProblemSet pSet = state as ProblemSet;
            StringBuilder sb = new StringBuilder();
            for(int iterator=0; iterator<pSet.problemSet.GetLength(1); iterator++)
            {
                double twoRMinusOne = (2*pSet.problemSet[0, iterator])-1;
                pSet.solution[iterator] = Math.Floor((Math.Sqrt(Math.Pow(twoRMinusOne, 2) + (8 * pSet.problemSet[1, iterator])) - twoRMinusOne) / 4);

                sb.AppendLine("Case #" + (_problemSetSize*(pSet.id-1)+iterator+1) +": " +pSet.solution[iterator]);
            }


            lock (results)
            {
                results.Add(pSet.id, sb.ToString());
            }
            pSet.manualResetEvent.Set();
        }

        public static void WaitForEventSet(object obj)
        {
            EventCascader info = (EventCascader)obj;

            WaitHandle.WaitAll(info.eventsToWaitFor);
            info.eventToNotify.Set();
        }

        //public static void TryMowingLawn(object obj)
        //{
        //    Lawn lawn = obj as Lawn;

        //    //parse lawn into array
        //    for (int currentRow = 0; currentRow < lawn.lawnRows; currentRow++)
        //    {
        //        string[] columnEntries = lawn.unparsedLawn[currentRow].Split(' ');

        //        for (int currentColumn = 0; currentColumn < lawn.lawnColumns; currentColumn++)
        //        {
        //            lawn.parsedLawn[currentRow, currentColumn] = int.Parse(columnEntries[currentColumn]);
        //        }
        //    }

        //    //try mowing the lawn
        //    lawn.isMowable = true;
        //    for (int currentRow = 0; currentRow < lawn.lawnRows; currentRow++)
        //    {
        //        for (int currentColumn = 0; currentColumn < lawn.lawnColumns; currentColumn++)
        //        {
        //            if (!(
        //                   Mower.CanMowRowWithoutDestroyingPattern(currentRow, lawn.parsedLawn[currentRow, currentColumn], lawn)
        //                 || Mower.CanMowColumnWithoutDestroyingPattern(currentColumn, lawn.parsedLawn[currentRow, currentColumn], lawn)
        //               ))
        //            {
        //                lawn.isMowable = false; break;
        //            }
        //        }
        //    }

        //    //store the result
        //    lock (results)
        //    {
        //        results.Add(lawn.id, lawn.isMowable);
        //    }

        //    //notify that I am done
        //    lawn.manualResetEvent.Set();
        //}

        //public static void CalculateResult(Object obj)
        //{
        //    Game game = (Game)obj;

        //    bool isGameUnfinished = false;

        //    char[,] parsedState = new char[Constants._boardSize, Constants._boardSize];

        //    for (int index = 0; index < Constants._boardSize; index++)
        //    {
        //        char[] chars = game.unparsedState[index].ToCharArray();

        //        if (!isGameUnfinished)
        //            isGameUnfinished = chars.Contains('.');

        //        parsedState[index, 0] = chars[0];
        //        parsedState[index, 1] = chars[1];
        //        parsedState[index, 2] = chars[2];
        //        parsedState[index, 3] = chars[3];
        //    }

        //    char winningSide = Game.CalculateColumnWinner(parsedState);
        //    if (winningSide == '.')
        //        winningSide = Game.CalculateRowWinner(parsedState);
        //    if (winningSide == '.')
        //        winningSide = Game.CalculateDiagonalWinner(parsedState);

        //    switch (winningSide)
        //    {
        //        case 'X': game.state = "X won"; break;
        //        case 'O': game.state = "O won"; break;
        //        case '.':
        //            {
        //                if (isGameUnfinished)
        //                    game.state = "Game has not completed";
        //                else
        //                    game.state = "Draw";
        //            }; break;
        //    }

        //    lock (results)
        //    {
        //        results.Add(game.id, game.state);
        //    }
        //    game.resetEvent.Set();
        //}
    }
}
