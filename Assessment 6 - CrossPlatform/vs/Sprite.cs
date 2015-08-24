using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;

/*
 * 
 *  takes sprite control from actor 
 *  handle sprite animation updating
 *  must know the following:
 *      size of image
 *      size of frames
 *      frame count
 * 
 * can handle different animation states
 * 
 * TODO
 *      add frame rate option to config
 */

namespace CrossPlatform
{

    public class SpriteState
    {
        /*the name is the over-arching "actor" that owns this sprite, for example "player"
         * the player than has sub states, for example
         * "player""idle"
         * "player""run"
         * "player"jump"
         * */
        public string m_name;
        public string m_animState;        

        public Actor m_owningActor;

        public List<Texture2D> m_spriteList = new List<Texture2D>();

        public int m_SpriteWidth, m_SpriteHeight, m_frameCount, m_currentFrame, m_frameColStart, m_frameRowStart;

        public int m_framesPerRow;

        private Vector2 srcUV;

        private float m_timeCounter, m_animSpeed;

        public bool m_hasReachedEnd;

        public void Init(Actor a_actor, string a_name, int a_fHeight, int a_fWidth, int a_fCount, int a_fStart, float a_animSpeed, List<Texture2D> a_textures)
        {
            //constructor            
            this.m_owningActor = a_actor;            
            this.m_SpriteWidth = a_fWidth;
            this.m_SpriteHeight = a_fHeight;
            this.m_frameCount = a_fCount;
            this.m_name = a_name;
            this.m_spriteList = a_textures;
            this.srcUV = new Vector2(0, 0);
            this.m_framesPerRow = a_textures[0].Width / a_fWidth;
            this.m_timeCounter = 0.0f;
            this.m_currentFrame = 0;
            this.m_animSpeed = a_animSpeed;
            this.m_frameColStart = a_fStart;
            this.m_frameRowStart = 0;
            this.m_animState = null;
            this.m_hasReachedEnd = false;
        }

        public void Init(Actor a_actor, string a_name, int a_fHeight, int a_fWidth, int a_fCount, int a_fColStart, int a_fRowStart, float a_animSpeed, List<Texture2D> a_textures, string a_state)
        {
            //constructor            
            this.m_owningActor = a_actor;
            this.m_SpriteWidth = a_fWidth;
            this.m_SpriteHeight = a_fHeight;
            this.m_frameCount = a_fCount;
            this.m_name = a_name;
            this.m_spriteList = a_textures;
            this.srcUV = new Vector2(0, 0);
            this.m_framesPerRow = a_textures[0].Width / a_fWidth;
            this.m_timeCounter = 0.0f;
            this.m_currentFrame = 0;
            this.m_animSpeed = a_animSpeed;
            this.m_frameColStart = a_fColStart;
            this.m_frameRowStart = a_fRowStart;
            this.m_animState = a_state;
            this.m_hasReachedEnd = false;
        }

        public void Update(float a_dt)
        {
            //Console.WriteLine("updating sprite");

            if (m_frameCount > 0)
            {

                //updates at 24fps 1/24 = .04
                if (m_timeCounter >= m_animSpeed)
                {
                    m_currentFrame++;
                    m_timeCounter = 0;
                    
                    if (m_currentFrame > (m_frameCount) - 1)
                    {
                        m_currentFrame = 0;
                        m_hasReachedEnd = true;
                    }
                }

                int row = m_currentFrame / m_framesPerRow + m_frameRowStart;
                int col = m_currentFrame % m_framesPerRow;
                //Console.WriteLine("row: " + row);
                //Console.WriteLine("col: " + col);
                //Console.WriteLine(m_currentFrame);


                srcUV = new Vector2(col * m_SpriteWidth, row * m_SpriteHeight);

                m_timeCounter += a_dt;
            }
        }

        public void Draw(SpriteBatch a_sb, float a_depth)
        {                        
            //draw all sprites attached to this actor
            foreach (var sprite in m_spriteList)
            {
                //source position within texture sheet
                Rectangle srcRect = new Rectangle();
                srcRect.X = (int)srcUV.X;
                srcRect.Y = (int)srcUV.Y;
                srcRect.Width = (int)(m_SpriteWidth);
                srcRect.Height = (int)(m_SpriteHeight);

                //final position and dimensions on screen
                Rectangle dstRect = new Rectangle();
                dstRect.X = (int)m_owningActor.m_pos.X;
                dstRect.Y = (int)m_owningActor.m_pos.Y;
                dstRect.Width = (int)(m_SpriteWidth * m_owningActor.m_scale); ;
                dstRect.Height = (int)(m_SpriteHeight * m_owningActor.m_scale); ;
                
                a_sb.Draw(sprite,
                    dstRect,
                    srcRect,
                    Color.White,
                    m_owningActor.m_rot,
                    m_owningActor.m_centre,
                    m_owningActor.m_spriteEffect,
                    a_depth);


//                a_sb.Draw(sprite,
//                    new Rectangle((int)m_owningActor.m_pos.X, (int)m_owningActor.m_pos.Y, (int)(sprite.Width * m_owningActor.m_scale), (int)(sprite.Height * m_owningActor.m_scale)),
//                    null,
//                    Color.White,
//                    m_owningActor.m_rot,
//                    m_owningActor.m_centre,
//                    m_owningActor.m_spriteEffect,
//                    a_depth);
            }

        }

    }

    public class SpriteManager
    {
        //ptr to the actor this sprite is attatched to
        protected Actor m_actor;

        protected SpriteBatch m_sb;

        
//        public List<Texture2D> m_spritesMain = new List<Texture2D>();
//        public List<Texture2D> m_spritesInfront = new List<Texture2D>();
//        public List<Texture2D> m_spritesBackground = new List<Texture2D>();

        public List<SpriteState> m_spriteStates = new List<SpriteState>();
        private List<SpriteState> m_spriteStatesNotActive = new List<SpriteState>(); //list of inactive states for switching
        public List<SpriteEvents> m_events = new List<SpriteEvents>();
        public List<SpriteEvents> m_secondryEvents = new List<SpriteEvents>();        


        public enum SpriteCommands
        {
            SWITCH,
            PUSH,
            POP,
            TRANSITION
        };

        public struct SpriteEvents
        {
            public string m_name, m_state;
            public SpriteCommands m_cmd;
            public int m_frameCount, m_frameWidth, m_frameHeight, m_frameColStart, m_frameRowStart;
            public List<Texture2D> m_textures;
            public float m_animSpeed;

            /// <summary>
            /// main event constructor for adding sprites to the actors manager
            /// </summary>
            public SpriteEvents(SpriteCommands a_cmd, string a_name, int a_frameCount, int a_frameColStart,
                int a_frameWidth, int a_frameHeight, float a_animSpeed, List<Texture2D> a_textures)
            {
                this.m_name = a_name;                
                this.m_cmd = a_cmd;
                this.m_state = null;
                this.m_frameCount = a_frameCount;
                this.m_frameHeight = a_frameHeight;
                this.m_frameWidth = a_frameWidth;
                this.m_textures = a_textures;
                this.m_animSpeed = a_animSpeed;
                this.m_frameColStart = a_frameColStart;
                this.m_frameRowStart = 0;
            }

            public SpriteEvents(SpriteCommands a_cmd, string a_name, int a_frameCount, int a_frameColStart, int a_frameRowStart,
                int a_frameWidth, int a_frameHeight, float a_animSpeed, List<Texture2D> a_textures, string a_state)
            {
                this.m_name = a_name;
                this.m_cmd = a_cmd;
                this.m_state = a_state;

                this.m_frameCount = a_frameCount;
                this.m_frameHeight = a_frameHeight;
                this.m_frameWidth = a_frameWidth;
                this.m_textures = a_textures;
                this.m_animSpeed = a_animSpeed;
                this.m_frameColStart = a_frameColStart;
                this.m_frameRowStart = a_frameRowStart;
            }

            /// <summary>
            /// overload event for switching active sprite state on the current actor
            /// </summary>
            public SpriteEvents(SpriteCommands a_cmd, string a_name, string a_state)
            {
                this.m_name = a_name;
                this.m_cmd = a_cmd;
                this.m_state = a_state;

                this.m_frameCount = 0;
                this.m_frameHeight = 0;
                this.m_frameWidth = 0;
                this.m_textures = null;
                this.m_animSpeed = 0;
                this.m_frameColStart = 0;
                this.m_frameRowStart = 0;
            }           
        };

        /// <summary>
        /// main command function for adding sprites to the manager
        /// </summary>
        /// <param name="a_cmd"></param>
        /// <param name="a_name"></param>
        /// <param name="a_textures"></param>
        /// <param name="a_frameCount"></param>
        /// <param name="a_frameWidth"></param>
        /// <param name="a_frameStart"></param>
        /// <param name="a_frameHeight"></param>
        /// <param name="a_animSpeed"></param>
        public void Cmd(SpriteCommands a_cmd, string a_name, List<Texture2D> a_textures,
            int a_frameCount, int a_frameWidth, int a_frameStart, int a_frameHeight, float a_animSpeed)
        {
            SpriteEvents newEvent = new SpriteEvents(a_cmd, a_name, a_frameCount, a_frameStart ,  
                a_frameWidth , a_frameHeight, a_animSpeed, a_textures);

            //add event to the command stack, not active until processed
            m_events.Add(newEvent);
        }

        public void Cmd(SpriteCommands a_cmd, string a_name, List<Texture2D> a_textures,
            int a_frameCount, int a_frameWidth, int a_frameColStart, int a_frameRowStart, int a_frameHeight, float a_animSpeed, string a_state)
        {
            SpriteEvents newEvent = new SpriteEvents(a_cmd, a_name, a_frameCount, a_frameColStart, a_frameRowStart,
                a_frameWidth, a_frameHeight, a_animSpeed, a_textures, a_state);

            //add event to the command stack, not active until processed
            m_events.Add(newEvent);
        }

        /// <summary>
        /// overloaded cmd - used for switching states on an actor
        /// </summary>
        /// <param name="a_cmd"></param>
        /// <param name="a_name"></param>
        /// <param name="a_state"></param>
        public void Cmd(SpriteCommands a_cmd, string a_name, string a_state)
        {
            SpriteEvents newEvent = new SpriteEvents(a_cmd, a_name, a_state);           

            //add event to the command stack, not active until processed
            m_events.Add(newEvent);
        }

        public void Init(Actor a_owningActor)
        {
            m_actor = a_owningActor;
            
        }

        public void Update(float a_dt)
        {
            //UPDATE STATES
            //m_spriteStates.Update();
            foreach (SpriteState sState in m_spriteStates)
            {
                sState.Update(a_dt);
            }
            
            //PROCESS COMMANDS
            ProcessCmds();

        }

        public void Draw(SpriteBatch a_sb, DRAWLAYER a_layer)
        {
            //draw the desired state based on the draw order set in game1.cs
            //states are selected by name!
            switch (a_layer)
            {
                case DRAWLAYER.DRAWLAYER_BACKGROUND:
                    foreach(SpriteState spriteState in m_spriteStates)
                    {
                        if (spriteState.m_name == "background")
                        {                            
                            spriteState.Draw(a_sb, 1.0f);
                        }
                    }
                    break;
                case DRAWLAYER.DRAWLAYER_BEHIND:
                    foreach (SpriteState spriteState in m_spriteStates)
                    {
                        if (spriteState.m_name == "behind")
                        {                           
                            spriteState.Draw(a_sb, 0.8f);
                        }
                    }
                    break;
                case DRAWLAYER.DRAWLAYER_MAIN:
                    foreach (SpriteState spriteState in m_spriteStates)
                    {
                        if (spriteState.m_name == "main")
                        {                            
                            spriteState.Draw(a_sb, 0.05f);
                        }
                    }
                    break;
                case DRAWLAYER.DRAWLAYER_PLAYER:
                    foreach (SpriteState spriteState in m_spriteStates)
                    {
                        if (spriteState.m_name == "player")
                        {
                            spriteState.Draw(a_sb, 0.02f);
                        }
                    }
                    break;
                case DRAWLAYER.DRAWLAYER_INFRONT:
                    foreach (SpriteState spriteState in m_spriteStates)
                    {
                        if (spriteState.m_name == "front")
                        {                            
                            spriteState.Draw(a_sb, 0.0f);
                        }
                    }
                    break;
                case DRAWLAYER.DRAWLAYER_OTHER:
                    foreach (SpriteState spriteState in m_spriteStates)
                    {
                        spriteState.Draw(a_sb, 0.0f);
                        
                    }
                    break;
                default:
                    Console.WriteLine("error finding draw layer");
                    break;
            }


        }

        private void ProcessCmds()
        {
            foreach (SpriteEvents newEvent in m_events)
            {
               // Console.WriteLine(newEvent.m_name);
                switch (newEvent.m_cmd)
                {
                    case SpriteCommands.PUSH:
                        AddSpriteState(newEvent);
                        break;
                    case SpriteCommands.SWITCH:
                        SwitchSpriteState( newEvent.m_name, newEvent.m_state);
                        break;
                    case SpriteCommands.TRANSITION:
                        TransitionSpriteStates(newEvent.m_name, newEvent.m_state);
                        break;
                }
            }

            //clear the current process list
            m_events.Clear();
            //add any secondry events that are needed
            m_events.AddRange(m_secondryEvents);
            //clear the secondery events
            m_secondryEvents.Clear();
        }

        //TODO currently hard coded for switching player states
        private void SwitchSpriteState(string a_name, string a_state)
        {

            m_secondryEvents.Clear();

            //retire current player states to the inactive list, can be used to clean out 
            //the state list when building the initial game - this method is verbose and assumes there may be to many states active            
            for (int i = m_spriteStates.Count - 1; i >= 0; i--) //traverse in reverse for element removal         
            {
                if (m_spriteStates[i].m_name == "player")
                {
                    m_spriteStatesNotActive.Add(m_spriteStates[i]);
                    m_spriteStates.RemoveAt(i);
                }
            }

            //add the desired player state to the list
            for(int i = m_spriteStatesNotActive.Count - 1; i >= 0; i--)
            {
                if (m_spriteStatesNotActive[i].m_name == "player" && m_spriteStatesNotActive[i].m_animState == a_state)
                {
                    m_spriteStates.Add(m_spriteStatesNotActive[i]);
                    m_spriteStatesNotActive.RemoveAt(i);
                }
            }
        }


        /// <summary>
        /// if the transition state has had it's end frame reached it calls the "switch"
        /// else the transition adds the command back onto the command stack to check again
        /// next frame
        /// </summary>
        /// <param name="a_name">name of sprite</param>
        /// <param name="a_state">name of state</param>
        private void TransitionSpriteStates(string a_name, string a_state)
        {
            bool endFrameFound = false;

            for (int i = m_spriteStates.Count - 1; i >= 0; i--) //traverse in reverse for element removal         
            {                
                if(m_spriteStates[i].m_currentFrame == m_spriteStates[i].m_frameCount - 2)//runs one loop extra, end on last frame
                {
                    //Console.WriteLine("found the end frame for state: " + 
                    //    m_spriteStates[i].m_animState + " switching state to: " + a_state);
                    
                    SpriteEvents eventToAdd = new SpriteEvents(SpriteManager.SpriteCommands.SWITCH, a_name, a_state);
                    m_secondryEvents.Add(eventToAdd);

                    endFrameFound = true;
                }
            }



            if(!endFrameFound)
            {
                //if the end frame is not found add the command to the secondry process for
                //the next frame
                SpriteEvents eventToAdd = new SpriteEvents(SpriteManager.SpriteCommands.TRANSITION, a_name, a_state);
                m_secondryEvents.Add(eventToAdd);
                //Console.WriteLine("checking again");
            }

        }

        private void AddSpriteState(SpriteEvents a_event)
        {
            SpriteState state = new SpriteState();
            if(a_event.m_state != null)
                state.Init(m_actor, a_event.m_name, a_event.m_frameHeight, a_event.m_frameWidth, a_event.m_frameCount, a_event.m_frameColStart, a_event.m_frameRowStart , a_event.m_animSpeed, a_event.m_textures, a_event.m_state);
            else
                state.Init(m_actor, a_event.m_name, a_event.m_frameHeight, a_event.m_frameWidth, a_event.m_frameCount, a_event.m_frameColStart, a_event.m_animSpeed, a_event.m_textures);            

            m_spriteStates.Add(state);
        }


    }
}
