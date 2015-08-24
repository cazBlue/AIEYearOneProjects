using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework.Graphics;

namespace CrossPlatform
{
    public class GameState
    {
        public Game1 m_game;

        public virtual void Init(Game1 a_game)
        {
            m_game = a_game;
        }

        public virtual void Update(float a_dt)
        {

        }

        public virtual void Draw(SpriteBatch a_sb)
        {

        }
    }
}
