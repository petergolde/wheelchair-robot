// WARNING
//
// This file has been generated automatically by Xamarin Studio from the outlets and
// actions declared in your storyboard file.
// Manual changes to this file will not be maintained.
//
using Foundation;
using System;
using System.CodeDom.Compiler;
using UIKit;

namespace Thingamabot
{
	[Register ("JoystickViewController")]
	partial class JoystickViewController
	{
		[Outlet]
		[GeneratedCode ("iOS Designer", "1.0")]
		JoystickControl joystick { get; set; }

		void ReleaseDesignerOutlets ()
		{
			if (joystick != null) {
				joystick.Dispose ();
				joystick = null;
			}
		}
	}
}
