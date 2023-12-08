/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       {file}                                                    */
/*    Author:       {Alvin Li}                                                */
/*    Created:      {Oct 12, 2023}                                            */
/*    Description:  {Robot code for Over Under Competition}                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// motorL               motor         1
// motorR               motor         2
// airSwitch            motor         3
// lift1                motor         5
// lift2                motor         11
// gun                  motor         6
// clawTop              motor         12
// clawBot              motor         13
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
brain Brain;
controller Controller1;
motor motorL = motor(PORT1, ratio18_1, false);
motor motorR = motor(PORT2, ratio18_1, true);
motor airSwitch = motor(PORT3, ratio18_1, false);
motor lift1 = motor(PORT5, ratio36_1, true);
motor lift2 = motor(PORT11, ratio36_1, false);
motor clawTop = motor(PORT12, ratio18_1, false);
motor clawBot = motor(PORT13, ratio18_1, false);
motor gun = motor(PORT6, ratio6_1, true);

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
    // All activities that occur before the competition starts
    // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
    // ..........................................................................
    // Insert autonomous user code here.
    // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
    // User control code here, inside the loop
    bool airSwitchState = false;
    bool clawStateT = false;
    bool clawStateB = false;
    float multiplier = 1;
    while (1) {
        // This is the main execution loop for the user control program.
        // Each time through the loop your program should update motor + servo
        // values based on feedback from the joysticks.

        // ........................................................................
        // Insert user code here. This is where you use the joystick values to
        // update your motors, etc.
        // ........................................................................

        // motorL.spin(forward, Controller1.Axis3.value(), percent);
        // motorR.spin(forward, Controller1.Axis2.value(), percent);

        // Keep catapult down
        gun.spin(forward, -10, percent);

        // tank drive
        int l = Controller1.Axis3.value();
        int r = Controller1.Axis2.value();
        // controller deadspace
        if (fabs(l) < 10) {
            l = 0;
        }
        motorL.spin(forward, l, percent);
        if (fabs(r) < 10) {
            r = 0;
        }
        motorR.spin(forward, r, percent);

        // Overdrive
        if(Controller1.ButtonR1.pressing() && Controller1.ButtonR2.pressing()) {
            if(multiplier == 1) {
                multiplier = 2;
            } else if (multiplier == 2) {
                multiplier = 1;
            }
        }

        // // pneumatic switch
        // if (Controller1.ButtonB.pressing()) {
        //     if (airSwitchState) {
        //         airSwitch.spin(forward, 70, percent);
        //         wait(250, msec);
        //         airSwitch.spin(forward, 0, percent);
        //         airSwitchState = false;
        //         wait(1000, msec);
        //     } else {
        //         airSwitch.spin(forward, -70, percent);
        //         wait(240, msec);
        //         airSwitch.spin(forward, 0, percent);
        //         airSwitchState = true;
        //         wait(1000, msec);
        //     }
        // }

        // pneumatic switch v2
        if (Controller1.ButtonB.pressing()) {
            if (airSwitchState) {
                airSwitch.setPosition(130, deg);
                // airSwitch.spinTo(130, deg);
                wait(500, msec);
                airSwitchState = false;
            } else {
                airSwitch.setPosition(0, deg);
                // airSwitch.spinTo(0, deg);
                wait(500, msec);
                airSwitchState = true;
            }
        }

        // // Claw
        if (Controller1.ButtonL1.pressing()) {
            if (clawStateT) {
                clawTop.spin(forward, -100, percent);
                wait(500, msec);
                clawStateT = false;
            } else if (clawStateT == false) {
                // clawBot.spinTo(270, deg, 100);
                clawTop.spin(forward, 100, percent);
                wait(500, msec);
                clawStateT = true;
            }
        }
        if (Controller1.ButtonL2.pressing()) {
            if (clawStateB) {
                clawBot.spin(forward, -100, percent);
                wait(500, msec);
                clawStateB = false;
            } else if (clawStateB == false) {
                // clawBot.spinTo(270, deg, 100);
                clawBot.spin(forward, 100, percent);
                wait(500, msec);
                clawStateB = true;
            }
        }
        // Disable Claws to conserve battery
        if (Controller1.ButtonX.pressing()) {
            clawTop.stop();
            clawBot.stop();
        }

        // Forklift
        if (Controller1.ButtonUp.pressing()) {
            lift1.spin(forward, 50, percent);
            lift2.spin(forward, 50, percent);
        } else if (Controller1.ButtonDown.pressing()) {
            // while (!Controller1.ButtonUp.pressing()) {
            lift1.spin(forward, -50, percent);
            lift2.spin(forward, -50, percent);
            // }
        } else {
            lift1.spin(forward, 0, percent);
            lift2.spin(forward, 0, percent);
        }

        // Gun
        if (Controller1.ButtonA.pressing()) {
            gun.spin(forward, 150, percent);
            wait(1000, msec);
            gun.spin(forward, -10, percent);
            wait(750, msec);
        }

        wait(20, msec);  // Sleep the task for a short amount of time to
                         // prevent wasted resources.Z
    }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
    // Set up callbacks for autonomous and driver control periods.
    Competition.autonomous(autonomous);
    Competition.drivercontrol(usercontrol);

    // Run the pre-autonomous function.
    pre_auton();

    usercontrol();
    // Prevent main from exiting with an infinite loop.
    while (true) {
        wait(100, msec);
    }
}