#include "test.h"
#include "ov7670.h"


void StringPgm(const char * str){
  do{
      while (!(UCSR0A & (1 << UDRE0)));//wait for byte to transmit
      UDR0 = pgm_read_byte_near(str);
      while (!(UCSR0A & (1 << UDRE0)));//wait for byte to transmit
  } while (pgm_read_byte_near(++str));
}

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    
  cli();//disable interrupts
  /* Setup the 8mhz PWM clock 
   * This will be on pin 11*/
  DDRB|=(1<<3);//pin 11
  ASSR &= ~(_BV(EXCLK) | _BV(AS2));
  TCCR2A=(1<<COM2A0)|(1<<WGM21)|(1<<WGM20);
  TCCR2B=(1<<WGM22)|(1<<CS20);
  OCR2A=0;//(F_CPU)/(2*(X+1))
  DDRC&=~15;//low d0-d3 camera
  DDRD&=~252;//d7-d4 and interrupt pins
  _delay_ms(3000);
  //set up twi for 100khz
  TWSR&=~3;//disable prescaler for TWI
  TWBR=72;//set to 100khz
  //enable serial
  UBRR0H=0;
  UBRR0L=1;//0 = 2M baud rate. 1 = 1M baud. 3 = 0.5M. 7 = 250k 207 is 9600 baud rate.
  UCSR0A|=2;//double speed aysnc
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);//Enable receiver and transmitter
  UCSR0C=6;//async 1 stop bit 8bit char no parity bits
  camInit();

}

void loop() {
  // put your main code here, to run repeatedly:
    Serial.println(" Celsius " + REG_GAIN);
    delay(2000); //Delay 2 sec.

}
