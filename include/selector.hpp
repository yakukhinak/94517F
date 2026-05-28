#pragma once
#include "api.h" 
#include <string>
#include "pros/distance.hpp"
#include "EZ-Template/api.hpp"

void ez_screen_task();
void ez_template_extras();
void selector_control();
void screen_print_tracker(ez::tracking_wheel *tracker, std::string name, int line);
