// Arduino code for Thingamashop wheelchair robot.
//
// Accepts commands over serial or Bluetooth LE.
// Commands are two lower case letters, space, an integer value,
// then CR, LF.
//
// A fail-safe mode will shut down the robot if no commands
// are processed within a fail-safe interval. Once fail-safe mode
// is entered, the "ef" command must be used to exit fail-safe mode.
// 
// Command summary:
// 
// ml <value>:     Motor Left. Value from -100 to 100. 0 is off.
// mr <value>:     Motor Right. Value from -100 to 100. 0 is off.
// bl <value>:     Brake Left. 0 is off, other value is on.
// br <value>:     Break Right. 0 is off, other value is on.
// sp <value>:     Servo Pan. Value from -180 to 180.
// st <value>:     Servo Tilt. Value from -180 to 180.
// ka:             Keep alive. Prevent from entering fail-safe mode. 
// fs <value>:     Set fail-safe interval in milliseconds. Use 0 to disable fail-safe mode.
// ef:             Exit fail safe. Once fail-safe mode is entered, you must
//                 use this command to exit.

//////////////////////////////////////////////
// UTILITY routines.

boolean logging = true;      // Set to true to enable logging to serial port.

// Log a message.
void log(const __FlashStringHelper * message)
{
    if (logging) {
        Serial.print(F("* "));
        Serial.println(message);
    }
}

// Log a message and an integer value.
void log(const __FlashStringHelper * message, long value)
{
    if (logging) {
        Serial.print(F("* "));
        Serial.print(message);
        Serial.print(F(" "));
        Serial.println(value);
    }
}

// Log two strings on same line.
void log(const __FlashStringHelper * message, const char * value)
{
    if (logging) {
        Serial.print(F("* "));
        Serial.print(message);
        Serial.print(F(" "));
        Serial.println(value);
    }
}

///////////////////////////////////////////////
// MOTORS

// Values designating the motors.
const int LEFT_MOTOR = 0;
const int RIGHT_MOTOR = 1;

// Configure motor hardware.
void setup_motors()
{
    // TODO: Configure motor hardware.
}

// Set a motor to a particular speed, or off.
// "motor" is which motor to set.
// "speed" is a value from -100 to 100. 0 is off, 
//         100 is full forward, -100 is full reverse
void set_motor(int motor, int speed)
{
    // TODO: Set motor hardware.
    // CONSIDER: Should setting a motor to non-zero automatically turn the brakes
    // off?
  
    if (motor == LEFT_MOTOR) 
        log(F("Set left motor to:"), speed);
    else if (motor == RIGHT_MOTOR)
        log(F("Set right motor to:"), speed);
}

///////////////////////////////////////////////
// BRAKES

// Values designating the brakes.
const int LEFT_BRAKE = 0;
const int RIGHT_BRAKE = 1;

// Configure break hardware.
void setup_brakes()
{
    // TODO: Configure brake hardware.
}

// Set a brake to on or off
void set_brake(int brake, boolean on)
{
    // TODO: Set brake hardware.
   
    const char * valueString = on ? "ON" : "OFF";
    if (brake == LEFT_BRAKE) 
        log(F("Set left brake to:"), valueString);
    else if (brake == RIGHT_BRAKE)
        log(F("Set right brake to:"), valueString);
}

///////////////////////////////////////////////
// SERVOS

// Values designating the servos.
const int PAN_SERVO = 0;
const int TILT_SERVO = 1;

// Configure servo hardware.
void setup_servos()
{
    // TODO: Configure servo hardware.
}

// Set a servo to a particular setting.
// "servo" is which servo to set.
// "angle" is a value from -180 to 180, in degrees
void set_servo(int servo, int angle)
{
    // TODO: Set servo hardware.
   
    if (servo == PAN_SERVO) {
        log(F("Set pan servo to:"), angle);
    }
    else if (servo == TILT_SERVO) {
        log(F("Set tilt servo to:"), angle);
    }
}

//////////////////////////////////////////////
// FAILSAFE routines.
// 
// The fail-safe timer puts the robot into fail-safe
// mode if a command isn't received within a short amount
// of time.

boolean failsafe_enabled = true;  // is fail-safe enabled?
boolean failsafe_on = false;      // is fail-safe currently on?
long failsafe_interval = 2000;    // ms before entering fail-safe mode.
long failsafe_last_input;         // last time a command was received.

// Enter the fail-safe mode. All hardward (motors, brakes, etc.) should return
// to the fail-safe position.
void enter_failsafe()
{
    // Set motors to off and engage brakes.
    failsafe_on = true;
    set_motor(LEFT_MOTOR, 0);
    set_motor(RIGHT_MOTOR, 0);
    set_brake(LEFT_BRAKE, true);
    set_brake(RIGHT_BRAKE, true);
    
    log(F("Entered fail-safe mode."));
}

void exit_failsafe()
{
    if (failsafe_on) {
        // If necessary, do anything to the hardware needed to exit failsafe mode.

        reset_failsafe_timer();
        failsafe_on = false;
        log(F("Exiting fail-safe mode."));
    }
}

// Set the fail-safe interval to the given number of milliseconds. If 
// <= 0, turn fail-safe monitoring off.
void set_failsafe(int intervalMillis)
{
    if (intervalMillis > 0) {
        failsafe_enabled = true;
        failsafe_interval = intervalMillis;
        log(F("Failsafe interval set to:"), intervalMillis);
    }
    else {
        failsafe_enabled = false;
        log(F("Failsafe disabled."));
    }
}

// Reset the fail-safe timer 
void reset_failsafe_timer()
{
    failsafe_last_input = millis();
}

// Check to see if we should enter fail-safe mode.
void check_failsafe()
{
    if (failsafe_enabled && !failsafe_on && (long) millis() - failsafe_last_input >= failsafe_interval) {
        enter_failsafe();
    }
}


///////////////////////////////////////////////
// COMMAND PARSING AND DISPATCH

// Dispatch a command.
// Commands are a two-letter lowercase designator, followed by a 
// space, then an integer value (-32767 to 32768).
void dispatch_command(char * commandString)
{
    log(F("Command received: "), commandString);
    
    int len = strlen(commandString);
    if (len < 2) { 
        log(F("Command string is too short -- ignoring"));
        return;
    }
    
    // Parse the command string.
    
    char c1 = commandString[0];
    char c2 = commandString[1];

    if (len > 2 && commandString[2] != ' ') {
        log(F("Command string wrong format -- ignoring"));
        return;
    }
    
    int value = 0;
    
    if (len >= 4) {
        value = atoi(commandString + 3);
    }

    // First -- commands that are allowed in fail-safe mode. 
    if (c1 == 'e' && c2 == 'f') {
        // "ef" - exit failsafe.
        exit_failsafe();
        return;
    }
    else if (c1 == 'f' && c2 == 's') {
        // "fs" - set failsafe interval
        set_failsafe((long) value);
        return;
    }
     
    // Don't allow other commands until exiting failsafe mode.
    
    if (failsafe_on) {
        // The fail-safe is ON. No commands until we explicitly exit fail-safe mode.
        log(F("Failsafe is ON. Command ignored. Use \"ef\" command to exit fail-safe mode."));
        return;
    }

    // Regular commands.    
    if (c1 == 'm') {
        // "ml" - Motor Left; "mr" - Motor right
        
        // Clamp value to -100 to 100.
        if (value > 100)
            value = 100;
        else if (value < -100)
            value = -100;
            
        if (c2 == 'l')
            set_motor(LEFT_MOTOR, value);
        else if (c2 == 'r')
            set_motor(RIGHT_MOTOR, value);
        else
            log(F("Invalid motor designator"));
    }
    else if (c1 == 's') {
        // "sp" - pan servo, "st" - tilt servo
        
        // Clamp value to -180 to 180.
        if (value > 180)
            value = 180;
        else if (value < -180)
            value = -180;
            
        if (c2 == 'p')
            set_servo(PAN_SERVO, value);
        else if (c2 == 't')
            set_servo(TILT_SERVO, value);
        else
            log(F("Invalid servo designator"));    
    }
    else if (c1 == 'b') {
        // "bl" - Brake Left; "br" - Brake Right
        
        if (c2 == 'l')
            set_brake(LEFT_BRAKE, value != 0);
        else if (c2 == 'r')
            set_brake(RIGHT_BRAKE, value != 0);
        else
            log(F("Invalid brake designator"));
    }
    else if (c1 == 'k' && c2 == 'a') {
        // "ka" - keep alive for failsafe. Does nothing.
    }
    else {
        log(F("Ignoring unknown command"));
        return;
    }
    
    // Reset the fail-safe timer.
    reset_failsafe_timer();
}

///////////////////////////////////////////////
// BLUETOOTH

// Set up Bluetooth LE hardward.
void setup_bluetooth()
{
    // TODO: Setup Bluetooth hardware.
}

// Check bluetooth for a command. If a command is 
// found, call dispatch_command.
void read_bluetooth()
{
    // TODO: read characters from Bluetooth.
}


///////////////////////////////////////////////
// SERIAL

const long baudRate = 57600;

const int SERIAL_BUFFER_SIZE = 80;
char serial_buffer[SERIAL_BUFFER_SIZE];
char * serial_next;  // Next free character in the buffer.

void clear_serial_buffer()
{
    serial_next = serial_buffer;
}

// Set up Serial hardware.
void setup_serial()
{
    Serial.begin(baudRate);
    clear_serial_buffer();
}

// Check serial for a command. If a command is 
// found, call dispatch_command.
void read_serial()
{
    while (Serial.available() > 0) {
        int b = Serial.read();

        if (b == -1 || b == 0 || b == '\r') {
            // Ignore nulls, CR.
        }
        else if (b == '\n') {
            // LF means end of command. Dispatch the command.
            *serial_next++ ='\0';  // NUL terminate command.
            dispatch_command(serial_buffer);
            clear_serial_buffer();
        }
        else if (serial_next < serial_buffer + SERIAL_BUFFER_SIZE - 1) {
            // Store character into buffer.
            *serial_next++ = (char) b;
        }
    }
}


///////////////////////////////////////////////
// SETUP and MAIN LOOP

// Set up all the devices and the fail-safe timer.
void setup()
{
    setup_motors();
    setup_brakes();
    setup_servos();
    setup_serial();
    setup_bluetooth();
    reset_failsafe_timer();

    log(F("Hardware initialized."));
    
    // Enter fail-safe mode to begin with.
    enter_failsafe();
}

// This is the main loop. It should always complete
// rapidly so it can continue to monitor inputs (e.g., serial port,
// bluetooth, etc.)
void loop()
{
    // Check for inputs. These functions will call dispatch_command
    // if an input command is read.
    read_serial();
    read_bluetooth();
    
    // Check if we need to enter fail-safe mode.
    check_failsafe();
}


