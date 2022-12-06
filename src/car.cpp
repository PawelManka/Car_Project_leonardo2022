//
// Created by Admin on 05.08.2022.
//

#include "car.hpp"
char print_state_tab[170];

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

//    static int case_12_ds_right;
    static int case_7_flag_1_ds_left;
    static int old_ds_left_distance;

    switch (automatic_state_) {
        case 0:
            if(ds_front_.getDistance() < SAFE_SPACE){
                automatic_state_ = 1;
            }
            break;
        case 1:
            if(ds_front_.getDistance() >= SAFE_SPACE)
            {
                automatic_state_ = 0;
            }else
            {
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
            if(abs(mpu_.getAngleZ() - old_angle) > ROTATION_DEGREE){
                automatic_state_ = 5;
                case_3_4_ds_front = ds_front_.getDistance();
            }
            break;
        case 4:
            if(abs(mpu_.getAngleZ() - old_angle) > ROTATION_DEGREE){
                automatic_state_ = 5;
                old_time = millis();
                case_3_4_ds_front = ds_front_.getDistance();
            }
            break;
//        case 11:
//            if(ds_right_.getDistance() > 15 and (abs(case_3_4_ds_front - ds_front_.getDistance())) > 10){
//
//                automatic_state_ = 12;
//                old_angle = mpu_.getAngleZ();
//                case_11_distance_gained = abs(case_3_4_ds_front - ds_front_.getDistance());
//            }
//
//            break;
//        case 12:
//            if(abs(mpu_.getAngleZ() - old_angle) > 88){
//                automatic_state_ = 13;
//                avoiding_flag = 0;
//                case_12_ds_right = ds_right_.getDistance();
//
//            }
//            break;

        case 5:
            if(ds_front_.getDistance() < SAFE_SPACE){
                automatic_state_ = 1;
            }else {
                if (ds_left_.getDistance() > 15 and (abs(case_3_4_ds_front - ds_front_.getDistance()) > 13)) {
                    if (old_ds_left_distance > 15){
                        automatic_state_ = 6;
                        old_angle = mpu_.getAngleZ();
                        case_5_distance_gained = abs(case_3_4_ds_front - ds_front_.getDistance());
                    }
                    old_ds_left_distance = ds_left_.getDistance();
                }
            }
            break;
        case 6:
            if(abs(mpu_.getAngleZ() - old_angle) > ROTATION_DEGREE){
                automatic_state_ = 7;
                avoiding_flag = 0;
                case_6_ds_left = ds_left_.getDistance();
            }
            break;

        case 7:
            if(ds_front_.getDistance() < SAFE_SPACE){
                automatic_state_ = 1;
            }else {
                if ((0 == avoiding_flag) && (CASE7_AVOID_DEVIATION < case_6_ds_left - ds_left_.getDistance())) {
                    if (CASE7_AVOID_DEVIATION < case_6_ds_left - ds_left_.getDistance()) {
                        avoiding_flag = 1;

                        case_7_flag_1_ds_left = ds_left_.getDistance();

                    }

                } else if ((1 == avoiding_flag) &&
                           (CASE7_AVOID_DEVIATION < (ds_left_.getDistance() - case_7_flag_1_ds_left))) {

                    avoiding_flag = 2;

                } else if (avoiding_flag == 2) {
                    automatic_state_ = 8;
                    old_angle = mpu_.getAngleZ();
                    avoiding_flag = 0; // not estimated
                }
            }
            break;
        case 8:
            if(abs(mpu_.getAngleZ() - old_angle) > ROTATION_DEGREE){
                automatic_state_ = 9;
                actual_time = millis();
                case_8_ds_front = ds_front_.getDistance();
            }
            break;
        case 9:

            if( case_8_ds_front - ds_front_.getDistance()  > case_5_distance_gained){
                automatic_state_ = 10;
                old_angle = mpu_.getAngleZ();
            }
            break;
        case 10:
            if(abs(old_angle - mpu_.getAngleZ()) > ROTATION_DEGREE){
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
    char test_string[170];

    if(mode_ == 'm') {

        if(previous_mode_ == 'a' or previous_mode_ == 't' ){
            manual_state_ = 4;
        }
//        sprintf(test_string, "manual_state = %d", manual_state_);
//        bt_.bt_print(test_string);

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

        if(previous_mode_ == 'm'){

            automatic_state_ = 0;
            Print_State();
        }
        else{
            update_automatic_state();
        }
        switch (automatic_state_ ) {
            case 0:
                speed_ = 110;
                forward();

                Print_State();
                break;
            case 1:

                stop();
                Print_State();
                break;
            case 2:
                speed_ = 110;
                right();

                Print_State();
                break;
            case 3:
                speed_ = 110;
                right();
                Print_State();
                break;
            case 4:

                speed_ = 110;
                left();
                Print_State();
                break;
            case 11:
                speed_ = 150;
                forward();
                Print_State();
                break;
            case 12:
                speed_ = 150;
                right();
                Print_State();
                break;
            case 13:
                speed_ = 150;
                forward();
                Print_State();
                break;

            case 5:
                speed_ = 150;
                forward();
                Print_State();
                break;

            case 6:
                speed_ = 110;
                left();
                Print_State();
                break;
            case 7:
                speed_ = 150;
                forward();
                Print_State();
                break;
            case 8:
                speed_ = 110;
                left();
                Print_State();
                break;
            case 9:
                speed_ = 150;
                forward();
                Print_State();
                break;
            case 10:
                speed_ = 110;
                right();
                Print_State();
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

void Car::setSpeed(int speed) {
    speed_ = speed;
}

int Car::getSpeed() const {
    return speed_;
}

int Car::getAutomaticState() const {
    return automatic_state_;
}

void Car::Print_State() {
    sprintf(print_state_tab, "text automatic_state = %d", automatic_state_);
    bt_.bt_print(print_state_tab);
}
