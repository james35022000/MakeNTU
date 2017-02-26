#include "mbed.h"
#include "Motor.h"
#include "Timer.h"
#include "Servo.h"
#include "stdlib.h"
#include "cstdio"

Motor M1(D3, D8, D9); // pwm, fwd, rev
Motor M2(D5, D6, D7); // pwm, fwd, rev
Servo servo1(PD_14);
Servo servo2(PB_9);
DigitalOut d1(D13, 1);
DigitalOut d2(D12, 1);
DigitalOut myled(LED1);

Serial BT(PD_5, PD_6);
Serial PC(SERIAL_TX, SERIAL_RX);

char c;

void sendData(const char*);

void RxIRQ()
{
    PC.putc(BT.getc());
    sendData("OK\r\n\0");
    M1.speed(1);
    M2.speed(1);
    wait(2);
    M1.speed(0);
    M2.speed(0);
    /*char c = BT.getc();
    if(c == 'C'){
        PC.printf("%c\r\n", c);
        sendData("OK\r\n\0");
        M1.speed(1);
        M2.speed(1);
        wait(2);
        M1.speed(0);
        M2.speed(0);
    }
    else if(c == 'B') {
        PC.printf("%c\r\n", c);
        sendData("OK\r\n\0");
        M1.speed(-1);
        M2.speed(-1);
        wait(2);
        M1.speed(0);
        M2.speed(0);
    }
    else {
        PC.printf("%c\r\n", c);
    }*/
}

void sendData(const char* cmd)
{
    int i = 0;
    while(cmd[i] != '\0')  BT.putc(cmd[i++]);
}

double DistanceSensor()
{
    Timer timer;
    
    DigitalIn  ECHO(A0); // Distance sensor ECHO
    DigitalOut TRIG(D11, 1);// Distance sensor TRIG
    
    wait(0.3);
    
    TRIG.write(0);
    
    while(ECHO.read() == 0);
    
    timer.reset();
    timer.start();
    
    while(ECHO.read() == 1);
    
    timer.stop();
    
    return (timer.read()/58.0)*1000000;
}

int main()
{
    PC.baud(115200);
    BT.baud(115200);
    BT.attach(&RxIRQ, Serial::RxIrq);
    DigitalIn button(PB_8);
    while(1) {
        myled = !myled;
        
        wait(1);
        char tmp[20];
        sprintf(tmp, "%f\r\n\0", DistanceSensor());
        PC.printf(tmp);
        if(DistanceSensor() < 30.0) {
            servo1 = 1;
            servo2 = 0;
            wait(3);
            servo1 = 0;
            servo2 = 1;
        }
    }
}

