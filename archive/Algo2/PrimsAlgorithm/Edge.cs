using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PrimsAlgorithm
{
    class Edge: IComparable<Edge>
    {
        public Vertex[] Vertices { get; private set; }
        public int Weight { get; private set; }

        public Edge(string edgeDescriptor)
        {
            string[] tokens = edgeDescriptor.Split(' ');

            //double storage of Vertices!! make these refer to the set of vertices in graph data structure
            Vertices = new Vertex[2] { new Vertex(int.Parse(tokens[0])), new Vertex(int.Parse(tokens[1])) };
            Weight = int.Parse(tokens[2]);
        }

        public int CompareTo(Edge other)
        {
            return (this.Weight - other.Weight);
        }

        public override string ToString()
        {
            return Vertices[0].Id + " " + Vertices[1].Id + " " + Weight;
        }
    }
}
