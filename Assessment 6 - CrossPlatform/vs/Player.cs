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

using Microsoft.Xna.Framework.Input.Touch; 

///Radius
/////the player is assumed to have a radius based on texture size


namespace CrossPlatform
{
    class Player : Actor
    {
        float m_planetRot;
        Vector2 m_gravHeading;

        Vector2 m_gravityVelocity;

        bool m_hasCollided;
        bool m_killUpVelocity;

        public float m_spaceMoveSpeed;
        public float m_planetMoveSpeed;

        private List<string> m_jumpStates = new List<string>();

        public float m_jumpSpeed;

        //current planet the player is aimed at
        public Planet m_curPlanet;
        //last planet
        private bool m_newPlanet;

        public float m_width, m_height;

        private Boolean m_isJumping;

        private Boolean lastDir;    //last direction the player was moving - used to set correct idle animation orientation etc - true is left

        public override void Init(Game1 inGame, SpriteBatch a_sb, CLASSID a_type)
        {
            lastDir = false;
            base.Init(inGame, a_sb, a_type);

            m_classID = CLASSID.CLASSID_PLAYER;

            //hard set mass and friction
            m_mass = 50.0f;
            m_friction = 10.0f;
            
            
            m_hasCollided = false;

//            m_sprite = m_game.Content.Load<Texture2D>("player");

            m_rot = 0.0f;

//            m_centre = new Vector2(m_sprite.Width / 2, m_sprite.Height / 2);

            m_matrix *= Matrix.CreateTranslation(new Vector3(300,300,0));

            m_heading = new Vector2(0, 0);

//            m_pos = new Vector2(m_matrix.Translation.X, m_matrix.Translation.Y);

            m_pos = new Vector2(0, 0);

            m_planetRot = 0f;

            m_gravHeading = new Vector2(0, 0);

            m_newPlanet = false;

            m_killUpVelocity = false;

            m_isJumping = false;

            m_jumpStates.Add("start");
            m_jumpStates.Add("loop");
            m_jumpStates.Add("end");
        }


        public override void Update(float a_DT)
        {
            base.Update(a_DT);

            GamePadState currentState = GamePad.GetState(PlayerIndex.One);

            KeyboardState myKeyboard = Keyboard.GetState();

            TouchCollection touchCollection;

            touchCollection = TouchPanel.GetState();

            bool moveLeft = false;
            bool moveRight = false;
            bool jump = false;

            //android controls
            foreach(TouchLocation tl in touchCollection)
            {
                //Console.WriteLine("pos" + tl.Position);
                //Console.WriteLine("state" + tl.State);


                //check for touch on left of screen
                if (tl.Position.X < 200 && tl.Position.Y < 600)
                {
                    Console.WriteLine("left of screen");
                    moveLeft = true;
                }                

                if (tl.Position.X > 1700 && tl.Position.Y < 600)
                {
                    Console.WriteLine("right of screen");
                    moveRight = true;
                }

                if (tl.Position.Y > 600)
                {
                    Console.WriteLine("jump");
                    jump = true;
                }
            }

            if (myKeyboard.IsKeyDown(Keys.D) || currentState.ThumbSticks.Left.X > 0 || moveLeft)
            {
                if(currentState.ThumbSticks.Left.X > 0)
                    m_force.X += m_spaceMoveSpeed * currentState.ThumbSticks.Left.X;
                else
                    m_force.X += m_spaceMoveSpeed;
            }

            if (myKeyboard.IsKeyDown(Keys.A) || currentState.ThumbSticks.Left.X < 0 || moveRight)
            {
                if(currentState.ThumbSticks.Left.X < 0)
                    m_force.X -= m_spaceMoveSpeed * -currentState.ThumbSticks.Left.X;
                else
                    m_force.X -= m_spaceMoveSpeed;
            }

            if (myKeyboard.IsKeyDown(Keys.Space) || currentState.Buttons.A == ButtonState.Pressed || jump)
            {
                if (m_hasCollided)
                {
                    m_force.Y -= m_jumpSpeed * 500;
                    m_hasCollided = false;
                }
            }

            updateAnimState();

            //apply physics and update players position
            //m_force should be final before this is applied
            UpdatePhysics(a_DT);

            UpdateTransVars();

            LerpCamera(a_DT);
        }

        enum ANIM_STATE
        {
            ANIM_STATE_JUMP,
            ANIM_STATE_IDLE,
            AMIN_STATE_RUN
        }

        public void updateAnimState()
        {
            if (m_force.Y < 0 && !m_isJumping)
            {
                //Console.WriteLine("jump");
                //PLAY JUMP ANIMATION
                //run the jump start animation
                m_spriteManager.Cmd(SpriteManager.SpriteCommands.SWITCH, "player", m_jumpStates[0]);
                //que the jump loop animation
                m_spriteManager.Cmd(SpriteManager.SpriteCommands.TRANSITION, "player", m_jumpStates[1]);

                m_isJumping = true;

                if (!lastDir)
                    m_spriteEffect = SpriteEffects.None;
                else
                    m_spriteEffect = SpriteEffects.FlipHorizontally;


                //TODO - not viable with current implementation/structure
                /*
                 * using delegates add the ability for a 3 part jump
                 * each delegate should pass the animation to the next, so
                 * 
                 * read: http://stackoverflow.com/questions/2019402/when-why-to-use-delegates
                 * 
                 *  jump start > jump loop > jump end
                 * 
                 * the frame jump states should check the current frame, when the current frame reaches the end of the cycle (using the frame count in the config) 
                 * the state should kick onto the next one
                 * 
                 * 
                 * to ensure the jump isn't re-triggered
                 * if (m_force.Y < 0 && !m_isJumping){}
                 */
            }

            if (m_isJumping)
            {
                //Console.WriteLine("jumping");

                //m_spriteManager.Cmd(SpriteManager.SpriteCommands.SWITCH, "player", jumpState[1]);

                //Console.WriteLine(m_jumpStates[0]);

                //m_spriteManager.Cmd(SpriteManager.SpriteCommands.TRANSITION, "player", m_jumpStates[1]);

                

                //query current animation state
                //if(m_spriteManager
                
            }


            //NOTES ON IMPLEMENTATION
            /*
             * TODO 8/2/2015
             * Due to a type in the config.cs during testing the initial method
             * of stacking the animation states in the sprite class didn't seem to work
             * so the below verbose method was setup, only then did the mistake in the config.cs
             * surface, this more "elegant" solution should be re-looked at some time
             * 
             * get the distance from the player to the planet outside/collision and start the "landing" loop earlier to allow
             * for longer/more dynamic animation
             * 
             * -------------
             * an initial design idea was to have a list of the jump states that was then iterated through
             * and rebuilt once complete - this also fell apart in favour of the above 
             */

            /*
             * checks the current animation state, if it's part of the jump loop handles moving through
             * these stages, see above notes on updating this design
             */ 
            if(!m_isJumping)
            {
                bool animLock = true;
                //check if ending the jump loop
                if (m_spriteManager.m_spriteStates[0].m_animState == m_jumpStates[1])
                {
                    m_spriteManager.Cmd(SpriteManager.SpriteCommands.SWITCH, "player", m_jumpStates[2]);
                    //Console.WriteLine("switching to last stage");                    
                }
                else if (m_spriteManager.m_spriteStates[0].m_animState == m_jumpStates[2])
                {
                    //Console.WriteLine("in the last stage");
                    //m_spriteManager.Cmd(SpriteManager.SpriteCommands.TRANSITION, "player", "idle");

                    //TODO put check if the animation has "reached the end" and if so switch don't set the 
                    //animation lock...


                    if (m_spriteManager.m_spriteStates[0].m_animState == m_jumpStates[2] &&
                        m_spriteManager.m_spriteStates[0].m_hasReachedEnd)
                    {
                        m_spriteManager.m_spriteStates[0].m_hasReachedEnd = false;
                        animLock = false;
                    }
                    //else
                    //    Console.WriteLine("animation is looping");
                }
                else
                    animLock = false;

                if(!animLock)
                {
                    if (m_force.X != 0)
                    {
                        //Console.WriteLine("move");
                        //PLAY RUN ANIMATION
                        m_spriteManager.Cmd(SpriteManager.SpriteCommands.SWITCH, "player", "run");
                    }
                    else
                    {
                        //Console.WriteLine("idle");
                        //PLAY IDLE
                        if (!m_isJumping)
                        {
                            m_spriteManager.Cmd(SpriteManager.SpriteCommands.SWITCH, "player", "idle");

                            if(lastDir)
                                m_spriteEffect = SpriteEffects.None;
                            else
                                m_spriteEffect = SpriteEffects.FlipHorizontally;

                            //rebuild the jump state array
                            //m_jumpStates.Add("start");
                            //m_jumpStates.Add("loop");
                            //m_jumpStates.Add("end");

                        }
                    }
                }
            }
        }




        public void UpdateTransVars()
        {

        }

        public void UpdatePhysics(float a_DT)
        {
            //force coming in should be final
            //m_force

            if (!m_hasCollided)
                SpaceMove(a_DT);
            else
            {
                PlanetMove(a_DT);
                //we are on the planet, disable the jump!
                m_isJumping = false;
            }

            //reset force for next update
            m_force = new Vector2(0, 0);
        }


        public void SpaceMove(float a_DT)
        {
            Vector2 gravity = new Vector2(0, 0);

            Vector2 gravityNorm = new Vector2(0, 0);


            gravity = new Vector2(0f, m_curPlanet.m_planetGravity);


            gravityNorm = gravity;

            gravityNorm.Normalize();


            m_force += gravity;


            //get heading to planet


            Vector2 planetNorm = PanetNormal();

            m_planetRot = GetAngleBetween(new Vector2(0, 1), planetNorm);

            //planetRot -= 1.57079637f;

            //apply a fake friction to slow down the agent
            Vector2 fakeFriction = m_velocity * -m_friction;

            //get the accelaration of the player
            m_accelaration = (m_force + fakeFriction) * (1.0f / m_mass);

            //update velocity for this update
            m_velocity += m_accelaration * a_DT;

            

            //set on changing of planet
            //switch upwards velocity as we are entering a new gravity well
            if (m_killUpVelocity)
            {
                if(m_velocity.Y < 0)
                    m_velocity.Y = -m_velocity.Y;                
            }
            m_killUpVelocity = false;

            //set heading
            m_heading = m_velocity;
            m_heading.Normalize();


            if (m_heading.Length() > 0)
            {
                //TODO get a rotation for matrix and update position

                Matrix matGravityRot = Matrix.Identity * Matrix.CreateRotationZ(m_planetRot);
                Matrix matGravityTrans = Matrix.Identity * Matrix.CreateTranslation(new Vector3(m_velocity.X, m_velocity.Y, 0));

                Matrix matGravityMove = matGravityTrans * matGravityRot;

                m_gravityVelocity = new Vector2(matGravityMove.Translation.X, matGravityMove.Translation.Y);

                m_gravityVelocity *= .05f;

                m_rot = m_planetRot + 1.570796327f;

                m_gravHeading = m_gravityVelocity;
                m_gravHeading.Normalize();


                //flips the texture and rotates based on current direction
                if (m_heading.X > 0)
                {
                    // Console.WriteLine("going right");

                    //set the sprite effect for going right
                    //m_spriteEffect = SpriteEffects.None;

                    m_rot -= 1.570796327f;
                }
                else
                {
                    //flip the texture
                    //m_spriteEffect = SpriteEffects.FlipHorizontally;

                    m_rot -= 1.570796327f;
                }




                //update position
                m_pos += m_gravityVelocity;
            }

        }

        public Vector2 PanetNormal()
        {
            Vector2 headingToPlanet = m_curPlanet.m_pos - m_pos;

            headingToPlanet.Normalize();

            return headingToPlanet;
        }

        public Vector2 temp(Vector2 a_pos)
        {
            Vector2 playerActual = a_pos;

            Vector2 headingToPlanet = new Vector2((350), (250)) - playerActual;

            Vector2 planetNorm = headingToPlanet;

            planetNorm.Normalize();

            return planetNorm;
        }


        public void PlanetMove(float a_DT)
        {
            Vector2 planetPos = m_curPlanet.m_pos;
            float planetRad = m_curPlanet.m_radius * m_curPlanet.m_scale;

            Vector2 p2pDir = m_pos - planetPos;
            p2pDir.Normalize();

            bool canMove = false;

            Vector2 perp = new Vector2(0,0); 
            if (m_force.X > 0)
            {
                //right                
                m_spriteEffect = SpriteEffects.None;
                perp = new Vector2(-p2pDir.Y, p2pDir.X);
                canMove = true;
                lastDir = false;
            }
            else if(m_force.X < 0)
            {
                //left                                
                m_spriteEffect = SpriteEffects.FlipHorizontally;
                perp = new Vector2(p2pDir.Y, -p2pDir.X);
                canMove = true;
                lastDir = true;
            }

            if (canMove)
            {
                //could use inbuilt matrix methods to get this
                Vector2 movedPos = m_pos + perp * a_DT * m_planetMoveSpeed;

                p2pDir = movedPos - planetPos;
                p2pDir.Normalize();
                Vector2 adjusted = planetPos + p2pDir * (planetRad + GetRadius((int)(m_width * m_scale), (int)(m_height * m_scale)));

                m_pos = adjusted;
                Vector2 dir = new Vector2(-p2pDir.Y, p2pDir.X);
                m_rot = (float)Math.Atan2((float)dir.Y, (float)dir.X);
            }
            ///matrix stuff happend later/automatically
        }

        public override void IsCollidingGravityWell(Actor a_colActor)
        {
            base.IsCollidingGravityWell(a_colActor);

            if ((Planet)a_colActor != m_curPlanet && !m_newPlanet && !m_hasCollided)
            {
                //Console.WriteLine("switching planet");

                m_game.m_score++;

                m_curPlanet = (Planet)a_colActor;

                m_newPlanet = true;

                m_killUpVelocity = true;

            }
            //Console.WriteLine("colliding with gravity! \n");

        }


        public override void IsColliding(Actor a_colActor)
        {
            base.IsColliding(a_colActor);

            
            //get intersecting length
//            float r1 = GetRadius(m_sprite.Width, m_sprite.Height);
//            float r2 = GetRadius(a_colActor.m_sprite.Width, a_colActor.m_sprite.Height);
//
//            float length = (m_pos - a_colActor.m_pos).Length();
//
//            float distance = Math.Abs(length - (r1 + r2));
//
//            Vector2 reverseHeading = new Vector2(0,0);
//
//            if (m_gravHeading.X > 0)
//                reverseHeading.X = -m_gravHeading.X;
//
//            if (m_gravHeading.X < 0)
//                reverseHeading.X = Math.Abs(m_gravHeading.X);
//
//            if (m_gravHeading.Y > 0)
//                reverseHeading.Y = -m_gravHeading.Y;
//
//            if (m_gravHeading.Y < 0)
//                reverseHeading.Y = Math.Abs(m_gravHeading.Y);
//
//            Vector2 offset = reverseHeading * distance;
//
//            m_pos += offset;
            if (!m_hasCollided && a_colActor == m_curPlanet)
            {
                //initial velocity
                Vector2 Vi = m_gravityVelocity;
                //collision normal
                Vector2 N = new Vector2(-Vi.Y, Vi.X);
                N.Normalize();

                //negative magnitude of P
                float magP = -Vector2.Dot(Vi, N);

                Vector2 P = N * magP;

                P = -P;

                Vector2 Vf = 2 * P + Vi;
                Vf = -Vf;

                m_hasCollided = true;

                m_pos += Vf;

                m_newPlanet = false;

                m_killUpVelocity = false;
                
                //m_game.updateCamera(m_curPlanet.m_pos); //update the cameras position

            }
            //Console.WriteLine(Vf);
            //Console.WriteLine(m_gravityVelocity);
        }

        public void LerpCamera(float a_dt)
        {
            Vector2 vec = (m_curPlanet.m_pos - m_game.m_cam._pos);

            //get direction to go in
            Vector2 dir = vec;
            dir.Normalize();

            dir *= 30 * a_dt;
            
            if (vec.LengthSquared() > 10)
            {
                //Console.WriteLine("lerping");
                //NOTE - disabled for submission build
                //m_game.m_cam._pos += dir;                
            }
        }


        /// <summary>
        /// function as reference, use XNA transform point where needed
        /// </summary>
        /// <param name="a_vec"></param>
        /// <param name="a_rot"></param>
        /// <returns></returns>
        public Vector2 TransformPoint(Vector2 a_vec, Matrix a_rot)
        {
            float x = a_vec.X;
            float y = a_vec.Y;

            float finX = (x * a_rot.M11) + (y * a_rot.M21) + a_rot.M31;
            float finY = (x * a_rot.M12) + (y * a_rot.M22) + a_rot.M32;


            return new Vector2(finX, finY);
        }

        public override void Draw(SpriteBatch a_sb, DRAWLAYER a_layer)
        {
            base.Draw(a_sb, a_layer);

            //DrawLine(a_sb, m_pos, m_curPlanet.m_pos);
        }
    }
}
