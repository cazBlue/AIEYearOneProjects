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
    /// currently the actor list for the game is stored in the main game rather
    /// than this game state, this is due to the early implementation of the config file in the main game loop
    /// this should be split into 2 parts
    /// config
    /// state
    /// and then the states should handle their own actor lists rather than the "universal" one that is in place
    /// currently
    /// </summary>
    class GameStateMainGame : GameState
    {
        CollisionDector colDector = new CollisionDector();

        float m_gameTimeCounter = 30;
        float m_timeSinceLast = 0;
        
        //list of all actors in game
        //public List<Actor> m_actorList = new List<Actor>();

        public override void Init(Game1 a_game)
        {            
            base.Init(a_game);            
        }

        public override void Update(float a_dt)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
                m_game.m_exit = true;

            //update game actors
            for (int i = 0; i < m_game.m_actorList.Count; i++)            
                m_game.m_actorList[i].Update(m_game.m_deltaTime);            

            //check for collisions
            colDector.CheckCollisions(m_game.m_actorList);

            //check for gravity well collision
            colDector.CheckGravityCollisions(m_game.m_actorList);

            //check for reload
            if (Keyboard.GetState().IsKeyDown(Keys.R))            
                m_game.gameConfig.hotReload(m_game, m_game.spriteBatch, m_game.m_actorList, m_game.m_actorListSplash);            

            //update the current time counter 
            m_timeSinceLast += a_dt;

            if (m_timeSinceLast >= 10)
            {
                if (m_gameTimeCounter <= 0)
                {
                    //kick of the lose screen state
                    m_game.m_stateMangr.Cmd("game", StateManager.cmds.PUSH, new GameStateGameOver());
                }

                m_gameTimeCounter -= 1;
                m_timeSinceLast = 0;
            }
        }

        public override void Draw(SpriteBatch a_sb)
        {
            //draw all background layers
            for (int i = 0; i < m_game.m_actorList.Count; i++)
            {
                m_game.m_actorList[i].Draw(a_sb, DRAWLAYER.DRAWLAYER_BACKGROUND);
            }

            //draw all actors behind elements
            for (int i = 0; i < m_game.m_actorList.Count; i++)
            {
                m_game.m_actorList[i].Draw(a_sb, DRAWLAYER.DRAWLAYER_BEHIND);
            }

            //draw all actors main elements
            for (int i = 0; i < m_game.m_actorList.Count; i++)
            {
                if (m_game.m_actorList[i].m_classID != CLASSID.CLASSID_PLAYER)
                    m_game.m_actorList[i].Draw(a_sb, DRAWLAYER.DRAWLAYER_MAIN);
            }

            //draw player elements
            for (int i = 0; i < m_game.m_actorList.Count; i++)
            {
                if (m_game.m_actorList[i].m_classID == CLASSID.CLASSID_PLAYER)
                    m_game.m_actorList[i].Draw(a_sb, DRAWLAYER.DRAWLAYER_PLAYER);
            }

            //draw front elements
            for (int i = 0; i < m_game.m_actorList.Count; i++)
            {
                if (m_game.m_actorList[i].m_classID != CLASSID.CLASSID_PLAYER)
                    m_game.m_actorList[i].Draw(a_sb, DRAWLAYER.DRAWLAYER_INFRONT);
            }

            //draw fonts
            a_sb.DrawString(m_game.m_font, "Score: " + m_game.m_score, new Vector2(30, m_game.m_screenHeight - 80), Color.Black);
            a_sb.DrawString(m_game.m_font, "Time: " + Math.Floor(m_gameTimeCounter), new Vector2(30, m_game.m_screenHeight - 120), Color.White);
        }
    }
}