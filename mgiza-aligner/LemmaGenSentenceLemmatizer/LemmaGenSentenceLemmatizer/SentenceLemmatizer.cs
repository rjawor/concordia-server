using LemmaSharp;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LemmaGenSentenceLemmatizer
{
    class SentenceLemmatizer
    {
        private char[] wordInnerSeparator = { '-' };

        private ILemmatizer lemmatizer;

        public SentenceLemmatizer(string languageCode)
        {
            switch (languageCode)
            {
                case "pl":
                    lemmatizer = new LemmatizerPrebuiltCompact(LanguagePrebuilt.Polish);
                    break;
                case "en":
                    lemmatizer = new LemmatizerPrebuiltCompact(LanguagePrebuilt.English);
                    break;
                case "hr":
                    lemmatizer = new LemmatizerPrebuiltCompact(LanguagePrebuilt.Serbian);
                    break;
                default:
                    throw new ArgumentException("Unknown language code: " + languageCode);
            }
        }

        public string lemmatizeSentence(string sentence)
        {
            string[] tokens = sentence.Split(null);

            string result = "";
            foreach (string token in tokens)
            {
                result += lemmatizeWord(token) + " ";
            }

            return result.Trim();
        }

        private string lemmatizeWord(string word)
        {
            if (word.StartsWith("ne_") || word == "i" || word == "o" || word=="do")
            {
                return word;
            }
            string result = "";
            string[] parts = word.Split(wordInnerSeparator);
            if (parts.Length == 2)
            {
                string firstPart = parts[0];
                if (!parts[0].EndsWith("o"))
                {
                    firstPart = lemmatizer.Lemmatize(firstPart);
                }
                string secondPart = lemmatizer.Lemmatize(parts[1]);
                result = firstPart + "-" + secondPart;
            }
            else
            {
                result = lemmatizer.Lemmatize(word);
            }



            if (result == "" || result.Contains(" "))
            {
                return word;
            }
            else
            {
                return result;
            }
        }



    }
}
