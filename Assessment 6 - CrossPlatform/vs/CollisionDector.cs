using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace CrossPlatform
{
    class CollisionDector
    {
        //check collisions with main planet collision
        public void CheckCollisions(List<Actor> a_list)
        {
            for (int i = 0; i < a_list.Count; i++)
            {
                for (int x = 0; x < a_list.Count; x++)
                {
                    if(a_list[i] != a_list[x])
                    {
                        if (CheckPlanetCollision(a_list[i], a_list[x]))
                        {
                           // Console.WriteLine("collision!!!");

                            a_list[i].IsColliding(a_list[x]);   //notify actor one
                            //actor two will be notified on loop
                            //a_list[x].IsColliding(a_list[i]);   //notify actor two

                        }
                        //else
                        //    Console.WriteLine("no collision");
                    }
                }                
            }
        }

        public void CheckGravityCollisions(List<Actor> a_list)
        {
            for (int i = 0; i < a_list.Count; i++)
            {
                for (int x = 0; x < a_list.Count; x++)
                {
                    if (a_list[i] != a_list[x])
                    {
                        //only check if one is a planet and the other a player!
                        if (a_list[i].m_classID == CLASSID.CLASSID_PLAYER || a_list[x].m_classID == CLASSID.CLASSID_PLAYER)
                        {
                            if (CheckGravityCollision(a_list[i], a_list[x]))
                            {

                                a_list[i].IsCollidingGravityWell(a_list[x]);
                            }
                        }
                    }
                }
            }
        }

        private bool CheckPlanetCollision(Actor a_1,Actor a_2)
        {
            float a1Rad = a_1.m_radius * a_1.m_scale;
            float a2Rad = a_2.m_radius * a_2.m_scale;

            float distance = (a_1.m_pos - a_2.m_pos).Length();

            if (distance < a1Rad + a2Rad)
                return true;
            else
                return false;
        }


        /// <summary>
        /// function assumes one of the passed actors is the player
        /// </summary>
        /// <param name="a_1"></param>
        /// <param name="a_2"></param>
        /// <returns>true on collision with gravity well</returns>
        private bool CheckGravityCollision(Actor a_1, Actor a_2)
        {
            float a1Rad = 0;
            float a2Rad = 0;

            //            Vector2 pos1 = new Vector2(0,0);
            //            Vector2 pos2 = new Vector2(0,0);
            if (a_1.m_classID != CLASSID.CLASSID_BACKGROUND && a_2.m_classID != CLASSID.CLASSID_BACKGROUND)
            {
                //check if actor one is the player or planet
                if (a_1.m_classID == CLASSID.CLASSID_PLAYER)
                {
                    a1Rad = a_1.m_radius * a_1.m_scale;
                    //                pos1 = a_1.m_pos;
                }
                else
                {
                    Planet planet = (Planet)a_1;
                    a1Rad = planet.m_effectiveRadius * planet.m_scale;
                }

                //check if player one or two is the player
                if (a_2.m_classID == CLASSID.CLASSID_PLAYER)
                {
                    a2Rad = a_2.m_radius * a_2.m_scale;
                }
                else
                {
                    Planet planet = (Planet)a_2;
                    a2Rad = planet.m_effectiveRadius * planet.m_scale;
                }

                float distance = (a_1.m_pos - a_2.m_pos).Length();

                if (distance < a1Rad + a2Rad)
                    return true;
                else
                    return false;
            }
            return false;
        }

        public float GetRadius(float a_width, float a_height)
        {
            return (float)Math.Sqrt(((a_width * a_width) / 4) + ((a_height * a_height) / 4));
        }
    }
}
