#define LOG_OUT 1 // use the log output function
#define FHT_N 256 // set to 256 point fht
#define NUML 8
#define PINL 3
#include"FastLED.h"
CRGB leds[NUML];

// 256 - буффер для fht

#include <FHT.h> // include the library

void setup() {
  FastLED.addLeds<WS2812, PINL, GRB>(leds, NUML).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(100);
  Serial.begin(115200); // use the serial port
//  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
}

//ushort notes[] = { // храним герцы нот * 100
 //   1636, 1732, 1835, 1945, 2060, 2183, 2312, 2450, 2596, 2750, 2914, 3087
//}


int i = 0;
int get_max(){
  // узнаем индекс самого большого 
  uint8_t max=0; int ind=-1;
  for(int i=2; i< 128; i++){if(fht_log_out[i] > max){max=fht_log_out[i]; ind=i;}}
  Serial.print(max);
  if(max > 100){
  return ind; }
  return 0;
}

int get_sum(int s, int e){
  int sum = 0;
  for(int i=s; i<e; i++){
    if(fht_log_out[i] > 100){
     sum += fht_log_out[i];}
   } 
  return sum;
 }



void loop() {
  while(1) { 
    cli();  
    for (int i = 0 ; i < FHT_N ; i++) { 
      while(!(ADCSRA & 0x10)); 
      ADCSRA = 0xf5; 
      byte m = ADCL; 
      byte j = ADCH;
      int k = (j << 8) | m; 
      k -= 0x0200; 
      k <<= 6; 
      fht_input[i] = k;
    }
    fht_window(); 
    fht_reorder(); 
    fht_run(); 
    fht_mag_log(); 
    sei();
//    Serial.write(255);
    //Serial.println(300);
    //for(int i=0; i<128; i++){
  //  Serial.print(i);
   // Serial.print(' ');
   // Serial.println((int)fht_log_out[i]); 
   // }
    int jej[] = {get_sum(0, 7) / 7,
    get_sum(5, 14) / 7,
    get_sum(14, 28) / 7,
    get_sum(28, 55) / 15,
    get_sum(55, 100) / 10};
    for(int i=0; i<NUML; i++){
        leds[i] = 0;
        if(i < 5){
          if(jej[i] > 100){
          Serial.println(jej[i]);
          Serial.println(i);
        leds[i] = CHSV((jej[i] + 1) * 40, (jej[i] + 1) * 40, (jej[i] + 1) * 40);}}
  
     }
     FastLED.show();
    
    i = 0;
    delay(10);
  }

}
