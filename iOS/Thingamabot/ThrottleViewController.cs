
using System;
using System.Drawing;

using Foundation;
using UIKit;

namespace Thingamabot
{
    public partial class ThrottleViewController : UIViewController
    {
        public ThingamabotViewController robotViewController;

        public ThrottleViewController(IntPtr handle) : base(handle)
        {
        }

        public override void DidReceiveMemoryWarning()
        {
            // Releases the view if it doesn't have a superview.
            base.DidReceiveMemoryWarning();
			
            // Release any cached data, images, etc that aren't in use.
        }

        public override void ViewDidLoad()
        {
            base.ViewDidLoad();
			
            // Perform any additional setup after loading the view, typically from a nib.
        }

        partial void LeftMotorChanged(UISlider sender)
        {
            robotViewController.SendRobotCommand("ml", (int) sender.Value, 1);
        }

        partial void RightMotorChanged(UISlider sender)
        {
            robotViewController.SendRobotCommand("mr", (int) sender.Value, 1);
        }
    }
}

