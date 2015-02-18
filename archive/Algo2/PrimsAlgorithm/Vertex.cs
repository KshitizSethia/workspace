using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PrimsAlgorithm
{
    /// <summary>
    /// as used by prims algorithm
    /// </summary>
    class Vertex
    {
        public int Id { get; private set; }

        public double Key;
        public Vertex MinSpanningTreeParent;
        
        public Vertex(int id)
        {
            Id = id;
        }

        public override bool Equals(object other)
        {
            if (other is Vertex)
            { return (this.Id == ((Vertex)other).Id); }

            return false;
        }
        public override int GetHashCode()
        {
            return Id.GetHashCode();
        }

        public override string ToString()
        {
            return Id.ToString();
        }
    }
}
