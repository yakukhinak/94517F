#include "main.h"
#include "pros/misc.h"
#include "robodash/views/console.hpp"
#include "dsr.hpp"
#include "autons.hpp"
#include "selector.hpp"


/////
// For installation, upgrading, documentations, and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////

//ctrl+h  ctrl+i  for highlight

/*added externs to all teh following: extern bool driver;
extern rd::Console Brain;
extern rd::Console Odom;
extern rd::Image image2;
extern ez::Drive chassis;
extern ez::tracking_wheel horiz_tracker;
extern ez::tracking_wheel vert_tracker;
extern rd::Selector selector;
extern std::vector<std::function<void()>> screen_views;
extern int screenCounter;*/
bool driver = true;
rd::Console Brain; 
rd::Console Odom;
rd::Image image2("/usd/catresize.bin", "CAT");

//you can have more than one consle use one for odom!!!

// Chassis constructor
ez::Drive chassis(
    // These are your drive motors, the first motor is used for sensing!
    {1,-2,-5},     // Left Chassis Ports (negative port will reverse it!)
    {8,9,-10},  // Right Chassis Ports (negative port will reverse it!)

    13,      // IMU Port
    4.125,  // Wheel Diameter (Remember, 4" wheels without screw holes are actually 4.125!)
    343);   // Wheel RPM = cartridge * (motor gear / wheel gear)

// Uncomment the trackers you're using here!
// - `8` and `9` are smart ports (making these negative will reverse the sensor)
//  - you should get positive values on the encoders going FORWARD and RIGHT
// - `2.75` is the wheel diameter
// - `4.0` is the distance from the center of the wheel to the center of the robot
 ez::tracking_wheel horiz_tracker(11, 2, -3.5);  // This tracking wheel is perpendicular to the drive wheels
 ez::tracking_wheel vert_tracker(19, 2, 2.0);   // This tracking wheel is parallel to the drive wheels

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

rd::Selector selector({
  {"Odom Angular Set", odom_angular_set},
  {"Auton 1", odom_test},
  {"DSR", dsr_test},
  {"drive example", drive_example},
  {"turn example", turn_example},
  {"drive and turn", drive_and_turn},
  {"Auton 5", wait_until_change_speed},
  {"Auton 6", swing_example},
  {"Auton 7", motion_chaining},
  {"Auton 8", combining_movements},
  {"Auton 9", interfered_example},
  {"Auton 10", odom_drive_example},
  {"Auton 11", odom_pure_pursuit_example},
  {"Auton 12", odom_pure_pursuit_wait_until_example},
  {"Auton 13", odom_boomerang_example},
  {"Auton 14", odom_boomerang_injected_pure_pursuit_example},
  {"Get Pose", pose_get}
});



void initialize() {
  // Print our branding over your terminal :D
 
  ez::ez_template_print();

  pros::delay(500);  // Stop the user from doing anything while legacy ports configure
  // Look at your horizontal tracking wheel and decide if it's in front of the midline of your robot or behind it
  //  - change `back` to `front` if the tracking wheel is in front of the midline
  //  - ignore this if you aren't using a horizontal tracker
  // chassis.odom_tracker_back_set(&horiz_tracker);
  // Look at your vertical tracking wheel and decide if it's to the left or right of the center of the robot
  //  - change `left` to `right` if the tracking wheel is to the right of the centerline
  //  - ignore this if you aren't using a vertical tracker
  // chassis.odom_tracker_left_set(&vert_tracker);

  // Configure your chassis controls
  chassis.opcontrol_curve_buttons_toggle(true);   // Enables modifying the controller curve with buttons on the joysticks
  chassis.opcontrol_drive_activebrake_set(0.0);   // Sets the active brake kP. We recommend ~2.  0 will disable.
  chassis.opcontrol_curve_default_set(0.0, 0.0);  // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)

  // Set the drive to your own constants from autons.cpp!
  default_constants();
  

  // These are already defaulted to these buttons, but you can change the left/right curve buttons here!
  // chassis.opcontrol_curve_buttons_left_set(pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT);  // If using tank, only the left side is used.
  // chassis.opcontrol_curve_buttons_right_set(pros::E_CONTROLLER_DIGITAL_Y, pros::E_CONTROLLER_DIGITAL_A);
  //rd::Image image1("/usd/AlienCats.bin", "ALIENCAT");
  //rd::Image image2("/usd/catresize.bin", "CAT");

  // Initialize chassis and auton selector
  chassis.initialize();
  //ez::as::initialize();
  pros::Task ez_screen([](void*){ ez_screen_task(); }, nullptr, "EZ Screen");
  
  master.rumble(chassis.drive_imu_calibrated() ? "." : "---");
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
  // . . .
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
  // . . .
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
  chassis.pid_targets_reset();                // Resets PID targets to 0
  chassis.drive_imu_reset();                  // Reset gyro position to 0
  chassis.drive_sensor_reset();               // Reset drive sensors to 0
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);    // Set the current position, you can start at a specific position with this
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD);  // Set motors to hold.  This helps autonomous consistency

  
  /*
  Odometry and Pure Pursuit are not magic

  It is possible to get perfectly consistent results without tracking wheels,
  but it is also possible to have extremely inconsistent results without tracking wheels.
  When you don't use tracking wheels, you need to:
   - avoid wheel slip
   - avoid wheelies
   - avoid throwing momentum around (super harsh turns, like in the example below)
  You can do cool curved motions, but you have to give your robot the best chance
  to be consistent
  */
 selector.run_auton();
  //ez::as::auton_selector.selected_auton_call();  // Calls selected auton from autonomous selector
}




std::vector<std::function<void()>> screen_views = {
    []() { selector.focus(); },
    []() { Brain.focus(); },
    []() { Odom.focus(); },
    []() { image2.focus(); }
};

int screenCounter = 0;
//[]() { Distance.focus(); },
//free singal seletctor controls Y, RIGHT, UP
void opcontrol() {
  // This is preference to what you like to drive on
  chassis.drive_brake_set(MOTOR_BRAKE_COAST);

  while (true) {
    // Gives you some extras to make EZ-Template ezier
    //if (master.get_digital_new_press(DIGITAL_RIGHT) && master.get_digital(DIGITAL_Y))

    //chassis.opcontrol_tank();  // Tank control
     chassis.opcontrol_arcade_standard(ez::SPLIT);   // Standard split arcade
    //chassis.opcontrol_arcade_standard(ez::SINGLE);  // Standard single arcade
    // chassis.opcontrol_arcade_flipped(ez::SPLIT);    // Flipped split arcade
    // chassis.opcontrol_arcade_flipped(ez::SINGLE);   // Flipped single arcade

    // . . .
    // Put more user control code here!
    // . . .
    if (master.get_digital_new_press(DIGITAL_L2) && (!pros::competition::is_connected())) {
      driver = !driver;
      Brain.clear();
      Brain.printf(
        "Driver mode: %s", driver ? "ON" : "OFF\n");
    }
    if (!driver){
      if (master.get_digital_new_press(DIGITAL_L1)) {
            screenCounter = (screenCounter + 1) % screen_views.size();
            screen_views[screenCounter]();
      }
      if (master.get_digital_new_press(DIGITAL_L2)) {
        screenCounter = (screenCounter - 1) % screen_views.size();
            screen_views[screenCounter]();
      }
      //Distance_print();
      ez_template_extras();

  // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
  }
    pros::delay(50);
  }
}
