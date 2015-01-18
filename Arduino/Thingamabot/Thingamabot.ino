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
// sp <value>:     Servo Pan. Value from 0 - 160 (eventually 180).
// st <value>:     Servo Tilt. Value from 0 - 160 (eventually 180).
// ka:             Keep alive. Prevent from entering fail-safe mode. 
// fs <value>:     Set fail-safe interval in milliseconds. Use 0 to disable fail-safe mode.
// ef:             Exit fail safe. Once fail-safe mode is entered, you must
//                 use this command to exit.


#include <SPI.h>
#include <Servo.h> 
#include <boards.h>
#include <RBL_nRF8001.h>
#include <services.h> 

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
const int LEFT_MOTOR_PIN = 5;
const int RIGHT_MOTOR_PIN = 6;

Servo left_motor_controller;
Servo right_motor_controller;

// Configure motor hardware.
void setup_motors()
{
  left_motor_controller.attach(LEFT_MOTOR_PIN);
  right_motor_controller.attach(RIGHT_MOTOR_PIN);
  set_motor(LEFT_MOTOR, 0);
  set_motor(RIGHT_MOTOR, 0);
}

// Set a motor to a particular speed, or off.
// "motor" is which motor to set.
// "speed" is a value from -100 to 100. 0 is off, 
//         100 is full forward, -100 is full reverse
void set_motor(int motor, int speed)
{
    // CONSIDER: Should setting a motor to non-zero automatically turn the brakes
    // off?
  
      // Motors are controlled by PWM so we are using servo classes to control them.
    // 180 is full forward, 90 is off, and 0 is full backward
    int servoValue = (speed + 100)* 9 / 10; // scale (-100 to 100) to (0 to 180)

    if (motor == LEFT_MOTOR)
    {
        left_motor_controller.write(servoValue);
        log(F("Set left motor to:"), speed);
        log(F(" Left PWM value is:"), servoValue);
    }
    else if (motor == RIGHT_MOTOR)
    {
        right_motor_controller.write(servoValue);
        log(F("Set right motor to:"), speed);
        log(F(" Right PWM value is:"), servoValue);
    }
}

///////////////////////////////////////////////
// BRAKES

// Values designating the brakes.
const int LEFT_BRAKE = 0;
const int RIGHT_BRAKE = 1;
const int LEFT_BRAKE_PIN = 3;
const int RIGHT_BRAKE_PIN = 4;

// Configure brake hardware.
void setup_brakes()
{
  pinMode(LEFT_BRAKE_PIN, OUTPUT); 
  pinMode(RIGHT_BRAKE_PIN, OUTPUT); 
  set_brake(LEFT_BRAKE, true);
  set_brake(RIGHT_BRAKE, true);
}

// Set a brake to on or off
void set_brake(int brake, boolean on)
{
    const char * valueString = on ? "ON" : "OFF";
    if (brake == LEFT_BRAKE) 
    {
        digitalWrite(LEFT_BRAKE_PIN, on ? HIGH : LOW);
        log(F("Set left brake to:"), valueString);
    }
    else if (brake == RIGHT_BRAKE)
    {
        digitalWrite(RIGHT_BRAKE_PIN, on ? HIGH : LOW);
        log(F("Set right brake to:"), valueString);
    }
}

///////////////////////////////////////////////
// SERVOS

// Values designating the servos.
const int PAN_SERVO = 0;
const int TILT_SERVO = 1;
const int PAN_SERVO_PIN = 9;
const int TILT_SERVO_PIN = 10;

// Declare servos
Servo pan_servo;
Servo tilt_servo;

// Configure pan/tilt servo hardware.
void setup_servos()
{
  pan_servo.attach(PAN_SERVO_PIN);
  tilt_servo.attach(TILT_SERVO_PIN);
  pan_servo.write(90);
  tilt_servo.write(90);
}


// Set a servo to a particular setting.
// "servo" is which servo to set.
// "angle" is a value from 0 to 160 (eventually 180), in degrees, except that 999 indicates return to home position
void set_servo(int servoID, int angle)
{
   
    if (angle == 999) {
        pan_servo.write(90);
        tilt_servo.write(90);
    }
    else if (servoID == PAN_SERVO) {
        pan_servo.write(angle);
        log(F("Set pan servo to:"), angle);
    }
    else if (servoID == TILT_SERVO) {
        tilt_servo.write(angle);
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
        
        // Clamp value to 0 to 160 - eventually 180.
        if (value > 160 && value != 999)  // using 999 as value to return camera to home position
            value = 160;
        else if (value < 0)
            value = 0;
            
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

const int BLUETOOTH_BUFFER_SIZE = 80;
char bluetooth_buffer[BLUETOOTH_BUFFER_SIZE];
char * bluetooth_next;  // Next free character in the buffer.

void clear_bluetooth_buffer()
{
    bluetooth_next = bluetooth_buffer;
}


// Check bluetooth for a command. If a command is 
// found, call dispatch_command.
void read_bluetooth()
{
    while (ble_available() > 0) {
        int b = ble_read();

        if (b == -1 || b == 0 || b == '\r') {
            // Ignore nulls, CR.
        }
        else if (b == '\n' || b == '!') {
            // LF or '!' means end of command. Dispatch the command.
            *bluetooth_next++ ='\0';  // NUL terminate command.
            
            // Acknowledge command.
            ble_write_bytes((unsigned char *)"ACK: ", 5);
            ble_write_bytes((unsigned char *)bluetooth_buffer, strlen(bluetooth_buffer));
            
            // Dispatch the command.
            dispatch_command(bluetooth_buffer);
            clear_bluetooth_buffer();
        }
        else if (bluetooth_next < bluetooth_buffer + BLUETOOTH_BUFFER_SIZE - 1) {
            // Store character into buffer.
            *bluetooth_next++ = (char) b;
        }
    }
}

// Set up Bluetooth LE hardware.
void setup_bluetooth()
{
    clear_bluetooth_buffer();
    
    // Default pins set to 9 and 8 for REQN and RDYN
    // Set your REQN and RDYN here before ble_begin() if you need
    ble_set_pins(7, 8);
    
    // Set your BLE Shield name here, max. length 10
    ble_set_name("Thingmabot");
    
    // Init. and start BLE library.
    ble_begin();
}

// This must be called from main loop to process bluetooth commands.
void process_bluetooth()
{
    ble_do_events();
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
    process_bluetooth();
    read_bluetooth();
    
    // Check if we need to enter fail-safe mode.
    check_failsafe();
}


