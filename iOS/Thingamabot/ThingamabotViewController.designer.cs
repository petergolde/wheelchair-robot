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
	[Register ("ThingamabotViewController")]
	partial class ThingamabotViewController
	{
		[Outlet]
		[GeneratedCode ("iOS Designer", "1.0")]
		UIBarButtonItem connectButton { get; set; }

		[Outlet]
		[GeneratedCode ("iOS Designer", "1.0")]
		UIButton stopButton { get; set; }

		void ReleaseDesignerOutlets ()
		{
			if (connectButton != null) {
				connectButton.Dispose ();
				connectButton = null;
			}
			if (stopButton != null) {
				stopButton.Dispose ();
				stopButton = null;
			}
		}
	}
}
