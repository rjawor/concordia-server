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
    class LemmatizerListener
    {
        private Dictionary<String, ILemmatizer> lemmatizersDict = new Dictionary<string, ILemmatizer>();

        private char[] wordInnerSeparator = { '-' };
        

        private void initializeLemmatizers()
        {
            lemmatizersDict.Add("pl", new LemmatizerPrebuiltCompact(LemmaSharp.LanguagePrebuilt.Polish));
            lemmatizersDict.Add("en", new LemmatizerPrebuiltCompact(LemmaSharp.LanguagePrebuilt.English));
        }

        public LemmatizerListener()
        {
            initializeLemmatizers();
        }

        private string lemmatizeSentence(string languageCode, string sentence)
        {
            string[] tokens = sentence.Split(null);

            string result = "";
            foreach (string token in tokens)
            {
                result += lemmatizeWord(languageCode, token) + " ";
            }

            return result.Trim();
        }

        private string lemmatizeWord(string languageCode, string word)
        {
            string[] parts = word.Split(wordInnerSeparator);
            if (parts.Length == 2)
            {
                string firstPart = parts[0];
                if (!parts[0].EndsWith("o"))
                {
                    firstPart = lemmatizersDict[languageCode].Lemmatize(firstPart);
                }
                string secondPart = lemmatizersDict[languageCode].Lemmatize(parts[1]);
                return firstPart + "-" + secondPart;
            }
            else
            {
                return lemmatizersDict[languageCode].Lemmatize(word);
            }
        }

        public void DoListening()
        {
            // Data buffer for incoming data.  
            byte[] bytes = new Byte[1024];

            string data;

            // Establish the local endpoint for the socket.  
            IPAddress ipAddress = IPAddress.Parse("127.0.0.1");
            IPEndPoint localEndPoint = new IPEndPoint(ipAddress, 11000);

            // Create a TCP/IP socket.  
            Socket listener = new Socket(AddressFamily.InterNetwork,
                SocketType.Stream, ProtocolType.Tcp);

            // Bind the socket to the local endpoint and   
            // listen for incoming connections.  
            try
            {
                listener.Bind(localEndPoint);
                listener.Listen(10);

                // Start listening for connections.  
                while (true)
                {
                    // Program is suspended while waiting for an incoming connection.  
                    Socket handler = listener.Accept();
                    data = null;

                    // An incoming connection needs to be processed.  
                    while (true)
                    {
                        bytes = new byte[1024];
                        int bytesRec = handler.Receive(bytes);
                        data += Encoding.UTF8.GetString(bytes, 0, bytesRec);
                        if (data.IndexOf("@#@") > -1)
                        {
                            break;
                        }
                    }

                    data = data.Substring(0, data.IndexOf("@#@"));

                    string languageCode = data.Substring(0, 2);
                    string sentence = data.Substring(2);


                    // Show the data on the console.  
                    // Console.WriteLine("Sentence received : "+ sentence + ", language code : "+languageCode);

                    // Send lemmatized data back to client.  
                    byte[] msg = Encoding.UTF8.GetBytes(lemmatizeSentence(languageCode, sentence) + "@#@");

                    handler.Send(msg);
                    handler.Shutdown(SocketShutdown.Both);
                    handler.Close();
                }

            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }

        }


    }
}
