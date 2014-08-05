// Librerías:
    #include <LiquidCrystal_I2C.h>
    #include <VirtualWire.h>
    #include <Wire.h> 
   #include "RTClib.h"
  #include <DS1302.h> 
  #include "DHT.h"

#define DHTPIN 5     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 
  
    #undef int
    #undef abs
    #undef double
    #undef float
    #undef round
    
  DS1302 rtc(8, 7, 6);  
 const int primac = 9;
// Instanciacion del LCD:
   LiquidCrystal_I2C lcd(0x27,20,4);
 
// Variables globales:
  
  byte degree[8] = {
	0b01100,
	0b10010,
	0b10010,
	0b01100,
	0b00000,
	0b00000,
	0b00000,
	0b00000
};
    char prevod[100];
    int Tep01;
    int Tep02;
 DHT dht(DHTPIN, DHTTYPE);
 #define ALARM A0
 long tmax=70, tmin=10;
 
void setup()
{
  
    lcd.begin(20, 4);     // Configuración del LCD, 4 líneas de 20 caracteres cada una.
    lcd.init();
    lcd.backlight();
    lcd.createChar(0,degree);
   
    rtc.setDOW(1);        // Set Day-of-Week to FRIDAY
    rtc.setTime(19, 57, 0);     // Set the time to 12:00:00 (24hr format)
    rtc.setDate(4, 8, 2014);   // Set the date to August 6th, 2010
    vw_set_rx_pin(primac);
    vw_setup(2000);       // Se indica los bits por segundo (baudios) para la comunicacion.
    vw_rx_start();        // Se inicia la recepción.
}
 
 
void loop()
{
   int h = dht.readHumidity();
  int t = dht.readTemperature();  
  

  byte buf[VW_MAX_MESSAGE_LEN];        // Se crea una array de tipo byte de 30 componentes (Es el valor que tiene la constante VW_MAX_MESSAGE_LEN en la libreria)
    byte buflen = VW_MAX_MESSAGE_LEN;    // Se indica que por defecto el tamaño sera de hasta 30 caracteres.
    
 
    // Si se recibe un mensaje desde el Arduino emisor, la función vw_get_message devolverá por valor un true y por referencia el mensaje y si tamaño.
    if( vw_get_message(buf, &buflen) )
    
    {
      int i;
       
 
        // Se imprime por pantalla componente a componente del array hasta que se llegue al final.
        for (i = 0; i < buflen; i++)
        {
          prevod[i] = char(buf[i]);}
          
        sscanf(prevod, "%d,%d",&Tep01, &Tep02);  
           // lcd.print(buf[i]);
            // Otras formas de imprimir:
          // lcd.write(buf[i]);
         //   lcd.print(buf[i], BYTE);
         
      
          
           lcd.clear();
           
             lcd.setCursor(0,0);
             lcd.print("Pec:");
             lcd.print(Tep01);
             lcd.write(byte(8));
             lcd.print("C");
             lcd.print(" Bojler:");
             lcd.print(Tep02);
             lcd.write(byte(8));
             lcd.print("C");
        
        
        	if (Tep01>tmin &&  Tep01<tmax )
{
	noTone(ALARM);
}

else

{
	
	tone(ALARM,400,1000); 
    // lcd.clear();
	lcd.setCursor(0,4);
	
	//lcd.print(temp);
    
	lcd.print("VYSOKA TEPLOTA!!!");
	delay(5000);
}
                       
        }
        
   lcd.setCursor(0,1);
  lcd.print("Hodiny: ");
  lcd.print(rtc.getTimeStr());
  
  lcd.setCursor(0,2);
  lcd.print(rtc.getDOWStr());
  lcd.print(":");
  lcd.setCursor(10,2);
  lcd.print(rtc.getDateStr());
  
  lcd.setCursor(6,4);
  lcd.print("Izba: ");
  lcd.print(t);
  lcd.write(byte(0));
  lcd.print("C");

  lcd.setCursor(17,4);
  lcd.print("Vlh: ");
  lcd.print(h);
  lcd.print(" %");

    }
    

