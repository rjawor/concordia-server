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
                SentenceLemmatizer lemmatizer = new SentenceLemmatizer(args[0]);
                string line = Console.ReadLine();
                while (!string.IsNullOrEmpty(line))
                {
                    Console.WriteLine(lemmatizer.lemmatizeSentence(line));
                    line = Console.ReadLine();
                }


            }
            else
            {
                Console.WriteLine("Wrong number of parameters. Please specify the language code as the only parameter");
            }

        }
    }
}
