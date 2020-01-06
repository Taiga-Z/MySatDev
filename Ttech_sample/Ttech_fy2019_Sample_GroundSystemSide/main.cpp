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
Hepta9axis n_axis(p28,p27,0x68,0x18);
HeptaTemp temp(p17);
HeptaBattery battery(p16,p26);

DigitalOut myleds[] = {LED1,LED2,LED3,LED4};;

int main()
{
    xbee.baud(9600);

    float ax,ay,az;
    
    int rcmd=0,cmdflag=0;
    char data[10];
    
    myleds[0]=true;
    myleds[1]=false;
    myleds[2]=false;
    myleds[3]=false;
    

    while(1) {
        for(int i=0;i<4;i++){
            myleds[i]=0;
            wait(0.2);
            myleds[i]=1;
            wait(0.2);
        }
        
        xbee.xbee_receive(&rcmd,&cmdflag);
        
        if (cmdflag == 1) {
            
            pc.printf("Ground System Command Get = %d\r\n",rcmd);
            
            if (rcmd == 'a') {
                pc.printf("check1: cmd = %c",rcmd);
                n_axis.sen_acc(&ax,&ay,&az);
//                data[0]='H';
//                for(int i=1;i<10;i++){
//                    data[i]='X';
//                }
//                pc.puts(data);
//                xbee.puts(data);
                xbee.putc('H');
                xbee.printf("ax=%.2f, ay=%.2f, az=%.2\r\n");
                pc.printf("ax=%.2f, ay=%.2f, az=%.2\r\n");
                //xbee.putc(0x0a);
                
            }//a

            
            xbee.initialize();
        }
    }
}
