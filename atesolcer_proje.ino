/*
 Bu proje Orhan Akgün tarafından SFSLM kanalı için yapılmıştır.

kullanılan Malzemeler 
*******************************
1.8" TFT 128X160 St7735 DISPLAY
MLX90614 SENSOR
IR  MESAFE SENSÖRÜ
BUZER
İKİ RENKLİ LED
100 Ohm DİRENÇ 2 ADET
ARDUINO UNO
******************************

 1.8" TFT 128x160 SPI ST7735 display ile Arduino Bağlantıları

ST7735 TFT SPI display pin      Arduino Pin
 * LED                       =   3.3V veya 100 Ohm 5 volt
 * SCK                       =   13
 * SDA                       =   11
 * A0                        =    8
 * RESET                     =    9
 * CS                        =   10
 * GND                       =   GND
 * VCC                       =   5V

Kullanılan Bazı renkler:
 * BLACK    0x0000
 * BLUE     0x001F
 * RED      0xF800
 * GREEN    0x07E0
 * CYAN     0x07FF
 * MAGENTA  0xF81F
 * YELLOW   0xFFE0
 * WHITE    0xFFFF

MLX90614        Arduino
 VIN            5V         
 GND            GND
 SCL            A5
 SDA            A4
*/


#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Fonts/FreeMonoBoldOblique9pt7b.h>//GFX kütüphanesi içinde
#include <Fonts/FreeSerifBold9pt7b.h>//GFX kütüphanesi içinde


// Pin bağlantıları
#define TFT_CS     10
#define TFT_RST    8 
#define TFT_DC     9


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();  

int sensor = 7;
int buzzerPin=6;  
int j=0;
int YESIL=2;
int KIRMIZI=4;
int k=0;

float okuma[30];
float toplam ;
float sicaklik ;

unsigned long eskiZaman=0; 
unsigned long simdikiZaman;

void setup() {
  mlx.begin(); // MLX90614 sensörü başlat 
  
pinMode(YESIL,OUTPUT);    
pinMode(KIRMIZI,OUTPUT); 
pinMode(buzzerPin,OUTPUT); 
pinMode(sensor,INPUT); 


// Use this initializer if you're using a 1.8" TFT
  tft.initR(INITR_BLACKTAB);   // ST7735 Chip başlat 
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(1);   // ekran döndürmek için ....0-->0 derece ,1-->90 derece, 2-->180 derece, 3-->270 
  tft.fillScreen(0xF800); //  ekran arkaplan rengi 16 bit
  tft.fillRoundRect(5,5,150,25,5,0xFFE0);// yuvarlakkenarlı dikdörtgen doldur.
  tft.drawRoundRect(5, 5, 150, 25, 5,ST7735_BLACK );// yuvarlakkenarlı dikdörtgen çiz.
  tft.fillRoundRect(5,35,150,88,5,0xFFE0);// yuvarlakkenarlı dikdörtgen doldur.
  tft.drawRoundRect(5, 35, 150, 88, 5,ST7735_BLACK );// yuvarlakkenarlı dikdörtgen çiz.
  tft.setFont(&FreeMonoBoldOblique9pt7b);// yazı fontu
  testdrawtext(50, 22, "SFSLM",ST7735_BLACK);// x,y,metin, metin rengi(16bit)
  tft.setFont(&FreeSerifBold9pt7b);//yazı fontu ayarla
  testdrawtext(30, 55, "Non-Contact", ST7735_BLACK);//ekranda belirtilen x,y başlangıçından yaz.
  testdrawtext(25, 75, "Thermometer", ST7735_BLACK);//ekranda belirtilen x,y başlangıçından yaz.
  testdrawtext(35, 95, "V2 01/2021",ST7735_BLACK); //ekranda belirtilen x,y başlangıçından yaz.
  
  tft.drawRoundRect(50,105,80,8,10,ST7735_BLACK );//termometre uç kısmı ciz
  tft.fillCircle(122, 108, 10, ST7735_RED);//termometre yuvarlak kısmı
  tft.drawCircle(122, 108, 10, ST7735_BLACK);
  tft.fillRoundRect(95,106,20,6,0,ST7735_RED );
  tft.drawLine(110,118, 110, 112,ST7735_BLACK); //x0,y0,x1,y1,color
  tft.drawLine(105,116, 105, 112,ST7735_BLACK);//  termometredeki skalalar
  tft.drawLine(100,116, 100, 112,ST7735_BLACK);
  tft.drawLine(95,116, 95, 112,ST7735_BLACK);
  tft.drawLine(90,116, 90, 112,ST7735_BLACK);
  tft.drawLine(85,118, 85, 112,ST7735_BLACK);
  tft.drawLine(80,116, 80, 112,ST7735_BLACK);
  tft.drawLine(75,116, 75, 112,ST7735_BLACK);
  tft.drawLine(70,116, 70, 112,ST7735_BLACK);
  tft.drawLine(65,116, 65, 112,ST7735_BLACK);
  tft.drawLine(60,118, 60, 112,ST7735_BLACK);
  for(int i=0;i<=35;i++){                       // termometreyi yükselt
                          tft.fillRoundRect(95-i,106,20,6,0,ST7735_RED );
                          delay(20);
                        }
                        delay(1000);
  for(int i=0;i<=30;i++){                       // termometreyi düşür
                          tft.fillRoundRect(55+i,106,20,6,0,0xFFE0 );
                          delay(20);
                        }
 delay(4000); 

//tft.invertDisplay(true);//ekran rengini ters çevir.
//  delay(2000);
//  tft.invertDisplay(false);
//  delay(2000);

    
     tft.fillRect(20, 42, 120, 16,0xFFE0);//ekranı silmek için eski yazının üstünü renkle doldur.
     tft.setFont(&FreeMonoBoldOblique9pt7b);// yazı fontu
     testdrawtext(38, 55, "Elinizi", ST7735_BLACK);// yazı yazdırma fonksiyonunu çağır.
     tft.fillRect(23, 62, 120, 36,0xFFE0 );
     testdrawtext(37, 75, "Sensore", ST7735_BLACK);
     testdrawtext(9, 95, "Yaklastiriniz",ST7735_BLACK);
                                       
}

void loop() {
  
unsigned long simdikiZaman = millis(); //arka planda süre tut.

if(digitalRead(sensor)==0){   // elimiz sensöre yaklaştığında 
               delay(500);    // yaklaşmayı tamamlamak için zaman       
                
   toplam=0;                    // Sensor  hassas okumak için ortalama alma
          for (int i=0; i<30; i++)
          {
          float deger =mlx.readObjectTempC();
          okuma[i]= deger *1;
          toplam = toplam + okuma[i];
          delay (5);
          }
          sicaklik = toplam / 30.0;
          
                            for(int i=0;i<2;i++){// okumanın bittiğini belirtmek için buzer sesi.
                            tone(buzzerPin,494);
                            delay(100);
                            noTone(buzzerPin);
                             delay(100);
                                 }
                           
                            tft.fillRect(20, 42, 120, 16,0xFFE0);//ekranı temizlemek için doldur.
                             tft.setTextSize(1);
                            testdrawtext(38, 55, "ATESINIZ", ST7735_BLACK);
                            tft.fillRect(9, 62, 145, 36,0xFFE0 );
                            tft.setTextSize(2);
                            tft.setCursor(20,85);
                            tft.println(sicaklik+7,1);
                            tft.setCursor(110,85);
                            tft.println("C");
                            tft.drawCircle(140,60, 3, ST7735_BLACK);//derece isareti için daire
                            tft.drawCircle(140,60, 4, ST7735_BLACK);//kalın görünmesi için üstüne ikinci daire

                            if(sicaklik>=30){ digitalWrite(KIRMIZI,HIGH);//sensörden okunan Ham değer 30 ve üstü ise kırmızı led
                                             
                            }
                            else if(sicaklik>=25) {digitalWrite(YESIL,HIGH);//sensörden okunan Ham değer 25 ve üstü ise yeşil led
                              
                            }
                           
                            int yenideger = int(sicaklik);//derece üzerinde sıcaklık değerini göster
                            k=(yenideger/2)*2;
                                  for(int i=0;i<=k;i++){
                                                           tft.fillRoundRect(95-i,106,20,6,0,ST7735_RED );
                                                           delay(20);
                                                         }
                                                         
                            while((digitalRead(sensor)==0));//elimi  sensörden çekene kadar bir daha okuma yapma
                            delay(3000);// değeri okumak için 3 sn bekle
                            sicaklik=0;// sıcaklığı sıfırla ve ekranda gösterme ayarları
                            tft.fillRect(9, 62, 145, 36,0xFFE0 );
                            tft.setTextSize(2);
                            tft.setCursor(20,85);
                            tft.println(sicaklik,1);
                            tft.setCursor(110,85);
                            tft.println("C");
                            tft.drawCircle(140,60, 3, ST7735_BLACK);
                            tft.drawCircle(140,60, 4, ST7735_BLACK);
                           for(int i=0;i<=30;i++){    // dereceyi eski haline geri getirme 
                          tft.fillRoundRect(55+i,106,20,6,0,0xFFE0 );
                          delay(20);
                                                 }
                          eskiZaman=simdikiZaman;// süreyi başlatmak için eşitle
}

                 if(simdikiZaman-eskiZaman>15000){  //  ölçümden sonra 15 saniye geçmişse ve başka bir ölçüm olmamışsa 
                                                    //aşağıdaki ekranı göster.
                                                  tft.fillRect(20, 42, 120, 16,0xFFE0);
                                                  tft.fillRect(135,55, 15, 15,0xFFE0);
                                                  tft.setTextSize(1);
                                                  tft.setFont(&FreeMonoBoldOblique9pt7b);// yazı fontu
                                                  testdrawtext(38, 55, "Elinizi", ST7735_BLACK);
                                                  tft.fillRect(23, 62, 120, 36,0xFFE0 );
                                                  testdrawtext(37, 75, "Sensore", ST7735_BLACK);
                                                  testdrawtext(9, 95, "Yaklastiriniz",ST7735_BLACK);
                                                 eskiZaman=simdikiZaman;// süreyi başlatmak için eşitle
                                                                        // bunu yapmazsanız sürekli yenilendiği için ekranda 
                                                                        // titreme olur.
                                                }
                                                          
                                    
                                     
            digitalWrite(YESIL,LOW);//ledleri söndür.
            digitalWrite(KIRMIZI,LOW);
         
        }


void testdrawtext(int wid, int hei, char *text, uint16_t color) {    // yazı yazdırma ek fonksiyonu
  tft.setCursor(wid, hei);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}
