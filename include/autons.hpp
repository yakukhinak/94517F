#pragma once
#include <string>
#include "pros/distance.hpp"
#include "dsr.hpp"
//#include "main.h"

void default_constants();

void pose_get();
void odom_angular_set();
void drive_example();
void turn_example();
void drive_and_turn();
void wait_until_change_speed();
void swing_example();
void motion_chaining();
void combining_movements();
void interfered_example();
void odom_drive_example();
void odom_pure_pursuit_example();
void odom_pure_pursuit_wait_until_example();
void odom_boomerang_example();
void odom_boomerang_injected_pure_pursuit_example();
//void measure_offsets();
void odom_test();
//void resetAngle(double wall_target_heading);
void resetXY(pros::Distance& sensor, std::string axis, std::string side);
void odom_current();
void dsr_test();

