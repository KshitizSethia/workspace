using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AlgoHelperLib;

namespace AlgoHelperLib.Graph
{
    public class EdgeBasedGraph
    {
        public HashSet<Vertex> Vertices { get; private set; }
        public List<Edge> Edges { get; private set; }

        /// <summary>
        /// Returns a graph, given it's edge descriptors
        /// </summary>
        /// <param name="edgeDescriptors"> an array of strings describing each edge in the format:
        /// [vertex1] [vertex2] [edgeWeight]
        /// Edge Id will be it's index in this array</param>
        public EdgeBasedGraph(string[] edgeDescriptors)
        {
            Vertices = new HashSet<Vertex>();
            Edges = new List<Edge>();

            for (int index = 0; index<edgeDescriptors.Length; index++)
            {
                string edgeDesc = edgeDescriptors[index];
                string[] tokens = edgeDesc.Split(' ');

                Vertex v1 = new Vertex(int.Parse(tokens[0]));
                Vertex v2 = new Vertex(int.Parse(tokens[1]));

                Vertices.Add(v1);
                Vertices.Add(v2);

                Edge edge = new Edge(index, v1, v2, int.Parse(tokens[2]));

                Edges.Add(edge);
            }
        }
    }
}
