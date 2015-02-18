using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Assignment2_Q2
{
    class Cluster
    {
        public List<Node> Nodes;
        public Guid Id { get; private set; }

        public Cluster(Node node)
        {
            Nodes = new List<Node>();
            Nodes.Add(node);

            Id = new Guid();
        }

        public bool CanContain(Node targetNode)
        {
            if (Nodes.Min(node => node.HammingDistance(targetNode)) > 2)
                return false;
         
            return true;
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder("\n{\n");

            foreach (Node node in Nodes)
                sb.AppendLine("\t " + node.ToString());
            sb.AppendLine("}");

            return sb.ToString();
        }

        public override int GetHashCode()
        {
            return this.Id.GetHashCode();
        }
        //internal int MinDistance(Node targetNode)
        //{
        //    return Nodes.Min(node => node.HammingDistance(targetNode));
        //}
    }
}
