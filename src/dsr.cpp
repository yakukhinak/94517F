#include "main.h"
#include "subsystems.hpp"
#include "dsr.hpp"
#include <math.h>
#include <sys/_intsup.h>
#include <cmath>


double front_right_off_y = -7;//og 3.25
double front_left_off_y = -7;//og 2.25
double front_right_off_x = -3.25;//new
double front_left_off_x = 2.25;//new
double left_off_y = -1.0;
double right_off_y = -1.0;
double left_off_x = 8;//new
double right_off_x = -7.5;//new

//double sensor_distance = 5.5;
double field = 70.0; 




/*Both angle and distacne resets work bad fix fix fix*/
/*on dsr test after fist movement it start going different watch vid you made*/


void resetX(pros::Distance& sensor, double offset, bool negative) {
  double reading = (sensor.get() / 25.4);
  double current_x = chassis.odom_x_get(); 
  //double theta = (chassis.odom_theta_get())*(M_PI/180.0);
  //double blue = std::cos(theta) * reading;
  //double purple = std::cos(theta) * offset;
  //double green = std::sin(theta) * off_y * -1.0;
  //double dist_to_wall = blue + purple;
  double new_x = field - (reading+offset);
  if (negative) {
    new_x = -(field) + (reading+offset);
  }
  else{
    new_x = field - (reading+offset);
  }

  chassis.odom_pose_set({new_x, chassis.odom_y_get(), chassis.odom_theta_get()});
  //Brain.printf("new x: %f\n", new_x);
}


void resetY(pros::Distance& sensor, double offset, bool negative) {
  double reading = (sensor.get() / 25.4);
  double current_y = chassis.odom_y_get();
  double new_y = field - (reading + offset);
  if (negative) {
    new_y = -(field) + (reading+offset);
  }
  else{
    new_y = field - (reading+offset);
  }
  chassis.odom_pose_set({chassis.odom_x_get(), new_y, chassis.odom_theta_get()});
  //Brain.printf("new y: %f\n", new_y);
}

// Helper to convert degrees to radians for math functions
/*double to_rad(double degrees) {
    return degrees * (M_PI / 180.0);
}

void resetX(pros::Distance& sensor, double off_x, double off_y, double wall_angle) {
    // 1. Get raw sensor reading in inches
    double sensor_val = sensor.get() / 25.4; 
    
    // 2. Calculate theta (heading against wall)
    // Note: Ensure your chassis.odom_theta_get() matches your 'heading' convention
    double theta = to_rad(chassis.odom_theta_get() - wall_angle);

    // 3. Apply the 3-part trig logic from your diagram
    double blue = std::cos(theta) * sensor_val;
    double purple = std::cos(theta) * off_x;
    double green = std::sin(theta) * off_y * -1.0;

    double dist_to_wall = blue + purple + green;

    // 4. Update Odom (Assuming field - dist_to_wall if resetting from the far wall)
    double new_x = field - dist_to_wall;
    chassis.odom_pose_set({new_x, chassis.odom_y_get(), chassis.odom_theta_get()});
}*/


/*
for wall target heading, 
0 is the opposite wall of the distance sensor at the start of the match,
 90 is the right wall, 
 180 is the same wall as the distance sensor, 
 and -90 is the left wall
*/
void resetAngle(double wall_target_heading) {
  //Brain.printf("L: %d | R: %d\n", front_left.get(), front_right.get());
  // cauculates the total distance from wall to tracking center
  double reading_left = (front_left.get() / 25.4) + (front_left_off_y);
  double reading_right = (front_right.get() / 25.4) + (front_right_off_y);
  //Brain.printf("got sensor readings\n");

  // get angle at which bot thinks it is
  double current = chassis.imu.get_heading();
  //Brain.printf("got current imu heading\n");

  //cauculate the angle
  //double ang_rad = std::atan2((reading_right - reading_left), sensor_distance);//og:
   double ang_rad = std::atan((reading_right - reading_left) / (front_right_off_x - front_left_off_x));
  //Brain.printf("got angle / did math\n");

  //convert to degrees
  double ang_deg = ang_rad * (180.0 / M_PI);

  double abs_heading = wall_target_heading + ang_deg +180;//og not +180
  

  // correct possible errors - normalize to -180 to 180
  abs_heading = std::fmod(abs_heading, 360.0);
  if (abs_heading > 180.0) abs_heading -= 360.0;
  else if (abs_heading < -180.0) abs_heading += 360.0;
  double error = std::abs(abs_heading - current);
  if (error > 180) error = 360 - error;
  if (error > 0.5) { 
    chassis.imu.set_heading(abs_heading);
  }
  //Brain.printf("error fix\n");
  chassis.odom_theta_set( abs_heading);
  Brain.printf("convertd to degrees: %f\n", abs_heading);
  //Brain.printf("change odom heading\n");
}

/*void resetXY(pros::Distance& sensor, std::string axis, std::string side){
  // 1. Get reading in inches (PROS get() is in mm)
    double raw_mm = sensor.get();
    
    // Optional: Filter out bad readings (2000mm is often the max range)
    if (raw_mm > 2000 || raw_mm <= 0){
      Brain.print("sensor bad\n");
      return;
    }

    double reading = raw_mm / 25.4;
    double current_x = chassis.odom_x_get(); 
    double current_y = chassis.odom_y_get();


    if (axis == "x") {
        double offset = (side == "left") ? left_off_x : right_off_x;
        // Formula assumes robot is facing toward the high-coord wall
        //double new_x = field - (reading + offset); 
        double new_x = reading + offset;
        chassis.odom_pose_set({new_x, current_y, chassis.odom_theta_get()});
    } 
    else if (axis == "y") {
        double offset = (side == "front_left") ? front_left_off_y : front_right_off_y;
        double new_y = field - (reading + offset);
        chassis.odom_pose_set({current_x, new_y, chassis.odom_theta_get()});
    }
}
  /*if (axis == "x") {
    if (side == "left") {
      double reading = (sensor.get() / 25.4) + (left_off);
      double new_x = field - reading;
      chassis.odom_pose_set({new_x, chassis.odom_y_get(), chassis.odom_theta_get()});
    }
    else if (side == "right") {
      double reading = (sensor.get() / 25.4) + (right_off);
      double new_x = field - reading;
      chassis.odom_pose_set({new_x, chassis.odom_y_get(), chassis.odom_theta_get()});
    }
  }
  else if (axis == "y") {
    if (side == "front_left") {
      double reading = (sensor.get() / 25.4) + (front_left_off);
      double new_y = field - reading;
      chassis.odom_pose_set({chassis.odom_x_get(), new_y, chassis.odom_theta_get()});
    }
    else if (side == "front_right") {
      double reading = (sensor.get() / 25.4) + (front_right_off);
      double new_y = field - reading;
      chassis.odom_pose_set({chassis.odom_x_get(), new_y, chassis.odom_theta_get()});
    }
  
}*/


