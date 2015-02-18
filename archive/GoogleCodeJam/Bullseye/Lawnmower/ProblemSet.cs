using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Lawnmower
{
    class ProblemSet
    {
        internal int id;
        internal double[,] problemSet;
        internal double[] solution;
        internal System.Threading.ManualResetEvent manualResetEvent;

        public ProblemSet(int p, double[,] problemSet, System.Threading.ManualResetEvent manualResetEvent)
        {
            this.id = p;
            this.problemSet = problemSet;
            this.manualResetEvent = manualResetEvent;
            this.solution = new double[problemSet.GetLength(1)];
        }
    }
}
