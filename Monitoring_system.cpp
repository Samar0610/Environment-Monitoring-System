#include <Arduino.h>
#include "config.h"
#include "Monitoring_system.h"

float MQ2_sensor_read()
{
  int gas;
  gas= analogRead(MQ2);
  return gas;
}