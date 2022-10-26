#include <Arduino.h>
#include <SoftwareSerial.h>
#include "car_modules.hpp"
#include "car.hpp"

Engine left_en('F', 0, 5, 6);
Engine right_en('F', 0, 10, 11);
BluetoothModule bt(9, 8);
DistanceSensor ds(4, 7);
DistanceSensor ds2(12,13);
MPU6050 mpu(Wire);

Car car(bt, left_en, right_en, ds,ds2, mpu);
unsigned long myTime;
char bt_string[170];
int16_t x_pos;
int16_t y_pos;
int16_t z_pos;
int ds_val;
int ds2_val;

void serial_print_sensors();


void setup() {

    delay(500);
    car.setup();
    Serial.begin(9600);
    ds2.sensor_setup();

}


void loop() {
//    if (bt_.available()>0) { //wait for data received
//        bt_.read_device_value();
//        Serial.print("State is: ");
//        Serial.println(bt_.getStateValue());
//
//        Serial.print("Engine value is: ");
//        Serial.println(bt_.getEngineValue());
//    }
//
//
//    delay(100);
//    if(car.is_connected()){
//        car.drive();
//    }
//    if (Serial.available()>0){
//        ds.change_distance();
//
//    }


    car.drive();
    if( car.getMode() == 'a'){
        if((millis()-myTime) > 100) {
            serial_print_sensors();
            myTime = millis();
        }
    }
//    serial_print_sensors();
//    myTime = millis();

}

void serial_print_sensors(){

        x_pos = mpu.getAngleX();
        y_pos = mpu.getAngleY();
        z_pos = mpu.getAngleZ();


        Serial.print("Ds = ");
        Serial.print(ds.getDistance());
        Serial.print(" x_pos = ");
        Serial.print(x_pos);
        Serial.print(" y_pos = ");
        Serial.print(y_pos);
        Serial.print(" z_pos = ");
        Serial.println(z_pos);

        ds_val = ds.getDistance();
        ds2_val = ds2.getDistance();
        sprintf(bt_string, "text z_pos=%d, ds=%d, ds2=%d", z_pos, ds_val, ds2_val);
        bt.bt_print(bt_string);




}

//            String str2("VELOCITY distance is: ");
//            String str1(ds_front_.getDistance());
//            String str3 = str2 + str1;
//            bt_.bt_print(str3);



/*-----------------------------------------------------------------
 * ---------------------------------------------------------------
 */
//#include "Wire.h"
//#include <MPU6050_light.h>
//
//MPU6050 mpu(Wire);
//unsigned long timer = 0;
//
//void setup() {
//    Serial.begin(9600);
//    Wire.begin();
//
//    byte status = mpu.begin();
//    Serial.print(F("MPU6050 status: "));
//    Serial.println(status);
//    while(status!=0){ } // stop everything if could not connect to MPU6050
//
//    Serial.println(F("Calculating offsets, do not move MPU6050"));
//    delay(1000);
//    // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
//    mpu.calcOffsets(); // gyro and accelero
//    Serial.println("Done!\n");
//}
//
//void loop() {
//    mpu.update();
//
//    if((millis()-timer)>10){ // print data every 10ms
//        Serial.print("X : ");
//        Serial.print(mpu.getAngleX());
//        Serial.print("\tY : ");
//        Serial.print(mpu.getAngleY());
//        Serial.print("\tZ : ");
//        Serial.println(mpu.getAngleZ());
//        timer = millis();
//    }
//}