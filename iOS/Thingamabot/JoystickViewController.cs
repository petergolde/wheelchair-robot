
using System;
using System.Drawing;

using Foundation;
using UIKit;

namespace Thingamabot
{
    public partial class JoystickViewController : UIViewController, IThingamabotChildController
    {
        public ThingamabotViewController RobotViewController { get; set; }

        public JoystickViewController(IntPtr handle) : base(handle)
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
			
            joystick.LocationChanged += JoystickLocationChanged;
            // Perform any additional setup after loading the view, typically from a nib.
        }

        void JoystickLocationChanged (object sender, EventArgs e)
        {
            int speed = - (int) (joystick.Y * 100);
            int turn = (int) (joystick.X * 100);
            RobotViewController.SendRobotCommand("ds", speed, 1);
            RobotViewController.SendRobotCommand("dt", turn, 1);
        }

        public void Reset()
        {
            if (joystick != null)
                joystick.SetLocation(0, 0);
        }
    }
}

