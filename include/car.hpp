//
// Created by Admin on 05.08.2022.
//

#ifndef CAR_PROJECT_LEONARDO2022_CAR_HPP
#define CAR_PROJECT_LEONARDO2022_CAR_HPP

#include "car_modules.hpp"
#include <MPU6050_light.h>


class Car{
public:
    /**
     * Za pomocą listy inicjalizacyjnej zostały zainicjalizowane pola klasy
     * @param bt
     * @param en
     * @param c_en
     * @param ds
     */
    Car(BluetoothModule& bt, Engine& right_en, Engine& left_en, DistanceSensor& ds_front, DistanceSensor& ds_left, DistanceSensor ds_right, MPU6050& mpu): bt_(bt), right_en_(right_en), left_en_(left_en), ds_front_(ds_front), ds_left_(ds_left), ds_right_(ds_right), speed_(0), mode_('n'), state_(4), mpu_(mpu) {}

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

    void forward();
    void backward();
    void right();
    void left();
    void stop();
    char getMode() const;

private:
    BluetoothModule& bt_;
    Engine& right_en_;
    Engine& left_en_;
    DistanceSensor& ds_front_;
    DistanceSensor& ds_left_;
    DistanceSensor& ds_right_;
    MPU6050& mpu_;
    int speed_;
    char mode_;
    int state_;
    char automatic_mode;
    int time;
};

#endif //CAR_PROJECT_LEONARDO2022_CAR_HPP
