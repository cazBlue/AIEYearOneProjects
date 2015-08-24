using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;

namespace CrossPlatform
{
    class Planet : Actor
    {     
        public float m_planetGravity;

        public float m_effectiveRadius;

        public bool m_drawEfftiveRadius;

        //public float m_radius;

        public override void Init(Game1 inGame, Microsoft.Xna.Framework.Graphics.SpriteBatch a_sb, CLASSID a_type)
        {
            base.Init(inGame, a_sb, a_type);

            //m_classID = CLASSID.CLASSID_PLANET;

            //m_game.Content.Load<Texture2D>("planet");

            m_rot = 0.0f;

            m_pos = new Vector2(0, 0);

            m_centre = new Vector2(0,0);

            m_radius = -1;

//            m_radius = GetRadius(m_sprite.Width, m_sprite.Height);

        }

        public override void Update(float a_DT)
        {
            base.Update(a_DT);
        }

        public override void Draw(Microsoft.Xna.Framework.Graphics.SpriteBatch a_sb, DRAWLAYER a_layer)
        {
            base.Draw(a_sb, a_layer);

            if (m_drawEfftiveRadius)
                DrawCircle(a_sb, m_pos.X, m_pos.Y, m_effectiveRadius * m_scale, 36);

        }      
    }
}
