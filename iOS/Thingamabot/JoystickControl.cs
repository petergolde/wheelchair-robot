using System;

using UIKit;
using CoreGraphics;
using Foundation;
using System.ComponentModel;

namespace Thingamabot
{
    [Register("JoystickControl")]
    [DesignTimeVisible(true)]
    public class JoystickControl: UIView
    {
        public event EventHandler LocationChanged;

        float x, y;

        public JoystickControl()
        {
        }

        public JoystickControl(IntPtr handle) : base(handle)
        {}

        public float X { get { return x;}}
        public float Y { get { return y;}}


        public override void TouchesBegan(Foundation.NSSet touches, UIEvent evt)
        {
            HandleTouch((UITouch) touches.AnyObject);
        }

        public override void TouchesMoved(Foundation.NSSet touches, UIEvent evt)
        {
            HandleTouch((UITouch) touches.AnyObject);
        }

        public override void TouchesEnded(Foundation.NSSet touches, UIEvent evt)
        {
            SetLocation(0, 0);
        }

        public override void Draw(CGRect rect)
        {
            using (CGContext context = UIGraphics.GetCurrentContext()) {
                CGPoint center = CenterPoint();
                float radius = Radius();
                CGRect circleRect = new CGRect(center.X - radius, center.Y - radius, radius * 2, radius * 2);
                circleRect.Inflate(-4, -4);
                context.SetLineWidth(4);
                UIColor.Black.SetStroke();
                context.StrokeEllipseInRect(circleRect);

                center.X += x * radius;
                center.Y += y * radius;

                float radius2 = Radius() / 7;
                circleRect = new CGRect(center.X - radius2, center.Y - radius2, radius2 * 2, radius2 * 2);
                UIColor.Red.SetFill();
                context.FillEllipseInRect(circleRect);
            }
        }

        void HandleTouch(UITouch touch)
        {
            CGPoint touchLocation = touch.LocationInView(this);
            CGPoint center = CenterPoint();
            float radius = Radius();

            float x = (float) ((touchLocation.X - center.X) / radius);
            float y = (float) ((touchLocation.Y - center.Y) / radius);
            SetLocation(x, y);
        }

        public void SetLocation(float x, float y)
        {
            this.x = x;
            this.y = y;
            if (LocationChanged != null)
                LocationChanged(this, EventArgs.Empty);

            this.SetNeedsDisplay();
        }

        CGPoint CenterPoint()
        {
            return new CGPoint(Bounds.GetMidX(), Bounds.GetMidY());
        }

        float Radius()
        {
            return (float) Math.Min(Bounds.GetMaxX() - Bounds.GetMidX(), Bounds.GetMaxY() - Bounds.GetMidY());
        }
    }
}

