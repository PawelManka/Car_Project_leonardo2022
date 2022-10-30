//
// Created by Admin on 05.08.2022.
//

#include "car.hpp"

void Car::setup() {


    bt_.bluetooth_setup();
    left_en_.engine_setup();
    right_en_.engine_setup();
    ds_front_.sensor_setup();
    ds_left_.sensor_setup();
    ds_right_.sensor_setup();

    //Mpu setup:
    byte stat = mpu_.begin();
    while(stat!=0){}
    mpu_.calcOffsets();

}

void Car::update_automatic_state() {
    switch (automatic_state_) {
    case 0:
        if(ds_front_.getDistance() < 10){
            automatic_state_ = 1;
        }
        break;
    case 1:
        if(ds_front_.getDistance() < 10){
            automatic_state_ = 1;
        }else{
            automatic_state_ = 0;
        }

    }
}

void Car::drive() {

    bt_.read_device_value();
    char previous_mode_ = mode_;
    mode_ = bt_.getMode();
    ds_front_.change_distance();
    ds_left_.change_distance();
    ds_right_.change_distance();
    mpu_.update();

    if(mode_ == 'm') {

        speed_ = bt_.getEngineValue();
        state_ = bt_.getStateValue();

        if (state_ == 0) {
            forward();
        }
        if (state_ == 1) {
            backward();
        }
        if (state_ == 2) {
            left();
        }
        if (state_ == 3) {

            right();
        }
        if (state_ == 4) {
            stop();
        }

    }else if(mode_ == 'a'){
        if(previous_mode_ != mode_){
            automatic_state_ = 0;
        }else{
            update_automatic_state();
        }


        switch(automatic_state_){
            case 0:
                speed_ = 120;
                forward();

            case 1:
                stop();

        }

//        speed_ = 120;
//        forward();
//
//        if(ds_front_.getDistance() < 10){
//            automatic_state_ = 's';
//            delay(10);
//            ds_front_.change_distance();
//            if(ds_front_.getDistance() < 10){
//                automatic_state_ = 'r';
//                speed_ = 140;
//                right();
//                float old_mpu = mpu_.getAngleZ();
//                while(abs(mpu_.getAngleZ() - old_mpu) < 88){
//                    mpu_.update(); //BUUGG;
//
//                }
//
//                stop();
//                ds_front_.change_distance();
//                if(ds_front_.getDistance() > 10){
//                    speed_ = 120;
//                    forward();
//                    automatic_state_ = 'f'; //c = continue avoiding
//
//                }
//            }
//        }


    }


}

char Car::getMode() const {
    return mode_;
}

void Car::forward() {
    left_en_.set_speed(speed_);
    right_en_.set_speed(speed_);
    left_en_.set_direction('F');
    right_en_.set_direction('F');
    left_en_.upload_parameters();
    right_en_.upload_parameters();
}

void Car::backward() {
    left_en_.set_speed(speed_);
    right_en_.set_speed(speed_);
    left_en_.set_direction('B');
    right_en_.set_direction('B');
    left_en_.upload_parameters();
    right_en_.upload_parameters();
}

void Car::left() {
    left_en_.set_speed(speed_);
    right_en_.set_speed(speed_);
    left_en_.set_direction('B');
    right_en_.set_direction('F');
    left_en_.upload_parameters();
    right_en_.upload_parameters();
}

void Car::right() {
    left_en_.set_speed(speed_);
    right_en_.set_speed(speed_);
    left_en_.set_direction('F');
    right_en_.set_direction('B');
    left_en_.upload_parameters();
    right_en_.upload_parameters();
}


void Car::stop() {
    left_en_.set_speed(0);
    right_en_.set_speed(0);
    left_en_.set_direction('F');
    right_en_.set_direction('F');
    left_en_.upload_parameters();
    right_en_.upload_parameters();
}


