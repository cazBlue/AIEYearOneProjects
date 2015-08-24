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
    class GameStateSplash : GameState
    {

        public override void Init(Game1 a_game)
        {
            base.Init(a_game);

            //a_game.updateCamera(new Vector2(0,0));
        }        


        public override void Update(float a_dt)
        {
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
                m_game.m_stateMangr.Cmd("game", StateManager.cmds.PUSH, newGame);
            }

        }

        public override void Draw(SpriteBatch a_sb)
        {
            for (int i = 0; i < m_game.m_actorListSplash.Count; i++)
            {
                m_game.m_actorListSplash[i].Draw(a_sb, DRAWLAYER.DRAWLAYER_OTHER);
            }            
        }
    }
}
