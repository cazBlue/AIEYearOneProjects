using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework.Graphics;

namespace CrossPlatform
{
    /// <summary>
    /// state at the top of the stack if considered active, all other states 
    /// are considered "paused" and do not draw or update
    /// </summary>
    public class StateManager
    {
        public List<GameState> m_states = new List<GameState>();

        private Game1 m_game;

        private List<StateCmd> m_cmds = new List<StateCmd>();   //list of commands to be executed during the next update

        public void Init(Game1 a_game)
        {
            m_game = a_game;            
        }


        /// <summary>
        /// commands to be processed during each update cycle
        /// </summary>
        struct StateCmd
        {
            public string m_name;
            public cmds m_cmd;
            public GameState m_state;

            public StateCmd(string a_name, cmds a_cmd, GameState a_state)
            {
                m_name = a_name;
                m_cmd = a_cmd;
                m_state = a_state;
            }
        }

        public enum cmds
        {
            PUSH,
            POP
        }

        public void Cmd(string a_name, cmds a_cmd, GameState a_state)
        {
            StateCmd newCmd = new StateCmd(a_name, a_cmd, a_state);
            m_cmds.Add(newCmd);
        }

        private void ProcessCmds()
        {
            foreach (StateCmd stateCmd in m_cmds)
            {
                switch (stateCmd.m_cmd)
                {
                    case cmds.PUSH:
                        PushState(stateCmd.m_state);
                        break;
                    case cmds.POP:
                        PopState(stateCmd.m_state);
                        break;
                }
            }

            m_cmds.Clear();
        }

        private void PopState(GameState a_state)
        {
            m_states.Remove(a_state);
        }

        private void PushState(GameState a_state)
        {
            a_state.Init(m_game);
            m_states.Add(a_state);
        }

        public void Update(float a_dt)
        {
            if(m_states.Count > 0)
                m_states.Last().Update(a_dt);

            ProcessCmds();
        }

        public void Draw(SpriteBatch a_sb)
        {
            m_states.Last().Draw(a_sb);
        }
    }
}
