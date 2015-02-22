
using System;
using System.Drawing;

using Foundation;
using UIKit;
using CoreGraphics;

namespace Thingamabot
{
    public partial class ThrottleViewController : UIViewController, IThingamabotChildController
    {
        public ThingamabotViewController RobotViewController { get; set; }

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
			

        }

        public void Reset()
        {
            if (turnMotorSlider != null)
                turnMotorSlider.Value = 0;
            if (speedMotorSlider != null)
                speedMotorSlider.Value = 0;
        }

        public override void ViewWillLayoutSubviews()
        {
            base.ViewWillLayoutSubviews();

            turnMotorSlider.RemoveFromSuperview();
            turnMotorSlider.RemoveConstraints(View.Constraints);
            turnMotorSlider.TranslatesAutoresizingMaskIntoConstraints = true;
            turnMotorSlider.Transform = CGAffineTransform.MakeRotation((float)Math.PI / 2);
            View.AddSubview(turnMotorSlider);
            CGPoint center = new CGPoint(40, View.Bounds.Height * 0.75F);
            float width = 40;
            float height = (float) View.Bounds.Height * 0.5F;
            turnMotorSlider.Frame = CGRect.FromLTRB(center.X - width / 2, center.Y - height / 2, center.X + width / 2, center.Y + height / 2);
        }

        partial void SpeedMotorChanged(UISlider sender)
        {
            RobotViewController.SendRobotCommand("ds", (int) sender.Value, 1);
        }

        partial void TurnMotorChanged(UISlider sender)
        {
            RobotViewController.SendRobotCommand("dt", (int) sender.Value, 1);
        }
    }
}

