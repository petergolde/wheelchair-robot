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
		UIView controlView { get; set; }

		[Outlet]
		[GeneratedCode ("iOS Designer", "1.0")]
		UIButton releaseFailSafeButton { get; set; }

		[Outlet]
		[GeneratedCode ("iOS Designer", "1.0")]
		UIButton stopButton { get; set; }

		[Action ("ConnectClicked:")]
		[GeneratedCode ("iOS Designer", "1.0")]
		partial void ConnectClicked (UIBarButtonItem sender);

		[Action ("ControlSchemeChanged:")]
		[GeneratedCode ("iOS Designer", "1.0")]
		partial void ControlSchemeChanged (UISegmentedControl sender);

		[Action ("HandleFullStopClicked:")]
		[GeneratedCode ("iOS Designer", "1.0")]
		partial void HandleFullStopClicked (UIButton sender);

		[Action ("HandleReleaseFailSafeClicked:")]
		[GeneratedCode ("iOS Designer", "1.0")]
		partial void HandleReleaseFailSafeClicked (UIButton sender);

		void ReleaseDesignerOutlets ()
		{
			if (connectButton != null) {
				connectButton.Dispose ();
				connectButton = null;
			}
			if (controlView != null) {
				controlView.Dispose ();
				controlView = null;
			}
			if (releaseFailSafeButton != null) {
				releaseFailSafeButton.Dispose ();
				releaseFailSafeButton = null;
			}
			if (stopButton != null) {
				stopButton.Dispose ();
				stopButton = null;
			}
		}
	}
}
