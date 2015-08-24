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
    /// This is the main type for your game
    /// </summary>
    public class Game1 : Game
    {
        GraphicsDeviceManager graphics;
        public SpriteBatch spriteBatch;

        public StateManager m_stateMangr = new StateManager();

        //time keeping
        public float m_deltaTime;

        public Config gameConfig;

        //actors for splash screen
        public List<Actor> m_actorListSplash = new List<Actor>();

        //actors for main game
        public List<Actor> m_actorList = new List<Actor>();

        public Camera2d m_cam = new Camera2d();

        public bool m_exit;

        public int m_screenWidth, m_screenHeight;

        public int m_score;

        public SpriteFont m_font;

        //TODO move reload key to game state
        public string hotReloadKey;       

        public Game1()
            : base()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
        }

        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {
            // TODO: Add your initialization logic here

            base.Initialize();

            m_exit = false;
            m_deltaTime = 0;            

            m_score = 0;

            m_stateMangr.Init(this);

            gameConfig = new Config();

            gameConfig.Init(this, spriteBatch, m_actorList, m_actorListSplash);

            //check start state
            if (gameConfig.m_firstState == "game")
            {
                GameStateMainGame newGame = new GameStateMainGame();
                //boot up the main game!
                m_stateMangr.Cmd("game", StateManager.cmds.PUSH, newGame);

                //centre the camera
                m_cam.Pos = new Vector2(m_screenWidth / 2, m_screenHeight / 2);
            }
            else
            {
                GameStateSplash newSplash = new GameStateSplash();
                //boot up the splash screen!
                m_stateMangr.Cmd("splash", StateManager.cmds.PUSH, newSplash);
                m_cam.Pos = new Vector2(m_screenWidth / 2, m_screenHeight / 2);
            }
        }

        public void UpdateRes(Game1 a_game, int a_width, int a_height)
        {
            graphics.PreferredBackBufferHeight = a_height;
            graphics.PreferredBackBufferWidth = a_width;

            m_screenHeight = a_height;
            m_screenWidth = a_width;

            m_cam.UpdateRes(a_width, a_height);

            graphics.ApplyChanges();
        }


        /// <summary>
        /// LoadContent will be called once per game and is the place to load
        /// all of your content.
        /// </summary>
        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);

            m_font = Content.Load<SpriteFont>("SpriteFontMain"); // Use the name of your sprite font file here instead of 'Score'.

            // TODO: use this.Content to load your game content here


        }

        /// <summary>
        /// UnloadContent will be called once per game and is the place to unload
        /// all content.
        /// </summary>
        protected override void UnloadContent()
        {
            // TODO: Unload any non ContentManager content here
        }

        /// <summary>
        /// Allows the game to run logic such as updating the world,
        /// checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {
            //update delta time
            //m_lastTime      =   m_currentTime;
            //m_currentTime   =   gameTime.ElapsedGameTime.Milliseconds;
            //m_deltaTime     =   m_currentTime - m_lastTime;
            m_deltaTime = gameTime.ElapsedGameTime.Milliseconds * 0.01f;

            if (m_exit)
                Exit();

            m_stateMangr.Update(m_deltaTime);

            base.Update(gameTime);
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            // TODO: Add your drawing code here
            // Draw the sprite.
            spriteBatch.Begin(SpriteSortMode.BackToFront, BlendState.AlphaBlend, null, null, null, null, m_cam.get_transformation(GraphicsDevice));
            //spriteBatch.Begin(SpriteSortMode.Deferred, BlendState.AlphaBlend);

            m_stateMangr.Draw(spriteBatch);
           
            spriteBatch.End();

            base.Draw(gameTime);
        }

        public void updateCamera(Vector2 a_pos)
        {
            //TODO check how far player is from centre of screen
            //if outside range lerp screen back to player
        
            //OR if player is nearing edge shift screen over
        
            //camMatrix = Matrix.CreateTranslation(new Vector3(a_pos.X, a_pos.Y, 0));

            //TODO
            /*
             * create a processing list for camera commands to allow multiple events 
             */
 
            m_cam.Pos = a_pos;
        }
    }
}
