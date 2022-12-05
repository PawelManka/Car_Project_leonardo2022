#include <Arduino.h>
#include <SoftwareSerial.h>
#include "car.hpp"

Engine left_en('F', 0, 5, 6);
Engine right_en('F', 0, 10, 11);
BluetoothModule bt(9, 8);
DistanceSensor ds_f(4, 7);
DistanceSensor ds_l(12, 13);
DistanceSensor ds_r(0, 1);
MPU6050 mpu(Wire);

unsigned long start_testing = 0;
unsigned long start_measure = 0;


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
void map_pwm_to_velocity();
void Bt_Map_Area();

void setup() {

    delay(500);
    car.setup();
    Serial.begin(9600);

}

void loop() {

    car.drive();
    if( car.getMode() == 'a'){

        if((millis()-myTime) > 50) {
            if( 0 == start_measure ){
                start_measure = millis();
            }
            Bt_Map_Area();
            myTime = millis();
        }

    }else if(car.getMode() == 't'){
        if (start_testing == 0){
            start_testing = millis();
        }
        map_pwm_to_velocity();
    }

}

void serial_print_sensors(){
/* print with USB */
//        x_pos = mpu.getAngleX();
//        y_pos = mpu.getAngleY();
//        z_pos = mpu.getAngleZ();
//        Serial.print("Ds = ");
//        Serial.print(ds_f.getDistance());
//        Serial.print(" x_pos = ");
//        Serial.print(x_pos);
//        Serial.print(" y_pos = ");
//        Serial.print(y_pos);
//        Serial.print(" z_pos = ");
//        Serial.println(z_pos);

    ds_val_f = ds_f.getDistance();
    ds_val_l = ds_l.getDistance();
    ds_val_r = ds_r.getDistance();

    sprintf(bt_string, "text z_pos=%d, ds_f=%d, ds_l=%d, ds_r=%d", z_pos, ds_val_f, ds_val_l, ds_val_r);
    bt.bt_print(bt_string);
}

void Bt_Map_Area(){
    sprintf(bt_string, "%d %d %d %d %d %ld %d",  ds_f.getDistance(), ds_l.getDistance(), ds_r.getDistance(), int(mpu.getAngleZ()), car.getSpeed(), (millis() - start_measure), car.getAutomaticState());
    bt.bt_print(bt_string);
}

void map_pwm_to_velocity(){

    if( ds_f.getDistance() > 10){
        if((millis()-myTime) > 10){
            car.setSpeed(bt.getTestEngineValue());
            car.forward();

            sprintf(bt_string, "%d %ld",  ds_f.getDistance(), (millis() - start_testing));
            bt.bt_print(bt_string);
            myTime = millis();
        }

    } else{
        car.stop();
    }

}
