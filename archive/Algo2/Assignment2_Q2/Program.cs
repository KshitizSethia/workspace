using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Assignment2_Q2
{
    class Program
    {

        // In this question your task is again to run the clustering algorithm from lecture,
        // but on a MUCH bigger graph. So big, in fact, that the distances (i.e., edge costs)
        // are only defined implicitly, rather than being provided as an explicit list.
        // The data set is here. The format is:
        // 
        // [# of nodes] [# of bits for each node's label]
        // [first bit of node 1] ... [last bit of node 1]
        // [first bit of node 2] ... [last bit of node 2]
        // ...
        // For example, the third line of the file "0 1 1 0 0 1 1 0 0 1 0 1 1 1 1 1 1 0 1 0 1 1 0 1"
        // denotes the 24 bits associated with node #2.
        // 
        // The distance between two nodes u and v in this problem is defined as the Hamming distance---
        // the number of differing bits --- between the two nodes' labels. For example, the Hamming 
        // distance between the 24-bit label of node #2 above and the label 
        // "0 1 0 0 0 1 0 0 0 1 0 1 1 1 1 1 1 0 1 0 0 1 0 1" is 3 (since they differ in the 3rd, 7th, and 21st bits).
        // 
        // The question is: what is the largest value of k such that there is a k-clustering with spacing 
        // at least 3? That is, how many clusters are needed to ensure that no pair of nodes with all but 
        // 2 bits in common get split into different clusters?
        // 
        // NOTE: The graph implicitly defined by the data file is so big that you probably can't write it 
        // out explicitly, let alone sort the edges by cost. So you will have to be a little creative to 
        // complete this part of the question. For example, is there some way you can identify the smallest 
        // distances without explicitly looking at every pair of nodes?
        static List<Cluster> Main(string[] args)
        {
            string[] fileLines = (string[])File.ReadAllLines(@"..\..\clustering_test4.txt");

            // build all nodes
            List<Node> nodes = new List<Node>();
            for (int index = 1; index < fileLines.Length; index++)
                nodes.Add(new Node(fileLines[index], index - 1));

            // make cluster list
            List<Cluster> clusters = new List<Cluster>();

            // add 1st node to cluster
            clusters.Add(new Cluster(nodes[0]));

            // add subsequent nodes to new or existing clusters
            for (int nodeCounter = 1; nodeCounter < nodes.Count; nodeCounter++)
            {
                Node targetNode = nodes[nodeCounter];
                List<Cluster> candidateClusters = (from cluster in clusters
                                                   //orderby cluster.MinDistance(targetNode)
                                                   where cluster.CanContain(targetNode)
                                                   select cluster).ToList();

                if (candidateClusters.Count == 0)
                {
                    clusters.Add(new Cluster(targetNode));
                    continue;
                }

                if (candidateClusters.Count > 1)
                {
                    List<Node> mergedNodes = new List<Node>();
                    foreach (Cluster cluster in candidateClusters)
                    {
                        mergedNodes.Union(cluster.Nodes);
                    }
                }
                else
                {
                    candidateClusters[0].Nodes.Add(targetNode);
                }

            }

            // merge clusters which can be merged

            //bool clustersMerged = false;

            //do
            //{
            //    foreach(Cluster cluster in clusters)
            //    {
            //        cluster.Nodes.Union(
            //    }
            //}while(clustersMerged
        }
    }
}
