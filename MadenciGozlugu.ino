//---oled işlemleri-----------------
#include <Wire.h>
#include <Adafruit_GFX.h> // grafik tabanli projeler icin
#include <Adafruit_SSD1306.h> // metin tabanli projeler icin
#define SCREEN_WIDTH 128 // genislik
#define SCREEN_HEIGHT 64 // yukseklik
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
//---sensörler-----------------
#define metanPini A0
#define coPini A1
#define lpgPini A2
#define temizHavaPini A3
int androidDegerCek;
int metanDegeri;
int coDegeri;
int lpgDegeri;
int temizHavaDegeri;
//---buzzer----------------
int buzzerPin=3;

void setup() {
  Serial.begin(9600); // bluetooth bağlantısı
  pinMode(buzzerPin,OUTPUT); // buzzer
  
  //-----------oled ekran işlemleri
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // i2c adresi
    Serial.println(F("SSD1306 baglanti problemi"));
    for(;;);
  }
   display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 10);
      display.print("Hosgeldiniz! Lutfen Uygulamaya girip Bluetooth baglantisini yapiniz..."); // bLUETOOTH'A BAĞLANMADAN ÖNCEKİ YAZI
  
  
}
void loop() {
   
  if (Serial.available() > 0) {
  
  androidDegerCek = Serial.read(); // androidden gelen değer
     
   if (androidDegerCek == '1') {
       for(int i=0;i<3240;i++){ // buradan günlük kaç kere veri göndereceğimizi belirleyeceğiz. // for döngüsü olmazsa sadece 1 veri gönderiyor!!!
       //delay 10 sn.de bir veri gönderiyor. Bizde 3240'kadar yaptık.
       //Nedeni, 3240x10=32400sn yapıyor. O da 9 saate eşit. Yani mesai saatine.
       //---------------------------------------------------------------------------
       
      //5 tane veri göndereceğiz. çünkü ölçülmesi gereken, metan,butan,propan,co,temizHava olarak 5 verimiz var. 
      //Ama bütan ve propan LPG olarak aynı değerleri alıyorlar. O yüzden 4 değer tanımlayıp. MQ6dan alınan değeri hem 
      //bütana hem de propana ekledik! Bu sayede 5 değere de atama yapabildik.
       metanDegeri = analogRead(metanPini); // metan (mQ4) değeri
       coDegeri = analogRead(coPini); // CO (mQ7) değeri
       lpgDegeri = analogRead(lpgPini); // lpg (bütan/propan) (mQ6) değeri
       temizHavaDegeri = analogRead(temizHavaPini); // lpg (bütan/propan) (mQ6) değeri
      
      Serial.print(metanDegeri); //metan // değerler hepsinde ppm olarak alınıyor.
      Serial.print(",");
      Serial.print(lpgDegeri);//bütan
      Serial.print(",");
      Serial.print(lpgDegeri); //propan
      Serial.print(",");
      Serial.print(coDegeri); // co
      Serial.print(",");
      Serial.println(temizHavaDegeri); //temizhava
      //Örnek değer gönderimi-> 251,620,620,163,366

      if(metanDegeri>290){        // bu 290 değeri buzzerın çalıştığını göstermek içindir. Aslında esas değer, ppmi LEL'e çevirerek belirlenecektir.' 
      //1 %LEL Metan gazı > 0,05 %vol Metan gazına > 500 ppm Metan gazına eşdeğerdir.
      //kaynak: https://novel.com.tr/vol-ppm-lel-uel-nedir-gaz-konsantrasyon-birimleri.html
      //Metan %5 LEL'de patlayıcı olabilir. YAni, metanın gerçek patlayıcılık değerinin alt sınırı 2500 ppm'Dir.

      //--------oled uyarı---
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 30); // BAŞLANGIÇ KARAKTERİNİN X-Y KOORDİNATÖRÜ 
      display.println("ORTAMI TERK ET!");
      display.display(); 
      
      //--buzzer uyarı---
      digitalWrite(buzzerPin,HIGH);
      delay(2000);
      digitalWrite(buzzerPin,LOW);
      delay(2000);
      
      }
      if(coDegeri>400){        // ülkemizdeki yasal değeri 50ppm.' 400 ppm 1-2 saat maruz kalındığında Ciddi yoğun baş ağrısı
        //ve diğer belirtilerin daha şiddetli ortaya çıkışı ve 3 saatten sonra yaşamsal tehdit oluşumu 
      //kaynak: https://www.maden.org.tr/genel/bizden_detay.php?kod=8735
      //https://www.ttb.org.tr/STED/sted1100/3.html
      
      //--------oled uyarı---
      display.clearDisplay();
      display.setTextSize(3);
      display.setTextColor(WHITE);
      display.setCursor(10, 20); // BAŞLANGIÇ KARAKTERİNİN X-Y KOORDİNATÖRÜ 
      display.println("ORTAMI TERK ET!");
      display.display(); 
      
      //--buzzer uyarı---
      digitalWrite(buzzerPin,HIGH);
      delay(2000);
      digitalWrite(buzzerPin,LOW);
      delay(2000);
      }
      if(lpgDegeri>1000){        // LPG gazını oluşturan Propan gazının Alt Patlama Limiti %2.1 hacimsel
      // üst patlama limiti ise % 9.5, Bütan gazının Alt Patlama Limiti %2 hacimsel, üst patlama limiti ise % 9.5 hacimseldir.
      // Yani alt limit 2x500ppm= 1000
      //kaynak: https://www.maden.org.tr/genel/bizden_detay.php?kod=8735
        //--------oled uyarı---
      display.clearDisplay();
      display.setTextSize(3);
      display.setTextColor(WHITE);
      display.setCursor(10, 20); // BAŞLANGIÇ KARAKTERİNİN X-Y KOORDİNATÖRÜ 
      display.println("ORTAMI TERK ET!");
      display.display(); 
      
      //--buzzer uyarı---
      digitalWrite(buzzerPin,HIGH);
      delay(2000);
      digitalWrite(buzzerPin,LOW);
      delay(2000);
      }

      if(temizHavaDegeri>1000){        // aslında ölçülen ppm değeri kirli havayı gösterir.
      //baz aldığımız gaz CO2 olup max değeri 1000ppm'dir.
      //http://nek.istanbul.edu.tr:4444/ekos/TEZ/48410.pdf  
       //--------oled uyarı---
      display.clearDisplay();
      display.setTextSize(3);
      display.setTextColor(WHITE);
      display.setCursor(10, 20); // BAŞLANGIÇ KARAKTERİNİN X-Y KOORDİNATÖRÜ 
      display.println("ORTAMI TERK ET!");
      display.display(); 
      
      //--buzzer uyarı---
      digitalWrite(buzzerPin,HIGH);
      delay(2000);
      digitalWrite(buzzerPin,LOW);
      delay(2000);
      }
      //------------OLED İŞLEMLERİ----
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 10);
      display.print("Metan Degeri: ");
      display.println(metanDegeri);
      display.print("LPG Degeri: ");
      display.println(lpgDegeri);
      display.print("CO Degeri: ");
      display.println(coDegeri);
      display.print("Hava Degeri: ");
      display.println(temizHavaDegeri);
      display.display(); 
       
      delay(10000); // 10sn'ye de 1 değer gönder
      }
     } 
   else{   Serial.println("Arduino Kaynaklı Bağlantı Hatası!");  }
  }
}
