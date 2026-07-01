#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"
#include "pros/distance.hpp"
#include "EZ-Template/piston.hpp"
#include "pros/motors.h"

extern Drive chassis;

inline pros::Motor intake(1);
inline pros::MotorGroup score({2, 3});
inline ez::Piston piston('A');

// Your motors, sensors, etc. should go here.  Below are examples
inline pros::Distance front_right(20);
inline pros::Distance front_left(12);
inline pros::Distance left_s(3);
inline pros::Distance right_s(17);


// inline pros::Motor intake(1);
// inline pros::adi::DigitalIn limit_switch('A');