#include "json/telemetry.h"
#include <config.h>
#include <json/json.h>

#ifdef SENSOR_TILE
#include <stdio.h>
#include "steval_stlcx01v1.h"
char *telemetry_serialize(arrow_device_t *device, sensor_data_t *d) {
  SensorTile *data = (SensorTile *)d;
  printf("--------- stlcx serialize ---------\r\n");
  while ( ! data->sync() ) {
    printf("+++ NO sync +++ \r\n");
    data->reset();
    wait(10);
  }
  printf("+++ sync +++ \r\n");
  data->test();
  JsonNode *_node = json_mkobject();
  json_append_member(_node, TELEMETRY_DEVICE_HID, json_mkstring(device->hid));

  if ( data->hasTemperature() )
    json_append_member(_node, TELEMETRY_TEMPERATURE, json_mknumber(data->getTemperature()));
  if ( data->hasHumidity() )
    json_append_member(_node, TELEMETRY_HUMIDITY, json_mknumber(data->getHumidity()));
  json_append_member(_node, TELEMETRY_BAROMETER, json_mknumber(data->getPressure()));
  SensorAxes_t axes = {0,0,0};
  axes = data->getAcceleration();
  printf("Acc [acc/mg]: %6ld, %6ld, %6ld\r\n", axes.AXIS_X, axes.AXIS_Y, axes.AXIS_Z);
  json_append_member(_node, TELEMETRY_ACCELEROMETER_X, json_mknumber(axes.AXIS_X));
  json_append_member(_node, TELEMETRY_ACCELEROMETER_Y, json_mknumber(axes.AXIS_Y));
  json_append_member(_node, TELEMETRY_ACCELEROMETER_Z, json_mknumber(axes.AXIS_Z));
  axes = data->getAngularVelocity();
  printf("AnVe [gyro/mdps]:   %6ld, %6ld, %6ld\r\n", axes.AXIS_X, axes.AXIS_Y, axes.AXIS_Z);
  json_append_member(_node, TELEMETRY_GYROMETER_X, json_mknumber(axes.AXIS_X));
  json_append_member(_node, TELEMETRY_GYROMETER_Y, json_mknumber(axes.AXIS_Y));
  json_append_member(_node, TELEMETRY_GYROMETER_Z, json_mknumber(axes.AXIS_Z));
  axes = data->getMagneticField();
  printf("LIS3MDL [mag/mgauss]:  %6ld, %6ld, %6ld\r\n", axes.AXIS_X, axes.AXIS_Y, axes.AXIS_Z);
  json_append_member(_node, TELEMETRY_MAGNETOMETER_X, json_mknumber(axes.AXIS_X));
  json_append_member(_node, TELEMETRY_MAGNETOMETER_Y, json_mknumber(axes.AXIS_Y));
  json_append_member(_node, TELEMETRY_MAGNETOMETER_Z, json_mknumber(axes.AXIS_Z));
  char *tmp = json_encode(_node);
  json_delete(_node);
  return tmp;
}
#else
#include "x_nucleo_iks01a1_data.h"
char *telemetry_serialize(arrow_device_t *device, sensor_data_t *d) {
  X_NUCLEO_IKS01A1_data *data = (X_NUCLEO_IKS01A1_data *)d;
  JsonNode *_node = json_mkobject();
  json_append_member(_node, TELEMETRY_DEVICE_HID, json_mkstring(device->hid));
  float value;
//  data->ht_sensor->GetTemperature(&value);
  json_append_member(_node, TELEMETRY_TEMPERATURE, json_mknumber(value));
//  data->pt_sensor->GetTemperature(&value);
  json_append_member(_node, "f|lpsTemperature", json_mknumber(value));
//  data->ht_sensor->GetHumidity(&value);
  json_append_member(_node, TELEMETRY_HUMIDITY, json_mknumber(value));
//  data->pt_sensor->GetPressure(&value);
  json_append_member(_node, TELEMETRY_BAROMETER, json_mknumber(value));
  int32_t axes[] = {0,0,0};
//  data->GetAccelerometer()->Get_X_Axes(axes);
  json_append_member(_node, TELEMETRY_ACCELEROMETER_X, json_mknumber(axes[0]));
  json_append_member(_node, TELEMETRY_ACCELEROMETER_Y, json_mknumber(axes[1]));
  json_append_member(_node, TELEMETRY_ACCELEROMETER_Z, json_mknumber(axes[2]));
//  data->GetGyroscope()->Get_G_Axes(axes);
  json_append_member(_node, TELEMETRY_GYROMETER_X, json_mknumber(axes[0]));
  json_append_member(_node, TELEMETRY_GYROMETER_Y, json_mknumber(axes[1]));
  json_append_member(_node, TELEMETRY_GYROMETER_Z, json_mknumber(axes[2]));
//  data->magnetometer->Get_M_Axes(axes);
  json_append_member(_node, TELEMETRY_MAGNETOMETER_X, json_mknumber(axes[0]));
  json_append_member(_node, TELEMETRY_MAGNETOMETER_Y, json_mknumber(axes[1]));
  json_append_member(_node, TELEMETRY_MAGNETOMETER_Z, json_mknumber(axes[2]));
  char *tmp = json_encode(_node);
  json_delete(_node);
  return tmp;
}
#endif
