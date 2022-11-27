#ifndef CAR_PROJECT_LEONARDO2022_CAR_HPP
#define CAR_PROJECT_LEONARDO2022_CAR_HPP

#define SAFE_SPACE 10
#include "car_modules.hpp"
#include <MPU6050_light.h>

#define CASE7_AVOID_DEVIATION 5

class Car{
public:
    /**
     * Za pomocą listy inicjalizacyjnej zostały zainicjalizowane pola klasy
     * @param bt
     * @param en
     * @param c_en
     * @param ds
     */
    Car(BluetoothModule& bt, Engine& right_en, Engine& left_en, DistanceSensor& ds_front, DistanceSensor& ds_left, DistanceSensor& ds_right, MPU6050& mpu): bt_(bt), right_en_(right_en), left_en_(left_en), ds_front_(ds_front), ds_left_(ds_left), ds_right_(ds_right), speed_(0), mode_('n'), manual_state_(4), mpu_(mpu) {}

    /**
     * metoda setup korzysta z wszystkich dotychczasowych metód setup z każdego z podzespołów
     */
    void setup();

    bool is_connected(){
        return bt_.available() > 0;
    }
    /**
     * metoda drive jest odpowiedzialna za jazdę, użytkownik za pomocą infterfejsu operatorskiego z telefonu prowdzi
     */
    void drive();
    void update_automatic_state();
    void forward();
    void backward();
    void right();
    void left();
    void stop();
    char getMode() const;
    void autonomous_drive();

    void setSpeed(int speed);

private:
    BluetoothModule& bt_;
    Engine& right_en_;
    Engine& left_en_;
    DistanceSensor& ds_front_;
    DistanceSensor& ds_left_;
    DistanceSensor& ds_right_;
    MPU6050& mpu_;
    int speed_;
    char mode_ = 'm';
    int manual_state_;
    int automatic_state_ = 0;
    float old_angle;
    unsigned long old_time;
    unsigned long actual_time;
    unsigned long time_for_avoiding;
    int avoiding_flag;
    int case_3_4_ds_front;
    int case_6_ds_left;
    int case_5_distance_gained;
    int case_7_flag_1_ds_left;
    int case_8_ds_front;
};

#endif //CAR_PROJECT_LEONARDO2022_CAR_HPP