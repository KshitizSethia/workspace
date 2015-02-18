using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Text;
using AlgoHelperLib.Graph;
using System.Collections.Generic;

namespace Test.KruskalUnionFind
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public void GraphConstruct()
        {
            List<string> egdeDescriptors = new List<string>();

            egdeDescriptors.Add("1 2 0");
            egdeDescriptors.Add("2 3 1");
            egdeDescriptors.Add("3 1 3");
            egdeDescriptors.Add("1 4 4");
            egdeDescriptors.Add("4 2 5");

            EdgeBasedGraph graph = new EdgeBasedGraph(egdeDescriptors.ToArray());

            Assert.IsTrue(graph.Vertices.Count == 4);
            Assert.IsTrue(graph.Edges.Count == 5);
        }

        [TestMethod()]
        public void KruskalsUnionFind()
        {

        }
    }
}
