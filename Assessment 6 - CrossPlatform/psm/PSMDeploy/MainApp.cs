using System;
using System.Collections.Generic;
using System.Linq;

namespace CrossPlatform
{
	static class Program
	{
		private static Game1 game;
		
		[STAThread]
		static void Main ()
		{
			Console.WriteLine("Loading Game");
			game = new Game1();
			game.Run();			
		}
	}
}