using System;
using System.Collections.Generic;
using System.Linq;

using Robotics.Mobile.Core.Bluetooth.LE;

using Foundation;

namespace Thingamabot
{
    /// <summary>
    /// Event arguments for the TextReceived event.
    /// </summary>
    public class TextReceivedEventArgs: EventArgs
    {
        public readonly string Text;

        public TextReceivedEventArgs(string text)
        {
            this.Text = text;
        }
    }

    /// <summary>
    /// Class to handle dealing with a Bluetooth LE device running the RBL serial service.
    /// </summary>
    public class RblSerialBluetooth: ISerialWriter
    {
        static readonly Guid RBL_SERVICE_UUID = new Guid("713D0000-503E-4C75-BA94-3148F18D941E");
        static readonly Guid RBL_CHAR_TX_UUID = new Guid("713D0002-503E-4C75-BA94-3148F18D941E");
        static readonly Guid RBL_CHAR_RX_UUID = new Guid("713D0003-503E-4C75-BA94-3148F18D941E");

        readonly NSObject invoker = new NSObject();

        Adapter adapter;
        IDevice device;
        IService service;
        ICharacteristic txCharacteristic, rxCharacteristic;

        /// <summary>
        /// Occurs when device is connected or disconnected. Use the IsConnected property
        /// to determine which.
        /// </summary>
        public event EventHandler ConnectionChanged;

        /// <summary>
        /// Occurs when text received.
        /// </summary>
        public event EventHandler<TextReceivedEventArgs> TextReceived;

        public RblSerialBluetooth()
        {
            adapter = Adapter.Current;
            adapter.DeviceConnected += (sender, e) => { invoker.BeginInvokeOnMainThread(() => HandleDeviceConnected(e.Device)); };
            adapter.DeviceDisconnected += (sender, e) => { invoker.BeginInvokeOnMainThread(() => HandleDeviceDisconnected(e.Device)); };
            adapter.DeviceDiscovered += (sender, e) => { invoker.BeginInvokeOnMainThread(() => HandleDeviceDiscovered(e.Device)); };
        }

        void HandleDeviceDiscovered (IDevice device)
        {
            adapter.ConnectToDevice(device);
        }

        async void HandleDeviceConnected (IDevice device)
        {
            service = await device.GetServiceAsync(RBL_SERVICE_UUID);
            if (service == null) {
                Disconnect();
                return;
            }

            txCharacteristic = await service.GetCharacteristicAsync(RBL_CHAR_TX_UUID);
            if (txCharacteristic == null) {
                Disconnect();
                return;
            }

            rxCharacteristic = await service.GetCharacteristicAsync(RBL_CHAR_RX_UUID);
            if (rxCharacteristic == null) {
                Disconnect();
                return;
            }

            rxCharacteristic.ValueUpdated += (sender, e) => { invoker.BeginInvokeOnMainThread(() => HandleValueUpdated(e.Characteristic.StringValue)); };

            this.device = device;

            if (ConnectionChanged != null)
                ConnectionChanged(this, EventArgs.Empty);
        }

        void HandleDeviceDisconnected (IDevice device)
        {
            bool sendEvent = (this.device != null);

            this.device = null;
            service = null;
            txCharacteristic = rxCharacteristic = null;

            if (sendEvent && ConnectionChanged != null)
                ConnectionChanged(this, EventArgs.Empty);
        }

        void HandleValueUpdated(string text)
        {
            if (TextReceived != null)
                TextReceived(this, new TextReceivedEventArgs(text));
        }

        /// <summary>
        /// Gets a value indicating whether we are connected to a bluetooth LE device with the RBL serial service.
        /// </summary>
        /// <value><c>true</c> if we are connected; otherwise, <c>false</c>.</value>
        public bool IsConnected { get { return device != null; }}

        /// <summary>
        /// Begin trying to connect to a device.
        /// </summary>
        public void BeginConnecting()
        {
            adapter.StartScanningForDevices(RBL_SERVICE_UUID);
        }

        /// <summary>
        /// Disconnect from a connected device, if any.
        /// </summary>
        public void Disconnect()
        {
            IDevice deviceToDisconnect = device;

            if (deviceToDisconnect != null) {
                HandleDeviceDisconnected(deviceToDisconnect);
                adapter.DisconnectDevice(deviceToDisconnect);
            }
        }

        /// <summary>
        /// Send text over the RBL serial service to the connected device.
        /// </summary>
        /// <param name="text">Text.</param>
        public void SendText(string text)
        {
            if (rxCharacteristic == null)
                throw new Exception("No device is connected.");

            NSData data = NSData.FromString(text);
            if (data.Length > 20)
                throw new Exception("Cannot write more than 20 bytes of text.");

            rxCharacteristic.Write(data.ToArray());
        }
    }
}

