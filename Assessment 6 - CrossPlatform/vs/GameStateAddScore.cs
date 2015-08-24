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
    class GameStateAddScore : GameState
    {
        Texture2D m_bgTex;        

        LeaderBoard ldrBrd = new LeaderBoard();

        String m_userMessage;
        
        int m_runCount;

        //TODO
        //pull in leaderboard and load up

        public override void Init(Game1 a_game)
        {
            base.Init(a_game);

            Console.WriteLine("game over!");
            Console.WriteLine("Score is: " + m_game.m_score);

            m_runCount = 0;

            ldrBrd.Init(); //opens file or populates leaderboard

            //m_userMessage = ldrBrd.DisplayLeaderBoard(); //print leaderboard to console

            m_userMessage += " \n You got a high score! \n please enter your name \n in the console";
            
            m_bgTex = m_game.Content.Load<Texture2D>("gameOver_bg");
        }

        public override void Update(float a_dt)
        {
            if (m_runCount > 10)
            {
                //make sure the spritebactch has been called on the draw string, it seems to lag behind
                //draw sprite... 
                int getNewIndex = ldrBrd.SearchForNewHighScoreIndex(m_game.m_score); //debug test for this score                
                String name = Console.ReadLine();
                ldrBrd.AddNewHighScore(m_game.m_score, name, getNewIndex);                           

                //pop this state to return to game over state
                m_game.m_stateMangr.Cmd("addScore", StateManager.cmds.POP, this);
            }
            else
            {
                m_runCount++;                
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
                new Vector2(0, 0),
                SpriteEffects.None,
                1);

            a_sb.DrawString(m_game.m_font, m_userMessage, new Vector2(30, 30), Color.Black);
        }
    }
}