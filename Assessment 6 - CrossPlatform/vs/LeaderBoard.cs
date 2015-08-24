using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.Serialization.Formatters.Binary;
using System.Runtime.Serialization;
using System.Linq;

namespace CrossPlatform
{
    class LeaderBoard
    {
        class Entries
        {   //custom containfor for leaderboard entries, sortable, searchable and savable            
            public List<int> m_scores = new List<int>();
            public List<String> m_names = new List<string>();
        }

        public const int mc_maxEntries = 6; //how many entries does the leader board allow

        Entries m_currentEntries = new Entries();

        String m_filePath = "DataFile.dat";
        
        public void Init()
        {            
            //init can be called to refresh the board
            m_currentEntries = new Entries(); //clear entried

            if (File.Exists(m_filePath)) //check if the leaderboard file exists, is so use it
            {
                Console.WriteLine("file found, loading high scores");
                ParseLeaderBoard();
            }
            else
            {
                FileStream fs = new FileStream(m_filePath, FileMode.Create); //create the file for later dumping            
                fs.Close();
                // default leaderboard
                m_currentEntries.m_names.Add("Hero");
                m_currentEntries.m_scores.Add(5);
                m_currentEntries.m_names.Add("Guran");
                m_currentEntries.m_scores.Add(10);
                m_currentEntries.m_names.Add("Kit");
                m_currentEntries.m_scores.Add(9);
                m_currentEntries.m_names.Add("Ghost");
                m_currentEntries.m_scores.Add(3);
                m_currentEntries.m_names.Add("Devil");
                m_currentEntries.m_scores.Add(1);
                m_currentEntries.m_names.Add("Diana");
                m_currentEntries.m_scores.Add(30);

                m_currentEntries = SortLeaderBoard(m_currentEntries); //ensure we have a sorted leaderboard  

                Serialize(); //ensure there is data in the file
            }
        }

        private void ParseLeaderBoard()
        {
            //converts hash table to custom type            

            Hashtable hTable = Deserialize(); //get the hastable from file

            //populate the current leaderboard entries
            ICollection key = hTable.Keys;
            foreach (string k in key)
            {
                //Console.WriteLine(k + ": " + hTable[k]);

                m_currentEntries.m_names.Add(hTable[k].ToString());
                m_currentEntries.m_scores.Add(Int32.Parse(k));
            }
            
            m_currentEntries =  SortLeaderBoard(m_currentEntries); //ensure we have a sorted leaderboard            
        }

        public String DisplayLeaderBoard()
        {
            //prints leader board to console and returns leaderboard as string

            String board = "";

            for (int i = 0; i < m_currentEntries.m_scores.Count; i++)
            {
                Console.WriteLine("name: " + m_currentEntries.m_names[i] + " score: " + m_currentEntries.m_scores[i]);
                board += "name: " + m_currentEntries.m_names[i] + " score: " + m_currentEntries.m_scores[i] + "\n";
            }

            return board;
        }

        public void Serialize()
        {
            //https://msdn.microsoft.com/en-us/library/system.runtime.serialization.formatters.binary.binaryformatter(v=vs.110).aspx?cs-save-lang=1&cs-lang=csharp#code-snippet-1
            //http://www.tutorialspoint.com/csharp/csharp_hashtable.htm

            //Note that the key and value are reversed to allow the hash table to ensure unique scores and allow non unique names

            // Create a hashtable of values that will be serialized.
            Hashtable addresses = new Hashtable();

            //pass in the leaderboard to the has table
            for (int i = 0; i < m_currentEntries.m_scores.Count; i++)
            {
                addresses.Add(m_currentEntries.m_scores[i].ToString(), m_currentEntries.m_names[i]);
            }


            //Console.WriteLine(addresses[1]);
            FileStream fs = new FileStream(m_filePath, FileMode.Create);

            // To serialize the hashtable and its key/value pairs,   
            // you must first open a stream for writing.  
            // In this case, use a file stream.
            //FileStream fs = new FileStream("DataFile.dat", FileMode.OpenOrCreate);

            // Construct a BinaryFormatter and use it to serialize the data to the stream.
            BinaryFormatter formatter = new BinaryFormatter();
            try
            {
                formatter.Serialize(fs, addresses);
            }
            catch (SerializationException e)
            {
                Console.WriteLine("Failed to serialize. Reason: " + e.Message);
                throw;
            }
            finally
            {
                fs.Close();
            }
        }

        /// <summary>
        /// sort the leader board based on score
        /// returns the sorted leaderboard
        /// </summary>
        /// <returns></returns>
        private Entries SortLeaderBoard(Entries board)
        {
            //using a quick sort algorythm to sort the leaderboard
            //reference from http://www.sorting-algorithms.com/quick-sort
            // and http://www.sorting-algorithms.com/static/QuicksortIsOptimal.pdf
            //http://betterexplained.com/articles/sorting-algorithms/

            Entries lesser = new Entries();
            Entries greater = new Entries();            

            //get random pivot to work with
            Random rnd = new Random();
            int pivotIndex = rnd.Next(board.m_names.Count); // creates a number between 0 and entries length
            pivotIndex = 1; //debug ovverride
            String pivotName = board.m_names[pivotIndex];
            int pivotScore = board.m_scores[pivotIndex];
            board.m_names.RemoveAt(pivotIndex); //remove the pivot entry from the index
            board.m_scores.RemoveAt(pivotIndex); //
            
            //sort the board into greater/lesser fields            
            for (int i = 0; i < board.m_scores.Count; i++)
            {
                if (board.m_scores[i] >= pivotScore)
                {
                    lesser.m_scores.Add(board.m_scores[i]);
                    lesser.m_names.Add(board.m_names[i]);
                }
                else
                {
                    greater.m_scores.Add(board.m_scores[i]);
                    greater.m_names.Add(board.m_names[i]);
                }
            }

            //check if we are done, if not recursivly go till we are
            if (lesser.m_scores.Count > 1)
                lesser = SortLeaderBoard(lesser);

            if (greater.m_scores.Count > 1)
                greater = SortLeaderBoard(greater);

            //build the resulting entry and return it
            Entries result = new Entries();

            result.m_names.AddRange(lesser.m_names);
            result.m_names.Add(pivotName);
            result.m_names.AddRange(greater.m_names);

            result.m_scores.AddRange(lesser.m_scores);
            result.m_scores.Add(pivotScore);
            result.m_scores.AddRange(greater.m_scores);

            return result;
        }



        //add high score

        /// <summary>
        /// assumes the leaderboard is sorted
        /// Searches the high scores to see if a new high score is to be created!
        /// sets the passed a_index if valid
        /// </summary>
        public int SearchForNewHighScoreIndex(int a_score)
        {            
            //starts at the "top" of the leader board and searches downards looking to see if there
            //is a spot on the leaderboard for this score, taking the max leaderboard entries allows into
            // account         
            int index = -1;

            if (mc_maxEntries > m_currentEntries.m_scores.Count)
            {
                //score automatically is ok when there arn't enough entries, return the correct index to add to
                return m_currentEntries.m_scores.Count;
            }
            else
            {
                //loop through all scores (descending) and see if the passed score is higher than any
                //find the highest and set the index, if none found return -1
                for (int i = m_currentEntries.m_scores.Count - 1; i >= 0; i--)
                {
                    if (m_currentEntries.m_scores[i] <= a_score)
                    {
                        //NOTE - due to hash tables not allows non unique keys scores with the same value are replaced
                        //this may be undesirable behaviour

                        //a valid score has been found to override                        
                        index = i; //hit the correct index                        
                    }
                }
            }

            return index;
        }

        
        public bool AddNewHighScore(int a_score, String a_name, int a_index)
        {
            bool scoreAdded = false;

            if (a_index < m_currentEntries.m_scores.Count && a_index >= 0)
            {
                //insert the new score
                m_currentEntries.m_scores.Insert(a_index, a_score);
                m_currentEntries.m_names.Insert(a_index, a_name);

                //check if we need to prune the score list
                //check if there is a matching score, remove it
                if (m_currentEntries.m_scores[a_index + 1] == m_currentEntries.m_scores[a_index])
                {
                    m_currentEntries.m_scores.RemoveAt(a_index + 1);
                    m_currentEntries.m_names.RemoveAt(a_index + 1);
                }
                else if (m_currentEntries.m_scores.Count > mc_maxEntries)
                {
                    //remove the last entry 
                    m_currentEntries.m_scores.RemoveAt(m_currentEntries.m_scores.Count - 1);
                    m_currentEntries.m_names.RemoveAt(m_currentEntries.m_names.Count - 1);
                }

                Serialize();
            }

            return scoreAdded;
        }
        


        public Hashtable Deserialize()
        {
            // Declare the hashtable reference.
            Hashtable ldrBoard = null;

            // Open the file containing the data that you want to deserialize.
            FileStream fs = new FileStream(m_filePath, FileMode.Open);
            try
            {
                BinaryFormatter formatter = new BinaryFormatter();

                // Deserialize the hashtable from the file and  
                // assign the reference to the local variable.
                ldrBoard = (Hashtable)formatter.Deserialize(fs);
            }
            catch (SerializationException e)
            {
                Console.WriteLine("Failed to deserialize. Reason: " + e.Message);
                throw;
            }
            finally
            {
                fs.Close();
            }

            return ldrBoard;
        }

    }
}
