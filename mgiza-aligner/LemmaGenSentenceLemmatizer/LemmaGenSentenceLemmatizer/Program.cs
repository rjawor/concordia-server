using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LemmaGenSentenceLemmatizer
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length == 1)
            {
                try
                {
                    SentenceLemmatizer lemmatizer = new SentenceLemmatizer(args[0]);
                    string line = Console.ReadLine();
                    while (line != null)
                    {
                        Console.WriteLine(lemmatizer.lemmatizeSentence(line));
                        line = Console.ReadLine();
                    }

                }
                catch (Exception ex)
                {
                    Console.WriteLine("Exception occurred: " + ex.Message);
                }


            }
            else
            {
                Console.WriteLine("Wrong number of parameters. Please specify the language code as the only parameter");
            }

        }
    }
}
