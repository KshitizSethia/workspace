using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PrimsAlgorithm
{
    class Graph
    {
        public int numVertices    {get; private set;}
        public int numEdges     {get; private set;}
        public List<Edge> edges { get; private set; }

        public HashSet<Vertex> vertices { get; set; }

        public Graph(Edge[] edges):this(new List<Edge>(edges))
        {
        }

        public Graph(/*int numVertices, int numEdges, */List<Edge> edges)
        {
            //this.numVertices = numVertices;
            //this.numEdges = numEdges;
            this.edges = edges;

            vertices = new HashSet<Vertex>();
            foreach (Edge edge in this.edges)
            {
                vertices.Add(edge.Vertices[0]);
                vertices.Add(edge.Vertices[1]);
            }
        }

    }
}
