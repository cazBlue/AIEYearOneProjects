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
    public enum CLASSID
    {
        CLASSID_ACTOR,
        CLASSID_PLAYER,
        CLASSID_PLANETSPHERE,
        CLASSID_PLANETCUBE,
        CLASSID_BACKGROUND
    }

    public enum DRAWLAYER
    {
        DRAWLAYER_BEHIND,
        DRAWLAYER_MAIN,
        DRAWLAYER_INFRONT,
        DRAWLAYER_BACKGROUND,
        DRAWLAYER_PLAYER,
        DRAWLAYER_OTHER
    }

    public class Actor
    {
        protected Game1 m_game;
        protected SpriteBatch m_sb;

        public Vector2 m_pos;
        public float m_rot;
        public Matrix m_matrix;
        public Vector2 m_centre;

        public Vector2 m_heading;
        public Vector2 m_velocity;                
        public Vector2 m_accelaration;
        public Vector2 m_force;
        
        public float m_mass;
        public float m_friction;

        /*
         *   each actor can have three lists of sprites
         *   these are based so that the player will always sit at the desired render level regardless
         *   of how many sprites the are to render for a given actor
         *   
         *  Actors are looped over three times by the main loop to draw each layer in order
         *
        */


        public SpriteManager m_spriteManager;        


        public SpriteEffects m_spriteEffect;
        public bool m_drawCollision;

        Texture2D m_lineTexture;

        public float m_scale;

        public float m_radius;

        public CLASSID m_classID;

        public virtual void Update(float a_DT)
        {
            m_spriteManager.Update(a_DT);

        }


        public virtual void Draw(SpriteBatch a_sb, DRAWLAYER a_layer)
        {

            List<Texture2D> spriteList = new List<Texture2D>();

            m_spriteManager.Draw(a_sb, a_layer);

            if (m_drawCollision)
                DrawCircle(a_sb, m_pos.X, m_pos.Y, (int)(m_radius * m_scale), 36);
            
        }

        public virtual void Init(Game1 inGame, SpriteBatch a_sb, CLASSID a_type)
        {
            m_classID = a_type;

            m_game = inGame;
            m_sb = a_sb;

            m_spriteEffect = SpriteEffects.None;

            m_matrix = Matrix.Identity;

            m_drawCollision = true;

            m_scale = 1;

            m_lineTexture = m_game.Content.Load<Texture2D>("edge_plain");

            m_classID = CLASSID.CLASSID_ACTOR;

            m_spriteManager = new SpriteManager();

            //init the sprite manager
            m_spriteManager.Init(this);
        }

        /// <summary>
        /// Get the up rotation of a matrix built from the actors matrix     
        /// Note: this is always the UP rotation
        /// </summary>
        /// <param name="a_matrix">matrix to get rotation off</param>
        /// <returns>UP rotation as radian (float)</returns>
        public float GetRotFromMatrix(Matrix a_matrix)
        {
            //define the up vector
            Vector2 up = new Vector2(0,1);            
            
            //get vector 3 rotation
            Vector3 myRot = a_matrix.Up;
            
            //pass to vector 2 (Z axis not in use
            Vector2 myRotv2 = new Vector2(myRot.X, myRot.Y);

            //check for 0 length to avoid NaN on normalise
            if (myRotv2.Length() > 0)
            {
                myRotv2.Normalize();
                //get the final rotation of the matrix
                return GetAngleBetween(up, myRotv2); ;
            }
            else
                return 0.0f;                    
        }

        /// <summary>
        /// get angle between two vectors
        /// </summary>
        /// <param name="a_start"></param>
        /// <param name="a_end"></param>
        /// <returns></returns>
        public float GetAngleBetween(Vector2 a_start, Vector2 a_end)
        {
            //get lhs normalised
            Vector2 l = a_start;
            l.Normalize();

            //get rhs normalised
            Vector2 r = a_start;
            r.Normalize();

            //get perpendicular to lhs
            Vector2 AP = new Vector2(l.Y, -l.X);

                            
            //get the rotation of the angle
            float rot = (float)Math.Acos(Vector2.Dot(a_start, a_end));

            //is the angle negative or not?
            if (Vector2.Dot(AP, a_end) > 0)
                rot = -rot;

            return rot;
        }


        public void DrawCircle(SpriteBatch sb, float cx, float cy, float r, int num_segments)
        {
            float theta = 2 * (3.1415926f / (float)num_segments);
            float c = (float)Math.Cos(theta);//precalculate the sine and cosine
            float s = (float)Math.Sin(theta);
            float t;

            float x = r;//we start at angle = 0 
            float y = 0.0f;

            Vector2[] circleArray = new Vector2[num_segments];

            for (int ii = 0; ii < num_segments; ii++)
            {
                float xStart = x + cx;
                float yStart = y + cy;
                //

                circleArray[ii] = new Vector2(x + cx, y + cy);

                //apply the rotation matrix
                t = x;
                x = c * x - s * y;
                y = s * t + c * y;


                DrawLine(sb, //draw line
                    new Vector2(xStart, yStart), //start of line
                    new Vector2(x + cx, y + cy) //end of line
                    );
            }
        }


        public void DrawLine(SpriteBatch sb, Vector2 start, Vector2 end)
        {
            Vector2 edge = end - start;
            // calculate angle to rotate line
            float angle =
                (float)Math.Atan2(edge.Y, edge.X);


            sb.Draw(m_lineTexture,
                new Rectangle(// rectangle defines shape of line and position of start of line
                    (int)start.X,
                    (int)start.Y,
                    (int)edge.Length() + 3, //sb will strech the texture to fill this rectangle
                    1), //width of line, change this to make thicker line
                null,
                Color.White, //colour of line
                angle,     //angle of line (calulated above)
                new Vector2(0, 0), // point in line about which to rotate
                SpriteEffects.None,
                0);
        }

        public float GetRadius(float a_width, float a_height)
        {
            return (float)Math.Sqrt(((a_width * a_width) / 4) + ((a_height * a_height) / 4));
        }

        public Vector2 GetIntersectingRadiusPoint(float a_angle, float a_radius, Vector2 a_pos)
        {

            Vector2 pointOnCircle = new Vector2((float)Math.Cos(a_angle) * a_radius, (float)Math.Sin(a_angle) * a_radius);

            pointOnCircle += a_pos;

            return pointOnCircle;
        }

        /// <summary>
        /// called by the collision detector, up to child actors to do anything with collisions
        /// used for main planet collisions
        /// </summary>
        /// <param name="a_colActor">colliding actor</param>
        public virtual void IsColliding(Actor a_colActor)
        {

        }

        public virtual void IsCollidingGravityWell(Actor a_colActor)
        {

        }
    }
}
