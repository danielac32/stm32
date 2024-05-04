
#include <os.h>
void nokia(){
int counter=0;


unsigned char x1=2;
unsigned char y1=2;
unsigned char x2=81;
unsigned char y2=45;
unsigned char fill=0;
unsigned char color=1;


int cc,ii,ss;



nokia_init();
    nokia_contrast(65);
    nokia_gotoxy(0, 0);
    nokia_printstr("nokia5110", 1);
    nokia_refresh();


nokia_box(x1, y1, x2, y2, fill, color);
nokia_circle(6, 6, 2, 1, 1);
nokia_circle(77, 6, 2, 1, 1);
nokia_circle(77, 41, 2, 1, 1);
nokia_circle(6, 41, 2, 1, 1);
nokia_line(2, 11, 10, 11, 1);
nokia_line(73, 11, 81, 11, 1);
nokia_line(2, 36, 10, 36, 1);
nokia_line(73, 36, 81, 36, 1);
nokia_line(11, 45, 11, 2, 1);
nokia_line(72, 45, 72, 2, 1);
nokia_circle(84/2,48/2,20,0,1);
nokia_circle(84/2,48/2,15,0,1);
nokia_circle(84/2,48/2,10,0,1);
nokia_circle(84/2,48/2,5,0,1);
nokia_circle(84/2,48/2,2,0,1);
nokia_refresh();
sleep(1000);
nokia_clean_buffer();
nokia_refresh();
 

    while(1){
        


       
     for(ss=0;ss<2;ss++){
          for(ii=1;ii<40;ii+=4){
              nokia_circle(84/2,48/2,ii,0,1);
              nokia_refresh();
              sleep(20);
           }
           nokia_clean_buffer();
           nokia_refresh();
           for(ii=40;ii>0;ii-=4){
              nokia_circle(84/2,48/2,ii,0,1);
              nokia_refresh();
              sleep(20);
           }
           nokia_clean_buffer();
      }
     for(ss=0;ss<2;ss++){
         for(ii=1;ii<40;ii+=1){
            nokia_circle(84/2,48/2,ii,0,1);
            nokia_refresh();
            sleep(10);
         }
         nokia_clean_buffer();

         for(ii=40;ii>1;ii-=1){
            nokia_circle(84/2,48/2,ii,0,1);
            nokia_refresh();
            sleep(10);
         }
         nokia_clean_buffer();

      }

    }
}
