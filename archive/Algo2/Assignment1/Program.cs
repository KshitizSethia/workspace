using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Assignment1
{
    class Program
    {
        static void Main(string[] args)
        {
            //Answer for Q1: 69119377652
            Question1And2();
        }

        private static void Question1And2()
        {
            string[] fileLines = (string[])File.ReadAllLines(@"C:\Dropbox\workspace\Algo2\Assignment1\jobs.txt");

            int numJobs = int.Parse(fileLines[0]);

            List<Job> jobs = new List<Job>();
            for (int index = 1; index <= numJobs; index++)
            {
                jobs.Add(new Job(fileLines[index]));
            }

            // Sample test data: answer for Q1: 267301, answer for Q2: 260696
            //List<Job> jobs = new List<Job>();
            //jobs.Add(new Job("8 50"));
            //jobs.Add(new Job("74 59"));
            //jobs.Add(new Job("31 73"));
            //jobs.Add(new Job("45 79"));
            //jobs.Add(new Job("24 10"));
            //jobs.Add(new Job("41 66"));
            //jobs.Add(new Job("93 43"));
            //jobs.Add(new Job("88 4"));
            //jobs.Add(new Job("28 30"));
            //jobs.Add(new Job("41 13"));
            //jobs.Add(new Job("4 70"));
            //jobs.Add(new Job("10 58"));
            //jobs.Add(new Job("61 34"));
            //jobs.Add(new Job("100 79"));
            //jobs.Add(new Job("17 36"));
            //jobs.Add(new Job("98 27"));
            //jobs.Add(new Job("13 68"));
            //jobs.Add(new Job("11 34"));
            //jobs.Add(new Job("80 50"));
            //jobs.Add(new Job("80 22"));
            //jobs.Add(new Job("68 73"));
            
            
            jobs.Sort();
            
            long timePassed = 0;
            long weightedCompletionTime = 0;

            foreach (Job job in jobs)
            {
                timePassed += job.Length;
                weightedCompletionTime += job.Weight * timePassed;
            }

            return;
        }
    }
}
