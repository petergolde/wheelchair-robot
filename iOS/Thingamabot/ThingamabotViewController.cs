using System;
using System.Drawing;
using System.Collections.Generic;

using Foundation;
using UIKit;

namespace Thingamabot
{
    public partial class ThingamabotViewController : UIViewController
    {
        RblSerialBluetooth bluetooth;
        List<UIViewController> childViewControllers = new List<UIViewController>();
        UIViewController currentChildViewController = null;
        RobotControl robotControl;

        public ThingamabotViewController(IntPtr handle) : base(handle)
        {
        }

        public override void DidReceiveMemoryWarning()
        {
            // Releases the view if it doesn't have a superview.
            base.DidReceiveMemoryWarning();
			
            // Release any cached data, images, etc that aren't in use.
        }

        public void SendRobotCommand(string commandName, int value, int priority = 0)
        {
            if (robotControl != null) {
                robotControl.QueueCommand(commandName, value, priority);
            }
        }

        partial void ConnectClicked(UIBarButtonItem sender)
        {
            if (bluetooth.IsConnected) {
                bluetooth.Disconnect();
            }
            else {
                bluetooth.BeginConnecting();
            }
        }

        partial void HandleFullStopClicked(UIButton sender)
        {
            SendRobotCommand("ml", 0, 2);
            SendRobotCommand("mr", 0, 2);
        }

        partial void HandleReleaseFailSafeClicked(UIButton sender)
        {
            SendRobotCommand("ef", 0, 1);
        }

        void HandleConnectionChanged (object sender, EventArgs e)
        {
            if (bluetooth.IsConnected) {
                Connected();
            }
            else {
                Disconnected();
            }
        }

        void Connected()
        {
            connectButton.Title = "Disconnect";
            robotControl = new RobotControl(bluetooth);
        }

        void Disconnected()
        {
            connectButton.Title = "Connect";
            if (robotControl != null) {
                robotControl.Dispose();
                robotControl = null;
            }
        }

        partial void ControlSchemeChanged(UISegmentedControl sender)
        {
            ShowChildControlViewController((int) sender.SelectedSegment);
        }

        void ShowChildControlViewController(int i)
        {
            if (currentChildViewController != null)
                currentChildViewController.View.RemoveFromSuperview();

            currentChildViewController = childViewControllers[i];
            currentChildViewController.View.Frame = controlView.Bounds;
            controlView.AddSubview(currentChildViewController.View);
        }


        #region View lifecycle

        public override void ViewDidLoad()
        {
            base.ViewDidLoad();
			
            // Perform any additional setup after loading the view, typically from a nib.
            bluetooth = new RblSerialBluetooth();
            bluetooth.ConnectionChanged += HandleConnectionChanged;

            JoystickViewController joystickViewController = (JoystickViewController) Storyboard.InstantiateViewController("JoystickViewController");
            childViewControllers.Add(joystickViewController);  

            ThrottleViewController throttleViewController = (ThrottleViewController) Storyboard.InstantiateViewController("ThrottleViewController");
            throttleViewController.robotViewController = this;
            childViewControllers.Add(throttleViewController);

            foreach (var vc in childViewControllers)
                this.AddChildViewController(vc);

            ShowChildControlViewController(0);
        }

        public override void ViewWillAppear(bool animated)
        {
            base.ViewWillAppear(animated);
        }

        public override void ViewDidAppear(bool animated)
        {
            base.ViewDidAppear(animated);
        }

        public override void ViewWillDisappear(bool animated)
        {
            base.ViewWillDisappear(animated);
        }

        public override void ViewDidDisappear(bool animated)
        {
            base.ViewDidDisappear(animated);
        }

        #endregion
    }
}

