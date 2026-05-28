#pragma once
#include "api.h" 

void resetAngle(double wall_target_heading);
void resetX(pros::Distance& sensor, double offset, bool negative);
void resetY(pros::Distance& sensor, double offset, bool negative);
