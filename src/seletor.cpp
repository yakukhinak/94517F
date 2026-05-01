/*#include "main.h"
#include "pros/misc.h"
#include "robodash/views/console.hpp"
#include "main.cpp"



void ez_screen_task() {
 while (true) {
    if (!pros::competition::is_connected()) {
      Odom.clear();
      Odom.printf("x: %.2f\ny: %.2f\na: %.2f\n", 
                     chassis.odom_x_get(), 
                     chassis.odom_y_get(), 
                     chassis.odom_theta_get());
    }
    pros::delay(20);
  }
}

void ez_template_extras() {
  if (!pros::competition::is_connected() && !driver) {
    // PID Tuner
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

void selector_control(){
  if (master.get_digital_new_press(DIGITAL_R1)) {
    selector.next_auton();
    selector.focus();
  }
  if (master.get_digital_new_press(DIGITAL_R2)) {
    selector.prev_auton();
    selector.focus();
  }
}

void Distance_print() {
  while (true) {
    if (!pros::competition::is_connected() && master.get_digital(DIGITAL_UP)) {
      Distance.clear();
      Distance.printf("Distance front_right: %.2f mm \n", front_right.get());
      Distance.printf("Distance front_left: %.2f mm \n", front_left.get());
      Distance.printf("Distance left_s: %.2f mm \n", left_s.get());
      Distance.printf("Distance right_s: %.2f mm \n", right_s.get());
    }
      pros::delay(20);
}
}*/