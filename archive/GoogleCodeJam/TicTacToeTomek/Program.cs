using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading;

namespace TicTacToeTomek
{
    class Constants
    {
        internal const int _numThreads = 10;
        internal const int _boardSize = 4;
        internal const int _maxWaitPoolSize = 50;
    }

    class EventCascader
    {
        public ManualResetEvent eventToNotify;
        public ManualResetEvent[] eventsToWaitFor;

        public EventCascader(ManualResetEvent eventToNotify, ManualResetEvent[] eventsToWaitFor)
        {
            this.eventToNotify = eventToNotify;
            this.eventsToWaitFor = eventsToWaitFor;
        }
    }

    class Program
    {
        static Dictionary<int, string> results = new Dictionary<int, string>();
        static int numTestCases;

        static void Main(string[] args)
        {
            StreamReader inputStream = new StreamReader(args[0]);

            numTestCases = int.Parse(inputStream.ReadLine());

            List<ManualResetEvent> events = new List<ManualResetEvent>(numTestCases);

            for (int iterator = 0; iterator < numTestCases; iterator++)
            {
                string[] unparsedState = new string[Constants._boardSize];

                for (int i = 0; i < Constants._boardSize; i++)
                {
                    unparsedState[i] = inputStream.ReadLine();
                }

                inputStream.ReadLine();

                events.Add(new ManualResetEvent(false));
                Game game = new Game(iterator + 1, unparsedState, events[iterator]);

                ThreadPool.QueueUserWorkItem(new WaitCallback(CalculateResult), game);
            }

            inputStream.Close();

            int numEventSets = (int) Math.Ceiling(((double)numTestCases/Constants._maxWaitPoolSize));
            ManualResetEvent[] eventSetEvents = new ManualResetEvent[numEventSets];

            for (int currentEventSetIndex = 0; currentEventSetIndex < numEventSets; currentEventSetIndex++)
            {
                eventSetEvents[currentEventSetIndex] = new ManualResetEvent(false);

                int numberOfElementsInCurrentSet = currentEventSetIndex == numEventSets - 1 
                                                    ? (numTestCases % Constants._maxWaitPoolSize)==0
                                                        ? Constants._maxWaitPoolSize
                                                        : numTestCases % Constants._maxWaitPoolSize
                                                    : Constants._maxWaitPoolSize;

                EventCascader eventCascader = new EventCascader(eventSetEvents[currentEventSetIndex], events.GetRange(currentEventSetIndex * Constants._maxWaitPoolSize, numberOfElementsInCurrentSet).ToArray());
                ThreadPool.QueueUserWorkItem(new WaitCallback(WaitForEventSet), eventCascader);
            }

            WaitHandle.WaitAll(eventSetEvents);

            StreamWriter streamWriter = new StreamWriter(File.Create(args[1]));
            for (int iterator = 1; iterator < results.Count+1; iterator++)
            {
                string message = "Case #" + iterator + ": " + results[iterator];
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

        public static void CalculateResult(Object obj)
        {
            Game game = (Game)obj;

            bool isGameUnfinished = false;

            char[,] parsedState = new char[Constants._boardSize, Constants._boardSize];

            for (int index = 0; index < Constants._boardSize; index++)
            {
                char[] chars = game.unparsedState[index].ToCharArray();

                if (!isGameUnfinished)
                    isGameUnfinished = chars.Contains('.');

                parsedState[index, 0] = chars[0];
                parsedState[index, 1] = chars[1];
                parsedState[index, 2] = chars[2];
                parsedState[index, 3] = chars[3];
            }

            char winningSide = Game.CalculateColumnWinner(parsedState);
            if (winningSide == '.')
                winningSide = Game.CalculateRowWinner(parsedState);
            if (winningSide == '.')
                winningSide = Game.CalculateDiagonalWinner(parsedState);

            switch (winningSide)
            {
                case 'X': game.state = "X won"; break;
                case 'O': game.state = "O won"; break;
                case '.':
                    {
                        if (isGameUnfinished)
                            game.state = "Game has not completed";
                        else
                            game.state = "Draw";
                    }; break;
            }

            lock (results)
            {
                results.Add(game.id, game.state);
            }
            game.resetEvent.Set();
        }
    }

    class Game
    {
        internal int id;
        internal string state;
        internal string[] unparsedState;
        internal ManualResetEvent resetEvent;

        public Game(int id, string[] unparsedState, ManualResetEvent resetEvent)
        {
            this.id = id;
            this.unparsedState = unparsedState;
            this.resetEvent = resetEvent;
        }

        internal static char CalculateDiagonalWinner(char[,] parsedState)
        {
            char result = '.';

            char matcher = parsedState[0, 0];

            if (matcher == 'T')
                matcher = parsedState[1, 1];

            if (
                  (matcher == parsedState[1, 1] || parsedState[1, 1] == 'T')
               && (matcher == parsedState[2, 2] || parsedState[2, 2] == 'T')
               && (matcher == parsedState[3, 3] || parsedState[3, 3] == 'T')
               )
                result = matcher;

            if (result != '.')
                return result;

            matcher = parsedState[0, 3];

            if (matcher == 'T')
                matcher = parsedState[1, 2];

            if (
                  (matcher == parsedState[1, 2] || parsedState[1, 2] == 'T')
               && (matcher == parsedState[2, 1] || parsedState[2, 1] == 'T')
               && (matcher == parsedState[3, 0] || parsedState[3, 0] == 'T')
               )
                result = matcher;

            return result;
        }

        internal static char CalculateRowWinner(char[,] parsedState)
        {
            char result = '.';

            for (int rowNum = 0; rowNum < Constants._boardSize; rowNum++)
            {
                char matcher = parsedState[rowNum, 0];

                if (matcher == 'T')
                    matcher = parsedState[rowNum, 1];

                if (
                     (matcher == parsedState[rowNum, 1] || parsedState[rowNum, 1] == 'T')
                  && (matcher == parsedState[rowNum, 2] || parsedState[rowNum, 2] == 'T')
                  && (matcher == parsedState[rowNum, 3] || parsedState[rowNum, 3] == 'T')
                  )
                {
                    result = matcher;
                    if (result != '.') return result;
                }
            }

            return result;
        }

        internal static char CalculateColumnWinner(char[,] parsedState)
        {
            char result = '.';

            for (int columnNum = 0; columnNum < Constants._boardSize; columnNum++)
            {
                char matcher = parsedState[0, columnNum];

                if (matcher == 'T')
                    matcher = parsedState[1, columnNum];

                if (
                    (matcher == parsedState[1, columnNum] || parsedState[1, columnNum] == 'T')
                  && (matcher == parsedState[2, columnNum] || parsedState[2, columnNum] == 'T')
                  && (matcher == parsedState[3, columnNum] || parsedState[3, columnNum] == 'T')
                  )
                {
                    result = matcher;
                    if (result != '.') return result;
                }
            }
            return result;
        }
    }
}
