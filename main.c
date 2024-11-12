#include <16F877A.h>
#device ADC=10
#include <string.h>
#include <math.h>

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES HS

#use delay(crystal=20000000)

#define LCD_ENABLE_PIN PIN_B2
#define LCD_RW_PIN PIN_B3
#define LCD_RS_PIN PIN_B4

#define LCD_DATA4 PIN_D7
#define LCD_DATA5 PIN_D6
#define LCD_DATA6 PIN_D5
#define LCD_DATA7 PIN_D4

#include <lcd.c>

//void lcd_scroll_left(); //ham con hien thi cuon lcd tu phai sang trai
//void lcd_scroll_right(); //ham con hien thi cuon lcd tu trai sang phai
void MQ2();
void FS();
    int i,done;
    float gas,output;
    
void MQ2()
{
   gas = 0;               
   for(i=0;i<100;i++)
      {
         gas=gas+read_adc();       
         delay_ms(1);
      }
   gas = gas/100;
   gas = (gas*5000)/1024;        
   output = (300+gas*2);
       if(output>=9999) 
          {
            output=9999.99;           
          }//if
       else if(output<9999 && output >=4000) //vuot nguong => canh bao
          {
            output=output;           
          }//if
       else if (output<9999 && output>=3925)           //an toan => KHONG canh bao   
          {          
            output=output/1.5;                                  
          }//else if
       else if (output<9999 && output>=3000)           //an toan => KHONG canh bao   
          { 
            output=output/2;                                  
          }//else if
       else if (output<9999 && output>=2000)           //an toan => KHONG canh bao   
          { 
            output=output/2.5;                       
          }//else if
       else if (output<9999 && output>=1201)           //an toan => KHONG canh bao   
          { 
            output=output/3;                       
          }//else if
       else if (output<9999 && output>=501)           //an toan => KHONG canh bao   
          { 
            output=output/1.5;                                           
          }//else if
       else if (output<9999 && output>=300)           //an toan => KHONG canh bao   
          {             
            output=output;                  
          }//else if
       else    
          { 
            output=300;                      
          }
}

void FS()
{
if(input(PIN_C4)==1)
      {  
         output_low(PIN_D1);  //tat COI
         output_low(PIN_D0);  //tat LED do
         output_high(PIN_B0); //bat LED xanh 
         lcd_gotoxy(1,1); //cot 1, dong 1
         lcd_putc("    NO FIRE!    ");        
         delay_ms(399);
         lcd_gotoxy(1,1); //cot 1, dong 1
         lcd_putc("You're safe here");
         delay_ms(399);
      }
      else
      { 
         output_low(PIN_B0);  //tat LED xanh
         output_high(PIN_D0);//bat LED do
         output_high(PIN_D1);//bat COI
         lcd_gotoxy(1,1); //cot 1, dong 1
         lcd_putc(" FIRE DETECTED! ");                                
      }
}

void main()
{       
           
    set_tris_A(0xFF); //input analog MQ2
    set_tris_C(0xFF); //input digital FS
    set_tris_D(0x00); //output cho LCD & coi (D1), LED D1 (D0)
    set_tris_B(0x00); //output cho LED D0 (B0)

    lcd_init(); //khoi dong lcd
    delay_ms(10);
    
    setup_adc(ADC_CLOCK_DIV_32); //xung clock chia cho 32
    setup_adc_ports(AN0); //thiet lap chan A0 cho ADC    
    set_adc_channel(0); //su dung kenh 0 cua ADC
    delay_us(10);

   while(TRUE)
   {
      {
      
         done=ADC_done();//kiem tra ADC lam viec xong
         while(!done) //neu ADC chua lam viec xong
         {
            done = ADC_done (); //tiep tuc lam viec
         }
      
      {
      
      
      FS();
      MQ2();
      lcd_gotoxy(1,2); //cot 1, dong 1
      printf(lcd_putc,"GAS: %f PPM ",output);    
      
      
      
      }
      }   
   }//while
}//main
