using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace Names
{
    class Program
    {
        static void Main(string[] args)
        {
            StreamReader inputFile = new StreamReader(args[0]);

            StreamWriter writer = new StreamWriter(args[1]);
            int numTestCases = int.Parse(inputFile.ReadLine());

            for (int testCase = 1; testCase <= numTestCases; testCase++)
            {
                writer.WriteLine("Case #" +testCase +": " +GetNValue(inputFile.ReadLine()));
            }

            writer.Flush();
            writer.Close();
        }

        private static Int32 GetNValue(string input)
        {
            string[] ip = input.Split(' ');

            char[] name = ip[0].ToCharArray();
            int n = int.Parse(ip[1]);

            Int32 nValue = 0;
            Int32[] previousIndexSet = new Int32[]{-1,name.Length-1};

            foreach (Int32[] indexSet in GetIndexSet(name, n))
            {
                for (Int32 a = previousIndexSet[0] + 1; a <= indexSet[0]; a++)
                {
                    nValue += name.Length - indexSet[1];
                    //nValue += (name.Length - 1)
                }
                previousIndexSet = indexSet;
            }

            return nValue;
        }

        private static Int32[][] GetIndexSet(char[] name, int n)
        {
            List<Int32[]> result = new List<Int32[]>();

            for (Int32 endIndex = n - 1; endIndex < name.Length; endIndex++)
            {
                bool targetStringReached = true;
                for(Int32 index = endIndex-n+1; index<endIndex+1;index++)
                    if("aeiou".Contains(name[index]))
                    { targetStringReached = false; break; }
                if (targetStringReached)
                    result.Add(new Int32[] { endIndex - n + 1, endIndex });
            }

            return result.ToArray();
        }
    }
}
