using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PrimsAlgorithm
{
    class Program
    {
        // Implementation of Prim's algo as in Cormen (3rd edition, Section 23.2)
        static void Main(string[] args)
        {
            //Answer for Q3: 3612829
            Graph graph = ConstructGraph(@"C:\Dropbox\workspace\Algo2\PrimsAlgorithm\edges.txt");
            

            // Sample data set for verifying correctness
            //Edge[] edges = new Edge[]
            //{
            //    new Edge("1 2 4"),new Edge("1 8 8"),new Edge("2 8 11"),new Edge("2 3 8"),
            //    new Edge("3 9 2"),new Edge("8 9 7"),new Edge("7 8 1"),new Edge("7 9 6"),
            //    new Edge("3 4 7"),new Edge("3 6 4"),new Edge("6 7 2"),new Edge("4 6 14"),
            //    new Edge("4 5 9"),new Edge("5 6 10"),
            //};
            //Graph graph = new Graph(edges);
            
            // start with first vertex of first edge as root
            List<Edge> minSpanningTree = MSTPrims(graph, graph.edges[0].Vertices[0]);

            int costOfMST = 0;
            foreach (Edge edge in minSpanningTree)
                costOfMST += edge.Weight;

            return;
        }

        // Implementation of Prim's algo as in Cormen (3rd edition, Section 23.2)
        private static List<Edge> MSTPrims(Graph graph, Vertex root)
        {
            //steps 1-3
            foreach (Vertex vertex in graph.vertices)
            {
                vertex.Key = double.PositiveInfinity;
                vertex.MinSpanningTreeParent = null;
            }

            //step 4
            graph.vertices.FirstOrDefault(vertex => vertex.Id == root.Id).Key = 0;

            //step 5
            HashSet<Vertex> verticesNotInMST = new HashSet<Vertex>(graph.vertices);
            verticesNotInMST.Remove(root);

            //initialize set of edges which will form the MST
            List<Edge> result = new List<Edge>();

            //step 6
            while (verticesNotInMST.Count > 0)
            {
                // get list of edges crossing cut
                List<Edge> edgesCrossingCut = graph.edges.FindAll(edge =>
                                verticesNotInMST.Contains(edge.Vertices[0])
                                ^ verticesNotInMST.Contains(edge.Vertices[1]));
                
                // sort the crossing edges
                edgesCrossingCut.Sort();

                // add light edge crossing current cut to mst
                Edge lightEdge = edgesCrossingCut[0];
                result.Add(edgesCrossingCut[0]);

                // remove vertices of this edge from verticesNotInMST
                verticesNotInMST.Remove(lightEdge.Vertices[0]);
                verticesNotInMST.Remove(lightEdge.Vertices[1]);
            }


            return result;
        }

        private static Graph ConstructGraph(string inputFile)
        {
            string[] fileLines = (string[])File.ReadAllLines(inputFile);

            string[] edgesAndVertices = fileLines[0].Split(' ');
            int numNodes = int.Parse(edgesAndVertices[0]);
            int numEdges = int.Parse(edgesAndVertices[1]);

            List<Edge> edges = new List<Edge>();
            for (int index = 1; index <= numEdges; index++)
            {
                edges.Add(new Edge(fileLines[index]));
            }

            Graph graph = new Graph(edges);

            return graph;
        }
    }
}
