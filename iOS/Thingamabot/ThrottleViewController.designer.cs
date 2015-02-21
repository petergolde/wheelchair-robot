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
		UISlider leftMotorSlider { get; set; }

		[Outlet]
		[GeneratedCode ("iOS Designer", "1.0")]
		UISlider rightMotorSlider { get; set; }

		[Action ("LeftMotorChanged:")]
		[GeneratedCode ("iOS Designer", "1.0")]
		partial void LeftMotorChanged (UISlider sender);

		[Action ("RightMotorChanged:")]
		[GeneratedCode ("iOS Designer", "1.0")]
		partial void RightMotorChanged (UISlider sender);

		void ReleaseDesignerOutlets ()
		{
			if (leftMotorSlider != null) {
				leftMotorSlider.Dispose ();
				leftMotorSlider = null;
			}
			if (rightMotorSlider != null) {
				rightMotorSlider.Dispose ();
				rightMotorSlider = null;
			}
		}
	}
}
