#include "mbed.h"
#include "SDFileSystem.h"
#include "HeptaXbee.h"
#include "HeptaCamera_GPS.h"
#include "Hepta9axis.h"
#include "HeptaTemp.h"
#include "HeptaBattery.h"

Serial pc(USBTX,USBRX);
SDFileSystem sd(p5, p6, p7, p8, "sd");
HeptaXbee xbee(p9,p10);
HeptaCamera_GPS cam_gps(p13, p14,p25,p24);
Hepta9axis n_axis(p28,p27,0xD0,0x18);
HeptaTemp temp(p17);
HeptaBattery battery(p16,p26);

DigitalOut myleds[] = {LED1,LED2,LED3,LED4};;

int main()
{
    pc.baud(9600);
    xbee.baud(9600);
    float bt;
    float temper;
    const char* warning;
    char str[100];
    char data[100];
    
    int flag = 0;
    int cmdflag=0;
    int rcmd[25];
    
    for(int i=0;i<10;i++){    
        rcmd[i]=0;
    }
    

    while(1) {
        //battery.power_saving_mode(&flag,&bt);
        //temp.temp_sense(&temper);
        wait(0.5);
        //xbee.printf("flag = %d, bt = %.2f [V], temp = %.1f [C]\r\n",flag,bt,temper);
        xbee.xbee_receive(rcmd,&cmdflag);
        
        
        if (cmdflag == 1) {
            pc.printf("check0: rcmd = %c\r\n",rcmd[0]);
            xbee.printf("Command Get = %c\r\n",rcmd[0]);
            
            if (rcmd[0] == 's') {
                pc.printf("check1: rcmd = %c\r\n",rcmd[0]);
                xbee.putc('a');
                pc.printf("check2: rcmd = %c\r\n",rcmd[0]);
                
                //rcmd = xbee.getc();
                
            }//a
            if (rcmd[0] == 'H') {
                pc.printf("check3: rcmd = %c%c\r\n",rcmd[0],rcmd[1]);
                for(int i=0;i<25;i++){
                    xbee.putc(rcmd[i]);
                }

                xbee.printf("\r\ndata returned!!!\r\n");
            }//b
            
            xbee.initialize();
        }
    }
}
