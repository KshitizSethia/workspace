using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Assignment1
{
    class Job : IComparable<Job>
    {
        public Job(string weightLength)
        {
            string[] str = weightLength.Split(' ');
            Weight = int.Parse(str[0]);
            Length = int.Parse(str[1]);
        }


        public int Weight { get; private set; }
        public int Length { get; private set; }

        ///// <summary>
        ///// Compare function for Question 1
        ///// 
        ///// In this programming problem and the next you'll code up the greedy algorithms from lecture for minimizing the weighted sum of completion times.. Download the text file here. This file describes a set of jobs with positive and integral weights and lengths. It has the format
        ///// [number_of_jobs]
        ///// [job_1_weight] [job_1_length]
        ///// [job_2_weight] [job_2_length]
        ///// ...
        ///// For example, the third line of the file is "74 59", indicating that the second job has 
        ///// weight 74 and length 59. You should NOT assume that edge weights or lengths are distinct.
        ///// 
        ///// Your task in this problem is to run the greedy algorithm that schedules jobs in decreasing order
        ///// of the difference (weight - length). Recall from lecture that this algorithm is not always optimal.
        ///// IMPORTANT: if two jobs have equal difference (weight - length), you should schedule the job with 
        ///// higher weight first. Beware: if you break ties in a different way, you are likely to get the wrong answer.
        ///// You should report the sum of weighted completion times of the resulting schedule --- a positive integer --- 
        ///// in the box below.
        ///// 
        ///// ADVICE: If you get the wrong answer, try out some small test cases to debug your algorithm 
        ///// (and post your test cases to the discussion forum)!
        ///// </summary>
        ///// <param name="other"></param>
        ///// <returns></returns>
        //public int CompareTo(Job other)
        //{
        //    int diffLocal = this.Weight - this.Length;

        //    int diffOther = other.Weight - other.Length;

        //    if (diffLocal != diffOther)
        //        return diffOther - diffLocal;

        //    return other.Weight - this.Weight;
        //}

        /// <summary>
        /// Compare function for Question 2
        /// 
        /// For this problem, use the same data set as in the previous problem. 
        /// Your task now is to run the greedy algorithm that schedules jobs 
        /// (optimally) in decreasing order of the ratio (weight/length). 
        /// In this algorithm, it does not matter how you break ties. 
        /// You should report the sum of weighted completion times of the 
        /// resulting schedule --- a positive integer --- in the box below.
        /// </summary>
        /// <param name="other"></param>
        /// <returns></returns>
        public int CompareTo(Job other)
        {
            int ratioLocal = this.Weight / this.Length;

            int ratioOther = other.Weight / other.Length;

            if (ratioLocal != ratioOther)
                return ratioOther - ratioLocal;

            return other.Weight - this.Weight;
        }

        public override string ToString()
        {
            return "Weight: " +Weight +", Length: " +Length +", Ratio: " +(Weight/Length);
        }
    }
}
