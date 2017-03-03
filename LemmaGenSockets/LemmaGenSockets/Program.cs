using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;
using LemmaSharp;

namespace LemmaGenSockets
{
    class Program
    {

        // Incoming data from the client.  
        public static string data = null;



        static void Main(string[] args)
        {
            LemmatizerListener listener = new LemmatizerListener();
            listener.DoListening();
        }
    }
}
