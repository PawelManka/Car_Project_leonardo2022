#include <Arduino.h>
#include <SoftwareSerial.h>
#include "car_modules.hpp"
#include "car.hpp"

Engine left_en('F', 0, 5, 6);
Engine right_en('F', 0, 10, 11);
BluetoothModule bt(9, 8);
DistanceSensor ds_f(4, 7);
DistanceSensor ds_l(12, 13);
DistanceSensor ds_r(0, 1);
MPU6050 mpu(Wire);

Car car(bt, left_en, right_en, ds_f, ds_l,ds_r, mpu);
unsigned long myTime;
char bt_string[170];
int16_t x_pos;
int16_t y_pos;
int16_t z_pos;
int ds_val_f;
int ds_val_r;
int ds_val_l;

void serial_print_sensors();


void setup() {

    delay(500);
    car.setup();
    Serial.begin(9600);

}


void loop() {


    car.drive();
    if( car.getMode() == 'a'){
        if((millis()-myTime) > 100) {
            serial_print_sensors();
            myTime = millis();
        }
    }
//    serial_print_sensors();

}

void serial_print_sensors(){

        x_pos = mpu.getAngleX();
        y_pos = mpu.getAngleY();
        z_pos = mpu.getAngleZ();


        Serial.print("Ds = ");
        Serial.print(ds_f.getDistance());
        Serial.print(" x_pos = ");
        Serial.print(x_pos);
        Serial.print(" y_pos = ");
        Serial.print(y_pos);
        Serial.print(" z_pos = ");
        Serial.println(z_pos);

        ds_val_f = ds_f.getDistance();
        ds_val_l = ds_l.getDistance();
        ds_val_r = ds_r.getDistance();
        sprintf(bt_string, "text z_pos=%d, ds_f=%d, ds_l=%d, ds_r=%d", z_pos, ds_val_f, ds_val_l, ds_val_r);
        bt.bt_print(bt_string);

}


