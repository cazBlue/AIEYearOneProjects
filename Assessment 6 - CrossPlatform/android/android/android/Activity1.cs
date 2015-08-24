using Android.App;
using Android.Content.PM;
using Android.OS;
using Android.Views;

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CrossPlatform
{
    [Activity(Label = "android"
        , MainLauncher = true
        , Icon = "@drawable/icon"
        , Theme = "@style/Theme.Splash"
        , AlwaysRetainTaskState = true
        , LaunchMode = Android.Content.PM.LaunchMode.SingleInstance
        , ScreenOrientation = ScreenOrientation.SensorLandscape
        , ConfigurationChanges = ConfigChanges.Orientation | ConfigChanges.Keyboard | ConfigChanges.KeyboardHidden | ConfigChanges.ScreenSize)]
    public class Activity1 : Microsoft.Xna.Framework.AndroidGameActivity
    {
        protected override void OnCreate(Bundle bundle)
        {
            Console.WriteLine("In Activity");
            base.OnCreate(bundle);
            var g = new Game1();
            SetContentView((View)g.Services.GetService(typeof(View)));
            g.Run();
        }
    }
}

