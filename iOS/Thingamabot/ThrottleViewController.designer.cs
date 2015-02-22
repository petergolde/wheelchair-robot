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
	[Register ("ThrottleViewController")]
	partial class ThrottleViewController
	{
		[Outlet]
		[GeneratedCode ("iOS Designer", "1.0")]
		UISlider speedMotorSlider { get; set; }

		[Outlet]
		[GeneratedCode ("iOS Designer", "1.0")]
		UISlider turnMotorSlider { get; set; }

		[Action ("SpeedMotorChanged:")]
		[GeneratedCode ("iOS Designer", "1.0")]
		partial void SpeedMotorChanged (UISlider sender);

		[Action ("TurnMotorChanged:")]
		[GeneratedCode ("iOS Designer", "1.0")]
		partial void TurnMotorChanged (UISlider sender);

		void ReleaseDesignerOutlets ()
		{
			if (speedMotorSlider != null) {
				speedMotorSlider.Dispose ();
				speedMotorSlider = null;
			}
			if (turnMotorSlider != null) {
				turnMotorSlider.Dispose ();
				turnMotorSlider = null;
			}
		}
	}
}
