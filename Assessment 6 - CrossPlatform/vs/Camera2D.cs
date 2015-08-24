using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;


/*
 * 
 * 
 * based on tutorials from
 * http://www.david-amador.com/2009/10/xna-camera-2d-with-zoom-and-rotation/
 * 
 * further reading
 * http://stackoverflow.com/questions/712296/xna-2d-camera-engine-that-follows-sprite
 * 
 */

namespace CrossPlatform
{
    public class Camera2d
    {
        protected float _zoom; // Camera Zoom
        public Matrix _transform; // Matrix Transform
        public Vector2 _pos; // Camera Position
        protected float _rotation; // Camera Rotation
        protected int _width, _height; //res of width and height

        public Camera2d()
        {
            _zoom = 1.0f;
            _rotation = 0.0f;
            _pos = Vector2.Zero;
        }

        // Sets and gets zoom
        public float Zoom
        {
            get { return _zoom; }
            set { _zoom = value; if (_zoom < 0.1f) _zoom = 0.1f; } // Negative zoom will flip image
        }

        public float Rotation
        {
            get { return _rotation; }
            set { _rotation = value; }
        }

        // Auxiliary function to move the camera
        public void Move(Vector2 amount)
        {
            _pos += amount;
        }

        // Get set position
        public Vector2 Pos
        {
            get { return _pos; }
            set { _pos = value; }
        }


        public void UpdateRes(int a_width, int a_height)
        {
            _width = a_width;
            _height = a_height;
        }

        /// <summary>
        /// calculates all the transforms
        /// </summary>
        /// <param name="graphicsDevice"></param>
        /// <returns></returns>
        public Matrix get_transformation(GraphicsDevice graphicsDevice)
        {
            _transform =       // Thanks to o KB o for this solution
              Matrix.CreateTranslation(new Vector3(-_pos.X, -_pos.Y, 0)) *
                                         Matrix.CreateRotationZ(Rotation) *
                                         Matrix.CreateScale(new Vector3(Zoom, Zoom, 1)) *
                                         Matrix.CreateTranslation(new Vector3(_width * 0.5f, _height * 0.5f, 0));
            return _transform;
        }
    }
}
