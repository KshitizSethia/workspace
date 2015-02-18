using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace FairAndSquare
{
    class FairAndSquareProblem
    {
        internal int id;
        internal double minNum;
        internal double maxNum;
        internal ManualResetEvent manualResetEvent;

        public FairAndSquareProblem(int id, double minNum, double maxNum, ManualResetEvent manualResetEvent)
        {
            this.id = id;
            this.minNum = minNum;
            this.maxNum = maxNum;
            this.manualResetEvent = manualResetEvent;
        }
    }
}
