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

        static Dictionary<int, bool> results = new Dictionary<int, bool>();
        static int numTestCases;

        static void Main(string[] args)
        {
            StreamReader inputStream = new StreamReader(args[0]);

            numTestCases = int.Parse(inputStream.ReadLine());

            List<ManualResetEvent> events = new List<ManualResetEvent>(numTestCases);

            for (int iterator = 0; iterator < numTestCases; iterator++)
            {
                string[] lawnSize = inputStream.ReadLine().Split(' ');

                int lawnRows = int.Parse(lawnSize[0]);
                int lawnColumns = int.Parse(lawnSize[1]);

                string[] unparsedLawn = new string[lawnRows];

                for (int i = 0; i < lawnRows; i++)
                {
                    unparsedLawn[i] = inputStream.ReadLine();
                }

                events.Add(new ManualResetEvent(false));
                Lawn lawn = new Lawn(iterator+1, lawnRows, lawnColumns, unparsedLawn, events[iterator]);

                ThreadPool.QueueUserWorkItem(new WaitCallback(TryMowingLawn), lawn);
            }

            inputStream.Close();

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

            StreamWriter streamWriter = new StreamWriter(File.Create(args[1]));
            for (int iterator = 1; iterator < results.Count + 1; iterator++)
            {
                string message = "Case #" + iterator + ": " + (results[iterator]==true?"YES":"NO");
                streamWriter.WriteLine(message);
            }
            streamWriter.Flush();
            streamWriter.Close();
        }

        public static void WaitForEventSet(object obj)
        {
            EventCascader info = (EventCascader)obj;

            WaitHandle.WaitAll(info.eventsToWaitFor);
            info.eventToNotify.Set();
        }
        
        public static void TryMowingLawn(object obj)
        {
            Lawn lawn = obj as Lawn;

            //parse lawn into array
            for (int currentRow = 0; currentRow < lawn.lawnRows; currentRow++)
            {
                string[] columnEntries = lawn.unparsedLawn[currentRow].Split(' ');

                for (int currentColumn = 0; currentColumn < lawn.lawnColumns; currentColumn++)
                {
                    lawn.parsedLawn[currentRow, currentColumn] = int.Parse(columnEntries[currentColumn]);
                }
            }

            //try mowing the lawn
            lawn.isMowable = true;
            for (int currentRow = 0; currentRow < lawn.lawnRows; currentRow++)
            {
                for (int currentColumn = 0; currentColumn < lawn.lawnColumns; currentColumn++)
                {
                    if (!(
                           Mower.CanMowRowWithoutDestroyingPattern(currentRow, lawn.parsedLawn[currentRow, currentColumn], lawn)
                         ||Mower.CanMowColumnWithoutDestroyingPattern(currentColumn, lawn.parsedLawn[currentRow, currentColumn], lawn)
                       ))
                    {
                        lawn.isMowable = false; break;
                    }
                }
            }

            //store the result
            lock (results)
            {
                results.Add(lawn.id, lawn.isMowable);
            }

            //notify that I am done
            lawn.manualResetEvent.Set();
        }

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
