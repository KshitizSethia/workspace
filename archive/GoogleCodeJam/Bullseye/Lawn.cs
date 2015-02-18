using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Lawnmower
{
    class Lawn
    {
        internal int id;
        internal bool isMowable;

        internal int lawnRows;
        internal int lawnColumns;
        internal string[] unparsedLawn;
        internal int[,] parsedLawn;
        internal System.Threading.ManualResetEvent manualResetEvent;

        public Lawn(int id, int lawnRows, int lawnColumns, string[] unparsedLawn, System.Threading.ManualResetEvent manualResetEvent)
        {
            this.id = id;
            this.lawnRows = lawnRows;
            this.lawnColumns = lawnColumns;
            this.parsedLawn = new int[lawnRows, lawnColumns];
            this.unparsedLawn = unparsedLawn;
            this.manualResetEvent = manualResetEvent;
        }

    }
}
