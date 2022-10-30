//
// Created by Admin on 05.08.2022.
//

#ifndef CAR_PROJECT_LEONARDO2022_CAR_MODULES_HPP
#define CAR_PROJECT_LEONARDO2022_CAR_MODULES_HPP

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <string.h>
#include <Wire.h>
#define INF 20000;

/** BluetoothModule - stworzony do komunikacji z innym urządzenie np telefon i sterowaniem zdalnym
 *  Obsługiwany do aplikacji RoboRemo z dwoma sliderami - osbgługujący skręt samochodu i napięcie zasilania,
 *  dwoma przyciskami - które obsługują biegi samochodu
**/
class BluetoothModule{
public:
    /**
     * Konstruktor który tworzy pola klas i nadaję początkową wartość silników na 0,
     * @param rx_pin - pin reprezentujący rzeczywiste podłączenie w Arduino, rx jest pinem rx od arduino czyli na bluetooth module tx
     * @param tx_pin - pin reprezentujący rzeczywiste podłączenie w Arduino, tx jest pinem rx od arduino czyli na bluetooth module rx
     */
    BluetoothModule(int rx_pin, int tx_pin): rx_pin_(rx_pin), tx_pin_(tx_pin), serial_bt_(SoftwareSerial(rx_pin_, tx_pin_)), mode_('n'), engine_value_(0), state_value_(0) { }

    /**
     * metoda odpowiedzialna za ustawienie urządzenia z płytką arduino
     */
    void bluetooth_setup(){ serial_bt_.begin(9600); }

    /**
     * na początku funkcja wywołuje metodę prywatną read_cmd
     * Funkcja która zczytuje wartości wysyłane przez przyciski lub slidery
     * Wartości są przekazywane jako stringi: "l xxx" gdzie l - to litera opisująca urządzenie, do wyboru: 's', 'e', 'r', 'f', 'n'(opis przy opisie pól)
     * xxx - to wartość zadana
     */
    void read_device_value();
    void bt_print(String str){
        serial_bt_.println(str);

    }

    int available(){
        return serial_bt_.available();
    }

    char getMode() const;

    int getStateValue() const;

    int getEngineValue() const;


    void read_cmd();

private:
    /**
     * metoda jest prywatną żebby zmniejszyć interfejs obiektu BluetoothModule, dodatkowo przy jej refaktoryzacji robię to tylko w obiekcie Bluetooth module
     * metoda ma za zadanie zczytywać znaki i zapisywać je w cmd_, jeżeli zostanie wysłany znak nowej lini
     */

    int& rx_pin_; //pin UART łączenia z arduino
    int& tx_pin_; //pin UART łączenia z arduino rx jest podanawany przez pin 8 (zielony) dla arduino jest to tx
    SoftwareSerial serial_bt_; //serial bt jest to klasa stworzona aby piny inne niż 0/1 mogły się komunikować przez bluetooth, do stworzenia podaje się rx i tx
    //polecenie zczytywane przez bluetooth w metodzie read_device_value
    char cmd_[100];
    int cmdIndex_ = 0;
    char mode_; //przyjmuje 'n' jeżeli nic nie zmienia, 'e' jeżeli zmienia silnik, 's' jeżeli serwomechanizm
    int engine_value_; //zapisywana wartość podawana na silnjik napędu
    int state_value_; //wartość podawana na  silnik kierownica

};

class Engine{
public:
    Engine(char direction, int speed, int forward_pin, int backward_pin) : direction_(direction), speed_(speed), forward_pin_(forward_pin), backward_pin_(backward_pin){}


    void set_direction(char direction){
        direction_ = direction;
    }

    void set_speed(int speed){
        speed_ = speed;
    }

    void engine_setup();

    void upload_parameters();


private:
    char direction_;
    int speed_;
    int forward_pin_;
    int backward_pin_;
};


/**
 * Klasa obsługuję obiekt czujnika odległości, który znajduje się na początku
 */
class DistanceSensor{
public:
    DistanceSensor(int trig_pin, int echo_pin): trig_pin_(trig_pin), echo_pin_(echo_pin){}

    void sensor_setup();

    void change_distance();

    int getDistance() const {
        return distance_;
    };


private:
    int trig_pin_;
    int echo_pin_;
    int distance_ = INF; // to może nie działać

};


#endif //CAR_PROJECT_LEONARDO2022_CAR_MODULES_HPP
