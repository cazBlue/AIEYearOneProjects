using System;
using System.Collections.Generic;
using System.Linq;
//using System.Text;
//using System.Xml;
using System.Xml.Linq;
//using System.Xml.Linq;
//using System.Xml.Linq.XElement;
//using System.object;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;


namespace CrossPlatform
{

//    public struct Planet
//    {
//        public float dt;
//
//        public Foo(float _dt)
//        {
//            dt = _dt;
//        }
//    }

    

    public class Config
    {
        //================DO NOT EDIT
        public float m_globalScale;

        public string m_firstState;

        //XmlDocument xmlDoc = new XmlDocument(); // Create an XML document object

        //XElement


        //TODO 
        //create global settings for platform to build to - psm, andriod etc

        //==================================        

        public void Init(Game1 a_game, SpriteBatch a_sb, List<Actor> a_list, List<Actor> a_splashList)
        {
            //GOBAL SETTINGS
            m_globalScale = 1;            

            //global scale will scale everything in the game


            //snippets from http://www.dotnetcurry.com/showarticle.aspx?ID=564
            //load xml file
            XElement xelement = XElement.Load("content/config.xml");

//            IEnumerable<XElement> planets = xelement.Elements();

//            var xmlPlanets = from planets in xelement.Elements("planets") select planets;


            Console.WriteLine("=============== Loading XML data ===================");

            //CONFIG
            foreach (var xConfig in xelement.Descendants("config"))
            {
                a_game.UpdateRes(a_game, Convert.ToInt32(xConfig.Element("screenX").Value), Convert.ToInt32(xConfig.Element("screenY").Value));

                Actor bg = new Actor();

                bg.Init(a_game, a_sb, CLASSID.CLASSID_BACKGROUND);

                //bg.m_spritesBackground.Add(a_game.Content.Load<Texture2D>(xConfig.Element("backgroundSprite").Value));
                List<Texture2D> texture = new List<Texture2D>();
                texture.Add(a_game.Content.Load<Texture2D>(xConfig.Element("backgroundSprite").Value));

                bg.m_spriteManager.Cmd(SpriteManager.SpriteCommands.PUSH, "background", texture, Convert.ToInt32(xConfig.Element("frameCount").Value),
                    Convert.ToInt32(xConfig.Element("frameWidth").Value), 0, Convert.ToInt32(xConfig.Element("frameHeight").Value), Convert.ToSingle((xConfig.Element("animSpeed").Value)));


                bg.m_pos = new Vector2(Convert.ToInt32(xConfig.Element("bgX").Value), Convert.ToInt32(xConfig.Element("bgY").Value));

                bg.m_classID = CLASSID.CLASSID_BACKGROUND;

                a_list.Add(bg);

                m_firstState = xConfig.Element("firstGameScreen").Value;
                if (m_firstState == "splash")
                {
                    foreach (var xSprite in xConfig.Descendants("splashSprite"))
                    {
                        Actor splash = new Actor();

                        splash.Init(a_game, a_sb, CLASSID.CLASSID_BACKGROUND);

                        splash.m_pos = new Vector2(0, 0);

                        List<Texture2D> Splashtexture = new List<Texture2D>();
                        Splashtexture.Add(a_game.Content.Load<Texture2D>(xSprite.Value));

                        splash.m_spriteManager.Cmd(SpriteManager.SpriteCommands.PUSH, "splash", Splashtexture, Convert.ToInt32(xConfig.Element("frameCount").Value),
                                            Convert.ToInt32(xConfig.Element("frameWidth").Value), 0, Convert.ToInt32(xConfig.Element("frameHeight").Value), Convert.ToSingle((xConfig.Element("animSpeed").Value)));


                        splash.m_classID = CLASSID.CLASSID_BACKGROUND;

                        a_splashList.Add(splash);
                    }
                }
            }

            //================= building player
            Player player = new Player();

            foreach (var xPlayer in xelement.Descendants("player"))
            {
                Console.WriteLine(xPlayer);

                //================================================================================
                //================ PLAYER CONFIG
                //================================================================================
                //There can only be only be one player
  

                player.Init(a_game, a_sb, CLASSID.CLASSID_PLAYER);  //don't edit this line

                //set the player x and y start position
                //0,0 is top left
                //This is the actual starting position from the centre of the player
                player.m_pos = new Vector2(Convert.ToInt32(xPlayer.Element("posX").Value), Convert.ToInt32(xPlayer.Element("posY").Value));

                float test = Convert.ToSingle(xPlayer.Element("scale").Value);

                //set the players local scale
                player.m_scale = test;

                //put an f at the end
                player.m_mass = Convert.ToSingle(xPlayer.Element("mass").Value);
                player.m_friction = Convert.ToSingle(xPlayer.Element("friction").Value);

                //sprite texture to load
//                player.m_sprite = a_game.Content.Load<Texture2D>(xPlayer.Element("sprite").Value);

                            //=================== building player srpites
                foreach (var xSprite in xPlayer.Descendants("sprite"))
                {
                    //player.m_spritesMain.Add(a_game.Content.Load<Texture2D>(xSprite.Value));
                    List<Texture2D> texture = new List<Texture2D>();
                    texture.Add(a_game.Content.Load<Texture2D>(xSprite.Value));
                    player.m_spriteManager.Cmd(SpriteManager.SpriteCommands.PUSH, "player", texture, Convert.ToInt32(xPlayer.Element("idleCount").Value),
                        Convert.ToInt32(xPlayer.Element("frameWidth").Value), Convert.ToInt32(xPlayer.Element("idleStartCol").Value), Convert.ToInt32(xPlayer.Element("idleStartRow").Value),
                        Convert.ToInt32(xPlayer.Element("frameHeight").Value), Convert.ToSingle(xPlayer.Element("animSpeed").Value), 
                        "idle");
                    player.m_spriteManager.Cmd(SpriteManager.SpriteCommands.PUSH, "player", texture, Convert.ToInt32(xPlayer.Element("runCount").Value),
                        Convert.ToInt32(xPlayer.Element("frameWidth").Value), Convert.ToInt32(xPlayer.Element("runStartCol").Value), Convert.ToInt32(xPlayer.Element("runStartRow").Value), 
                        Convert.ToInt32(xPlayer.Element("frameHeight").Value), Convert.ToSingle(xPlayer.Element("animSpeed").Value), 
                        "run");



                    //Jumping states
                    //jump start
                    player.m_spriteManager.Cmd(SpriteManager.SpriteCommands.PUSH, "player", texture, Convert.ToInt32(xPlayer.Element("jumpStartCount").Value),
                        Convert.ToInt32(xPlayer.Element("frameWidth").Value), Convert.ToInt32(xPlayer.Element("jumpStartCol").Value), Convert.ToInt32(xPlayer.Element("jumpStartRow").Value),
                        Convert.ToInt32(xPlayer.Element("frameHeight").Value), Convert.ToSingle(xPlayer.Element("animSpeed").Value),
                        "start");
                    //jump loop
                    player.m_spriteManager.Cmd(SpriteManager.SpriteCommands.PUSH, "player", texture, Convert.ToInt32(xPlayer.Element("jumpLoopCount").Value),
                        Convert.ToInt32(xPlayer.Element("frameWidth").Value), Convert.ToInt32(xPlayer.Element("jumpLoopCol").Value), Convert.ToInt32(xPlayer.Element("jumpLoopRow").Value),
                        Convert.ToInt32(xPlayer.Element("frameHeight").Value), Convert.ToSingle(xPlayer.Element("animSpeed").Value),
                        "loop");
                    //jump end
                    player.m_spriteManager.Cmd(SpriteManager.SpriteCommands.PUSH, "player", texture, Convert.ToInt32(xPlayer.Element("jumpEndCount").Value),
                        Convert.ToInt32(xPlayer.Element("frameWidth").Value), Convert.ToInt32(xPlayer.Element("jumpEndCol").Value), Convert.ToInt32(xPlayer.Element("jumpEndRow").Value),
                        Convert.ToInt32(xPlayer.Element("frameHeight").Value), Convert.ToSingle(xPlayer.Element("animSpeed").Value),
                        "end");



                    player.m_spriteManager.Cmd(SpriteManager.SpriteCommands.SWITCH,"player", "idle");   //set the initial state to idle
                }


                //center of the image, adjust as needed
                player.m_centre = new Vector2(Convert.ToSingle((xPlayer.Element("centreX").Value)), Convert.ToSingle(xPlayer.Element("centreY").Value));
                

                player.m_spaceMoveSpeed = Convert.ToInt32(xPlayer.Element("spaceMoveSpeed").Value);
                player.m_planetMoveSpeed = Convert.ToInt32(xPlayer.Element("planetMoveSpeed").Value);
                player.m_jumpSpeed = Convert.ToInt32(xPlayer.Element("jumpSpeed").Value);

                //draw the collision circle around the player
                player.m_drawCollision = Convert.ToBoolean(xPlayer.Element("drawCollision").Value);

                player.m_width = Convert.ToSingle(xPlayer.Element("width").Value);
                player.m_height = Convert.ToSingle(xPlayer.Element("height").Value);

                //set the radius of the player
                //default is to get the radius based on texture
                player.m_radius = player.GetRadius(player.m_width, player.m_height);

                //add the player to the main game loop
                a_list.Add(player);
                //=====================================
            }



            //=================== building planets
            foreach (var xPlanet in xelement.Descendants("planet"))
            {
                Console.WriteLine(xPlanet);

                //start creating planet one       
                Planet planet1 = new Planet();

                //TODO - split planet based on type
                planet1.Init(a_game, a_sb, CLASSID.CLASSID_PLANETSPHERE);

                //position
                planet1.m_pos = new Vector2( Convert.ToInt32( xPlanet.Element("posX").Value),  Convert.ToInt32( xPlanet.Element("posY").Value));
                planet1.m_centre = new Vector2(Convert.ToInt32(xPlanet.Element("centreX").Value), Convert.ToInt32(xPlanet.Element("centreY").Value));
                planet1.m_planetGravity = Convert.ToInt32(xPlanet.Element("gravity").Value);

                planet1.m_scale = Convert.ToSingle(xPlanet.Element("scale").Value);

                //radius of the planet for collisions
                planet1.m_radius = Convert.ToSingle(xPlanet.Element("collisionRadius").Value);
                //radius of the planet for detecting gravity effect on the player
                planet1.m_effectiveRadius = Convert.ToSingle(xPlanet.Element("effectiveRadius").Value);
                //draw the effective radius
                planet1.m_drawEfftiveRadius = Convert.ToBoolean(xPlanet.Element("drawCollision").Value);

                //draw the collision circle (good way to see radius)
                planet1.m_drawCollision = Convert.ToBoolean(xPlanet.Element("drawEffective").Value);

                //laod planet sprite
                //planet1.m_sprite = a_game.Content.Load<Texture2D>(xPlanet.Element("sprite").Value);
                
                //add main sprite
                foreach (var xSprite in xPlanet.Descendants("planetSprite"))
                {
                    //Console.WriteLine(xSprite.Value);
                    //planet1.m_spritesMain.Add(a_game.Content.Load<Texture2D>(xSprite.Value));
                    List<Texture2D> texture = new List<Texture2D>();
                    texture.Add(a_game.Content.Load<Texture2D>(xSprite.Value));
                    planet1.m_spriteManager.Cmd(SpriteManager.SpriteCommands.PUSH, "main", texture, Convert.ToInt32(xPlanet.Element("frameCount").Value), Convert.ToInt32(xPlanet.Element("frameWidth").Value),
                        0 , Convert.ToInt32(xPlanet.Element("frameHeight").Value), Convert.ToSingle(xPlanet.Element("animSpeed").Value));
                }

                //add behind sprite elements
                foreach (var xSprite in xPlanet.Descendants("behindSprite"))
                {
                    //Console.WriteLine(xSprite.Value);
                    //planet1.m_spritesBehind.Add(a_game.Content.Load<Texture2D>(xSprite.Value));
                    List<Texture2D> texture = new List<Texture2D>();
                    texture.Add(a_game.Content.Load<Texture2D>(xSprite.Value));
                    planet1.m_spriteManager.Cmd(SpriteManager.SpriteCommands.PUSH, "behind", texture, Convert.ToInt32(xPlanet.Element("frameCount").Value), Convert.ToInt32(xPlanet.Element("frameWidth").Value),
                        0 , Convert.ToInt32(xPlanet.Element("frameHeight").Value), Convert.ToSingle(xPlanet.Element("animSpeed").Value));

                }

                //add in front sprite elements
                foreach (var xSprite in xPlanet.Descendants("infrontSprite"))
                {
                    //Console.WriteLine(xSprite.Value);
                    //planet1.m_spritesInfront.Add(a_game.Content.Load<Texture2D>(xSprite.Value));
                    List<Texture2D> texture = new List<Texture2D>();
                    texture.Add(a_game.Content.Load<Texture2D>(xSprite.Value));
                    planet1.m_spriteManager.Cmd(SpriteManager.SpriteCommands.PUSH, "front", texture, Convert.ToInt32(xPlanet.Element("frameCount").Value), Convert.ToInt32(xPlanet.Element("frameWidth").Value), 
                        0, Convert.ToInt32(xPlanet.Element("frameHeight").Value), Convert.ToSingle(xPlanet.Element("animSpeed").Value));
                }                

                //add the planet to the list
                a_list.Add(planet1);


                //=================================================
                //set the first planet the player is attracted to, this can be any planet but it must be set 
                //TODO currently set to last planet added, change to be set via config
                player.m_curPlanet = planet1;
            }

            Console.WriteLine("============== End xml data ==============");

        }

        /// <summary>
        /// clear the current game variables and rebuild the game
        /// </summary>
        /// <param name="a_game"></param>
        /// <param name="a_sb"></param>
        /// <param name="a_list"></param>
        public void hotReload(Game1 a_game, SpriteBatch a_sb, List<Actor> a_list,  List<Actor> a_splashList)
        {
            a_list.Clear();
            a_splashList.Clear();

            Init(a_game, a_sb, a_list, a_splashList);

            Console.WriteLine("++++++++++++++++++++++++++ Hot reloading done +++++++++++++++++++++++++++++");
        }


    }
}