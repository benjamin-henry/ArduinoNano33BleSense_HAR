#include <threadedtimer.h>
#include <Arduino_LSM9DS1.h>
#include <lpc.h>

ThreadedTimer thread_axl;
int task_axl;

int axl_fs = 20; //Hz or 50ms
int fc = 10;

LPC *xfilter;
LPC *yfilter;
LPC *zfilter;

int start = 0;

void axl_tick() {
  float x, y, z;
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
    xfilter->filter(x);
    yfilter->filter(y);
    zfilter->filter(z);
    int now = millis();
    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);
    Serial.println(now-start);
    start = now;
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  while (!IMU.begin());
  task_axl = thread_axl.every(int(1000 / axl_fs), axl_tick);
  xfilter = new LPC(axl_fs, 5);
  yfilter = new LPC(axl_fs, 5);
  zfilter = new LPC(axl_fs, 5);
}

void loop() {

}
