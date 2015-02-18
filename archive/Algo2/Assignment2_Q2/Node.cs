using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Assignment2_Q2
{
    class Node: IEqualityComparer<Node>
    {
        public bool[] Bits { get; private set; }
        public int Id { get; private set; }

        public Node(string nodeDescriptor, int id)
        {
            Id = id;
            string[] tokens = nodeDescriptor.Split(' ');

            Bits = new bool[tokens.Length-1];
            for (int index = 0; index < Bits.Length; index++)
                Bits[index] = (int.Parse(tokens[index])==1)?true:false;
        }

        internal int HammingDistance(Node testNode)
        {
            int result = 0;
            for (int index = 0; index < this.Bits.Length; index++)
                result += (testNode.Bits[index] ^ this.Bits[index]) ? 1 : 0;

            return result;
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append("Id: " + Id + ";  ");

            foreach(bool bit in Bits)
                sb.Append((bit==true)?"1 ":"0 ");

            return sb.ToString();
        }

        public bool Equals(Node x, Node y)
        {
            return (x.Id == y.Id);
        }

        public int GetHashCode(Node obj)
        {
            return Id;
        }
    }
}
