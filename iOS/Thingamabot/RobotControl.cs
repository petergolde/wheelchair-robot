using System;
using System.Collections.Generic;
using Foundation;

namespace Thingamabot
{
    // Class to manage controlling the robot over a simple serial connection, managing
    // sending commands and keeping the robot alive.
    class RobotControl: IDisposable
    {
        const double secondsPerCommand = 0.05;
        const int ticksPerKeepAlive = 10;

        // The serial connection.
        ISerialWriter writer;

        // Timer that controls writing commands.
        NSTimer timer;
        int ticksWithoutCommand;

        // The queue of commands waiting to be transmitted to the command.
        // The item at index 0 is next in line.
        List<ControlCommand> commandQueue = new List<ControlCommand>();

        public RobotControl(ISerialWriter writer)
        {
            this.writer = writer;

            timer = NSTimer.CreateRepeatingScheduledTimer(secondsPerCommand, TimerTick);
        }

        public void Dispose()
        {
            if (timer != null) {
                timer.Invalidate();
                timer.Dispose();
                timer = null;
            }
        }

        public void QueueCommand(string commandName, int value, int priority = 0)
        {
            ControlCommand command = new ControlCommand(commandName, value, priority);
            MergeInQueue(command);
        }

        void MergeInQueue(ControlCommand command)
        {
            // See if there is already a command of the given kind. If so, just 
            // replace it.
            for (int i = 0; i < commandQueue.Count; ++i) {
                if (commandQueue[i].CommandName == command.CommandName) {
                    commandQueue[i] = command;
                    return;
                }
            }

            // Insert into the correct place based on priority.
            int index;
            for (index = 0; index < commandQueue.Count; ++index) {
                if (commandQueue[index].Priority < command.Priority)
                    break;
            }

            commandQueue.Insert(index, command);
        }

        void TimerTick(NSTimer timer)
        {
            if (commandQueue.Count > 0) {
                ControlCommand command = commandQueue[0];
                commandQueue.RemoveAt(0);

                TransmitCommand(command);
                ticksWithoutCommand = ticksPerKeepAlive;
            }
            else {
                // If we haven't send a command for a while, send a keep alive command.
                --ticksWithoutCommand;
                if (ticksWithoutCommand <= 0) {
                    TransmitCommand(new ControlCommand("ka", 0));
                    ticksWithoutCommand = ticksPerKeepAlive;
                }
            }
        }

        void TransmitCommand(ControlCommand command)
        {
            string text = string.Format("{0} {1}\n", command.CommandName, command.Value);
            writer.SendText(text);
            Console.WriteLine(text);
        }
    }

    // The interface used to write to the robot.
    interface ISerialWriter
    {
        void SendText(string text);
    }

    // Encapsulates a single command to the robot.
    class ControlCommand
    {
        public readonly string CommandName;  // Two-letter command name.
        public readonly int Value;           // Value for the command.
        public readonly int Priority;        // Priority for the command (0 = normal, 1 = high, 2 = critical)

        public ControlCommand(string commandName, int value, int priority = 0)
        {
            this.CommandName = commandName;
            this.Value = value;
            this.Priority = priority;
        }
    }
}

