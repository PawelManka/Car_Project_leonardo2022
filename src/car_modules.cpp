//
// Created by Admin on 05.08.2022.
//

#include <car_modules.hpp>


void BluetoothModule::read_cmd() {

    while(serial_bt_.available() > 0) //tutaj pyta się programu bluetooth czy coś jest w serialu jeżeli tak to wchodzi do ifa
    {
        char c = (char)serial_bt_.read(); //czyta znak char
        if (c == '\n') { //aplikacja kiedy kończy wysyłanie robi wysyła znak nowej lini
            cmd_[cmdIndex_] = 0; //przyrównuje ostatni do 0 jako koniec zapisu, robi się tak robi wg języka C
            cmdIndex_ = 0;
        } else {
            cmd_[cmdIndex_] = c;
            if (cmdIndex_ < 99) cmdIndex_++; // w przypadku gdy jest miejsce w tablicy przesuwa index,
        }
    }
}

void BluetoothModule::read_device_value() {

    read_cmd();
    if(cmd_[0] == 's'){
        state_value_ = atoi(cmd_ + 2);
    }else if( cmd_[0] == 'v'){
        engine_value_ = atoi(cmd_ + 2); // w funckji atoi() trzeba podać adres początku stringa (w języku c jest to  wskaźnik na pierwszy element tablicy, jeżeli chcemy wskaźnik na trzeci element dodajemy 2)
    }else if( cmd_[0] == 'm'){
        mode_ = 'm';
    }else if( cmd_[0] == 'a'){
        mode_ = 'a';
    }else if( cmd_[0] == 't'){
        mode_ = 't';
        test_engine_value_ = atoi(cmd_ + 2);
    }
    else{
        mode_ = 'n';
    }
//    char bt_text[50] ;
//    sprintf(bt_text, "mode = %c", mode_);
//    bt_print(bt_text);

}

char BluetoothModule::getMode() const {
    return mode_;
}

int BluetoothModule::getEngineValue() const {
    return engine_value_;
}

int BluetoothModule::getStateValue() const {
    return state_value_;
}

int BluetoothModule::getTestEngineValue() const {
    return test_engine_value_;
}

void Engine::engine_setup() {
    pinMode(forward_pin_, OUTPUT);
    pinMode(backward_pin_, OUTPUT);

}

void Engine::upload_parameters() {
    if(direction_ == 'F'){
        analogWrite(forward_pin_, speed_);
        analogWrite(backward_pin_, 0);
    }else if(direction_ == 'B'){
        analogWrite(backward_pin_, speed_);
        analogWrite(forward_pin_, 0);
    }
}



void DistanceSensor::change_distance() {

    int time;
//    int dis;
//    int sum;
    digitalWrite(trig_pin_, LOW);
    delayMicroseconds(2);
    digitalWrite(trig_pin_, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin_, LOW);

    time = pulseIn(echo_pin_, HIGH);


    if((time / 58) > 0){
        distance_ = time / 58;
    }
//    for(int i = 0; i < 4; i++){
//
//        sum += dist_tab[i];
//        dist_tab[i+1] = dist_tab[i];
//
//    }
//    dist_tab[0] = dis;
//    distance_ = int((sum + dis)/5);


}

void DistanceSensor::sensor_setup() {


    pinMode(trig_pin_, OUTPUT); //włączam pina
    pinMode(echo_pin_, INPUT);
}




