using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AlgoHelperLib.Graph
{
    public class Vertex
    {
        public int Id { get; private set; }

        public Vertex(int id)
        {
            this.Id = id;
        }

        public override string ToString()
        {
            return "Id: " + Id;
        }

        public override bool Equals(object other)
        {
            return this.Id == ((Vertex) other).Id;
        }

        public override int GetHashCode()
        {
            return this.Id;
        }
    }
}
