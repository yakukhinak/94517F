#include "main.h"
#include "pros/misc.h"
#include "robodash/views/console.hpp"
//#include "main.cpp"


void screen_print_tracker(ez::tracking_wheel *tracker, std::string name, int line) {
  std::string tracker_value = "", tracker_width = "";
  // Check if the tracker exists
  if (tracker != nullptr) {
    tracker_value = name + " tracker: " + util::to_string_with_precision(tracker->get());             // Make text for the tracker value
    tracker_width = "  width: " + util::to_string_with_precision(tracker->distance_to_center_get());  // Make text for the distance to center
  }
  //ez::screen_print(tracker_value + tracker_width, line);  // Print final tracker text
}

/**
 * Ez screen task
 * Adding new pages here will let you view them during user control or autonomous
 * and will help you debug problems you're having
 */
void ez_screen_task() {
//resetX(front_left, -7);
//resetY(right_s, -7.5);
 while (true) {
    // Check if we want to debug (e.g., not in competition)
    if (!pros::competition::is_connected()) {
      
      // Clear and print new odom data to the Robodash console
      Odom.clear();
      Odom.printf("x: %.2f\ny: %.2f\na: %.2f\n", 
                     chassis.odom_x_get(), 
                     chassis.odom_y_get(), 
                     chassis.odom_theta_get());
                     
      // Add your tracker logic here using console.println()
    }
    pros::delay(20);
  }
}

//pros::Task ezScreenTask(ez_screen_task);

/**
 * Gives you some extras to run in your opcontrol:
 * - run your autonomous routine in opcontrol by pressing DOWN and B
 *   - to prevent this from accidentally happening at a competition, this
 *     is only enabled when you're not connected to competition control.
 * - gives you a GUI to change your PID values live by pressing X
 */
void ez_template_extras() {
  // Only run this when not connected to a competition switch
  if (!pros::competition::is_connected() && !driver) {
    // PID Tuner
    // - after you find values that you're happy with, you'll have to set them in auton.cpp

    // Enable / Disable PID Tuner
    //  When enabled:
    //  * use A and Y to increment / decrement the constants
    //  * use the arrow keys to navigate the constants
    if (master.get_digital_new_press(DIGITAL_X))
      chassis.pid_tuner_toggle();

    // Trigger the selected autonomous routine
    if (master.get_digital(DIGITAL_B) && master.get_digital(DIGITAL_DOWN)) {
      pros::motor_brake_mode_e_t preference = chassis.drive_brake_get();
      autonomous();
      chassis.drive_brake_set(preference);
    }

    // Allow PID Tuner to iterate
    chassis.pid_tuner_iterate();
  }

  // Disable PID Tuner when connected to a comp switch
  else {
    if (chassis.pid_tuner_enabled())
      chassis.pid_tuner_disable();
  }
}
//}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
 /*slector = rd::Selector selector({{"Left", nullptr}, {"Right", nullptr}});
 rd::Console console("Console");
 rd::View image_view("Images");*/
 void selector_control(){
  if (master.get_digital_new_press(DIGITAL_DOWN)) {
    selector.next_auton();
    selector.focus();
  }
  if (master.get_digital_new_press(DIGITAL_UP)) {
    selector.prev_auton();
    selector.focus();
  }
}



