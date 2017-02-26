#include "mbed.h"

//------------------------------------
// Hyperterminal configuration
// 9600 bauds, 8-bit data, no parity
//------------------------------------

Serial BT(PD_5, PD_6);
Serial PC(SERIAL_TX, SERIAL_RX);

DigitalOut myled(LED1);

void RxIRQ()
{
    PC.putc(BT.getc());
}

void TxIRQ()
{
    
}

void sendData(const char* cmd)
{
    int i = 0;
    while(cmd[i] != '\0')  BT.putc(cmd[i++]);
}

int main()
{
    PC.baud(115200);
    BT.baud(115200);
    BT.attach(&RxIRQ, Serial::RxIrq);
    DigitalIn button(PB_8);
    char cmd[20];
    bool flag = true;
    while(1) {
        /*if(PC.readable()) {
            char c = PC.getc();
            if(c == 0xD) {
                PC.printf("\r\n\0");
                strcat(cmd, "\r\n\0");
                sendData(cmd);
                cmd[0] = '\0';
            }
            else {
                int len = strlen(cmd);
                cmd[len] = c;
                cmd[len + 1] = '\0';
                PC.putc(c);
            }
        }*/
        if(!button) {
            if(flag) {
                sendData("C\r\n\0");
                flag = !flag;
                while(!button);
            }
            else {
                sendData("B\r\n\0");
                flag = !flag;
                while(!button);
            }
        }
    }
}
