using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace CrossPlatform
{
    /// <summary>
    /// due to the initial setup the actor list is stored in the game1.cs rather than
    /// in this state as it should be
    /// 
    /// this is due to the implementation of the config file, this needs to be looked at
    /// </summary>
    class GameStateGameOver : GameState
    {
        Texture2D m_bgTex;

        bool m_runOnce;
        bool m_runOnReturn;

        LeaderBoard ldrBrd = new LeaderBoard();

        String m_userMessage;

        //TODO
        //pull in leaderboard and load up

        public override void Init(Game1 a_game)
        {
            base.Init(a_game);

            Console.WriteLine("game over!, checking for high score");
            //Console.WriteLine("Score is: " + m_game.m_score);

            m_userMessage = ""; //will be set in run once in update loop

            ldrBrd.Init(); //opens file or populates leaderboard
            m_runOnce = false;
            m_runOnReturn = false;
            //userMessage = ldrBrd.DisplayLeaderBoard(); //get leaderboard string
            
            m_bgTex = m_game.Content.Load<Texture2D>("gameOver_bg");            
        }

        public override void Update(float a_dt)
        {

            if (m_runOnReturn) //runs on return from add score state
            {
                //display the leaderboard and player score
                ldrBrd.Init(); //refresh the leaderboard from file
                m_userMessage = ldrBrd.DisplayLeaderBoard(); //print leaderboard to console
                m_userMessage += "Your score: " + m_game.m_score;
                m_runOnReturn = false; //don't run this again
            }


            //run once used to check if a new high score needs to be added
            if (!m_runOnce)
            {
                int getNewIndex = ldrBrd.SearchForNewHighScoreIndex(m_game.m_score); //debug test for this score
                if (getNewIndex != -1) //if the score is valid add it to the leaderboard
                {                    
                    GameStateAddScore scoreState = new GameStateAddScore();
                    m_game.m_stateMangr.Cmd("addScore", StateManager.cmds.PUSH, scoreState);
                }
                else
                {                    
                    m_userMessage = ldrBrd.DisplayLeaderBoard(); //print leaderboard to console
                    m_userMessage += "Your Score: " + m_game.m_score.ToString();
                }

                m_runOnce = true;
                m_runOnReturn = true; //make sure we update the display for the user
            }
            


            //update the splash screen actors
            //note that this is needed to have the sprites process/display
            for (int i = 0; i < m_game.m_actorListSplash.Count; i++)
            {
                m_game.m_actorListSplash[i].Update(m_game.m_deltaTime);
            }

            //check for keyboard input
            KeyboardState myKeyboard = Keyboard.GetState();
            //check for exit game
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
                m_game.m_exit = true;

            //check for space to begin game
            if (myKeyboard.IsKeyDown(Keys.Space))
            {
                GameStateMainGame newGame = new GameStateMainGame();
                //boot up the main game!
                //m_game.m_stateMangr.Cmd("game", StateManager.cmds.PUSH, newGame);
            }
        }

        public override void Draw(SpriteBatch a_sb)
        {            
            //draw end screen game over

            //final position and dimensions on screen
            Rectangle dstRect = new Rectangle();
            dstRect.X = 0;
            dstRect.Y = 0;
            dstRect.Width = 1200;
            dstRect.Height = 800;

            a_sb.Draw(m_bgTex,
                dstRect,
                dstRect,
                Color.White,
                0,
                new Vector2(0,0),
                SpriteEffects.None,
                1);

            a_sb.DrawString(m_game.m_font, m_userMessage, new Vector2(30, 30), Color.Black);            
        }
    }
}