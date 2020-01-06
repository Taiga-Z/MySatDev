#include "mbed.h"
#include "HeptaXbee.h"

DigitalOut myled(LED1);
Serial pc(USBTX,USBRX);
HeptaXbee xbee(p9,p10);
Serial com(p13,p14);

SPI spi_adc(p5, p6, p7); // mosi, miso, sclk
DigitalOut cs_adc(p8);
//SPISlave spi_com(p11, p12, p13, p14); // mosi, miso, sclk, ssel




char rx_data;
char rcmd[100];
char securityCode[3], exectime[10][7], APID[10][2], userID[10][4], length[10][3], parameter[10][100];
char USERID[4];

int cmdflag = 0;
int Length;


void CommandGet()
{
//    // from keyboard
//    int i=0;
//    do{
//        rx_data =pc.getc();
//        if(rx_data!='\r'){
//            xbee.putc(rx_data);
//            rcmd[i]=rx_data;
//            i++;
//        }
//    }while(rx_data!='\r');
//    //pc.printf("\r\n%d\r\n",sizeof rcmd/sizeof rcmd[0]);

    // from GS Software
    pc.gets(rcmd,100);

    cmdflag = 1;
}

void Initialize()
{
    for(int i=0; i<100; i++) {
        rcmd[i]='\0';
    }
    for(int i=0; i<3; i++) {
        securityCode[i]='\0';
    }
    for(int i=0; i<10; i++) {
        for(int j=0; j<7; j++) {
            exectime[i][j]='\0';
        }
        for(int j=0; j<2; j++) {
            APID[i][j]='\0';
        }
        for(int j=0; j<4; j++) {
            userID[i][j]='\0';
        }
        for(int j=0; j<3; j++) {
            length[i][j]='\0';
        }
        for(int j=0; j<100; j++) {
            parameter[i][j]='\0';
        }
    }
    cmdflag = 0;
}

int main()
{
    pc.printf ( "-----C&DH-----\r\n" );

    pc.baud(9600);
    xbee.baud(9600);

    cs_adc = 1;
    spi_adc.format(8,0);
    spi_adc.frequency(1000000);
    //spi_com.format(8,0);
    //spi_com.frequency(1000000);

    while(1) {

        // Nomal Mode

        myled = 1;
        wait(0.2);
        myled = 0;
        wait(0.2);

        // Interrupt
        pc.attach(CommandGet,Serial::RxIrq);

        if(cmdflag == 1) {
            //pc.printf("\r\nrcmd: %s\r\n",rcmd);
//            e.g.
//                  A80000001222054444455555567770A0123456789
//                  A800000012220A01234567895555556777054444400000012220544444

            //for set command
            int i=0,j=0,k=0;
            for(j=0; j<2; j++) {
                securityCode[j] = rcmd[j+k];
            }
            k=j+k;
            do {
                for(j=0; j<6; j++) {
                    exectime[i][j] = rcmd[j+k];
                }
                k=j+k;

                for(j=0; j<1; j++) {
                    APID[i][j] = rcmd[j+k];
                }
                k=j+k;

                for(j=0; j<3; j++) {
                    userID[i][j] = rcmd[j+k];
                }
                k=j+k;

                for(j=0; j<2; j++) {
                    length[i][j] = rcmd[j+k];
                }
                k=j+k;

                sscanf(length[i],"%x",&Length);

                //pc.printf("Length=%d\r\n",Length);

                for(j=0; j<Length; j++) {
                    parameter[i][j] = rcmd[j+k];
                }

                k=j+k;

                i++;
                pc.printf("number of cmd : %d\r\n",i);
            } while(rcmd[k+2]!='\0');


            pc.printf("Security Code:%s",securityCode);

            if(!strcmp(securityCode,"A8")) {
                pc.printf(" --> OK\r\n");

                for(int ii=0; ii<i; ii++) {
                    pc.printf("%s\r\n",exectime[ii]);
                    pc.printf("%s\r\n",APID[ii]);
                    pc.printf("%s\r\n",userID[ii]);
                    pc.printf("%s\r\n",length[ii]);
                    pc.printf("%s\r\n",parameter[ii]);
                }

                for(int ii=0; ii<i; ii++) {
                    for(int jj=0; jj<4; jj++) {
                        USERID[jj] = userID[ii][jj];
                    }
                    //com.puts(USERID);
                    

                    if(!strcmp(USERID,"222")) {
                        pc.printf("execute command %s\r\n",USERID);
                        pc.printf("parameter: %s\r\n",parameter[ii]);
                    }

                    if(!strcmp(USERID,"777")) {
                        pc.printf("execute command %s\r\n",USERID);
                        pc.printf("parameter: %s\r\n",parameter[ii]);
                    }


                    if(!strcmp(USERID,"551")) {
                        pc.printf("execute command %s\r\n",USERID);
                        pc.printf("ADC Accelation Sensing\r\n");

                        cs_adc=0;
                        wait(0.01);
                        com.printf("551\n");
                        spi_adc.write(0xa0);
//                        for(int i=0;i<3;i++){
//                            int com_1 = spi_adc.write(USERID[i]);
//                        }
                        wait(0.01);
                        cs_adc=1;
                    }

                }

            }
            //wait(0.5);
            Initialize();
        }

    }
}
