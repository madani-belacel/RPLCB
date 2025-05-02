#ifndef LIFETIME_H_
#define LIFETIME_H_

#include "project-conf.h"

void init_lifetime();
void update_lifetime(double consumption);
int is_battery_depleted();
double get_remaining_battery();

#endif /* LIFETIME_H_ */