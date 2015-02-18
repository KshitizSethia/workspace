using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AlgoHelperLib.Graph
{
    public class Edge: IComparable<Edge>
    {
        public Vertex[] Vertices {get; private set;}
        public int Weight { get; private set; }
        public int Id { get; private set; }

        public Edge(int Id, Vertex v1, Vertex v2, int weight)
        {
            this.Id = Id;
            this.Vertices = new Vertex[] { v1, v2 };
            this.Weight = weight;
        }

        public override bool Equals(object other)
        {
            return this.Id == ((Edge) other).Id;
        }

        public override int GetHashCode()
        {
            return this.Id;
        }

        public override string ToString()
        {
            return "Id: " + Id + ", vertices: " + Vertices[0] + "  " + Vertices[1] + ", weight: " + Weight; 
        }

        public int CompareTo(Edge other)
        {
            return this.Weight - other.Weight;
        }
    }
}
