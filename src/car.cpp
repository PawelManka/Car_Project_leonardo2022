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
            if(ds_front_.getDistance() < SAFE_SPACE){
                automatic_state_ = 1;
            }
            break;
        case 1:
            if(ds_front_.getDistance() >= SAFE_SPACE){
                automatic_state_ = 0;
            }else{

                if(ds_right_.getDistance() > SAFE_SPACE or ds_left_.getDistance() > SAFE_SPACE){
                    old_time = millis();
                    if(ds_right_.getDistance() > ds_left_.getDistance()){
                        automatic_state_ = 3;
                        old_angle = mpu_.getAngleZ();
                        old_time = millis();
                    }else{
                        automatic_state_ = 4;
                        old_angle = mpu_.getAngleZ();

                    }
                }else{
                    automatic_state_ = 2;
                    old_angle = mpu_.getAngleZ();
                }
            }
            break;
        case 2:
            if(abs(mpu_.getAngleZ() - old_angle) > 178){
                automatic_state_ = 0;
            }else{
                //TODO wykrywanie bledow

            }
            break;

        case 3:
            if(abs(mpu_.getAngleZ() - old_angle) > 85){
                automatic_state_ = 5;

            }
            break;
        case 4:
            if(abs(mpu_.getAngleZ() - old_angle) > 85){
                automatic_state_ = 5;
                old_time = millis();
            }
            break;
        case 5:
            if(ds_left_.getDistance() > 10){
                if(ds_left_.getDistance() > 10 and abs(millis() - old_time) > 1500){

                    automatic_state_ = 6;
                    old_angle = mpu_.getAngleZ();
                }
            }
            break;
        case 6:
            if(abs(mpu_.getAngleZ() - old_angle) > 88){
                automatic_state_ = 7;
                avoiding_flag = 0;
            }
            break;
        case 7:
            if((avoiding_flag == 0) && (ds_left_.getDistance() < 10)){
                if(ds_left_.getDistance() < 10){
                    avoiding_flag = 1;
                    bt_.bt_print("avoiding_flag1");
                }

            }else if((1 == avoiding_flag) && (ds_left_.getDistance() > 10)){
                if(ds_left_.getDistance() > 10){

                    bt_.bt_print("avoiding_flag1");
                }

            }else if(avoiding_flag == 2){
                automatic_state_ = 8;
                old_angle = mpu_.getAngleZ();
                avoiding_flag = 0; // not estimated
            }
            break;
        case 8:
            if(abs(mpu_.getAngleZ() - old_angle) > 85){
                automatic_state_ = 9;
                actual_time = millis();
            }
            break;
        case 9:

            if(abs(actual_time - millis()) > 2000){
                automatic_state_ = 10;
                old_angle = mpu_.getAngleZ();
            }
            break;
        case 10:
            if(abs(old_angle - mpu_.getAngleZ()) > 85){
                automatic_state_ = 0;
            }


//    case 20:
//        if(abs(mpu_.getAngleZ() - old_angle) > 88){
//            automatic_state_ = 5;
//        }
//        break;
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

        if(previous_mode_ == 'a'){
            manual_state_ = 4;
        }
        //TODO po zmianie z a ma sie zatrzymywac!
        speed_ = bt_.getEngineValue();
        manual_state_ = bt_.getStateValue();

        if (manual_state_ == 0) {
            forward();
        }
        if (manual_state_ == 1) {
            backward();
        }
        if (manual_state_ == 2) {
            left();
        }
        if (manual_state_ == 3) {

            right();
        }
        if (manual_state_ == 4) {
            stop();
        }

    }else if(mode_ == 'a'){

        char test_string[170];
        if(previous_mode_ == 'm'){

            automatic_state_ = 0;
            sprintf(test_string, "text automatic_state = %d", automatic_state_);
            bt_.bt_print(test_string);

        }else{
            update_automatic_state();
        }

        switch (automatic_state_ ) {
        case 0:
            speed_ = 120;
            forward();

            sprintf(test_string, "FORWARD text automatic_state = %d", automatic_state_);
            bt_.bt_print(test_string);
            break;
        case 1:

            stop();
            sprintf(test_string, "STOP text automatic_state = %d", automatic_state_);
            bt_.bt_print(test_string);
            break;
        case 2:
            speed_ = 140;
            right();

            sprintf(test_string, "text automatic_state = %d", automatic_state_);
            bt_.bt_print(test_string);
            break;
        case 3:
            speed_ = 180;
            right();

            sprintf(test_string, "text automatic_state = %d", automatic_state_);
            bt_.bt_print(test_string);
            break;
        case 4:

            speed_ = 150;
            left();

            sprintf(test_string, "text automatic_state = %d", automatic_state_);
            bt_.bt_print(test_string);
            break;
        case 5:
            speed_ = 150;
            forward();

            sprintf(test_string, "text automatic_state = %d", automatic_state_);
            bt_.bt_print(test_string);
            break;

        case 6:
            speed_ = 150;
            left();

            sprintf(test_string, "text automatic_state = %d", automatic_state_);
            bt_.bt_print(test_string);
            break;
        case 7:
            speed_ = 140;
            forward();
            sprintf(test_string, "text automatic_state = %d", automatic_state_);
            bt_.bt_print(test_string);
            break;
        case 8:
            speed_ = 150;
            left();
            sprintf(test_string, "text automatic_state = %d", automatic_state_);
            bt_.bt_print(test_string);
            break;
        case 9:
            sprintf(test_string, "text automatic_state = %d", automatic_state_);
            bt_.bt_print(test_string);
            break;
        case 10:
            speed_ = 150;
            right();

            sprintf(test_string, "text automatic_state = %d", automatic_state_);
            bt_.bt_print(test_string);
            break;
        case 20:
            speed_ = 220;
            right();

            sprintf(test_string, "text automatic_state = %d", automatic_state_);
            bt_.bt_print(test_string);
            break;
        }
    }
}

void Car::autonomous_drive() {


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


