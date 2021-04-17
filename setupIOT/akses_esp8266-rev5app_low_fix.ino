    #include <dht.h>
    #include <DHT.h>
    #include <SoftwareSerial.h>
    #include <Servo.h>

    Servo myservo;
    dht DHT;
    int sensor = 8;
    const int sensorTanah = A2;
    const int sensorhujan = A0;
    int LDR = A1;
    const int relay1 = 4;
    const int relay2 = 6;
    const int relay3 = 7; 
    const int relay4 = 9;
    int moisture;
    int hujan;
    int  humidity,temperature;
    int ldr;
    int LedPin = 12;
    int up = 90;
    int down = 0;
    long int period = 300000;
    long int time_now = 0;
    int pos = 0;

    SoftwareSerial wifi(10,11); // RX, TX

void setup() {                
  
    pinMode (sensor,INPUT);
    pinMode (sensorTanah, INPUT);    
    pinMode (LedPin,OUTPUT);
    pinMode (LDR,INPUT);
    pinMode (sensorhujan, INPUT);
    pinMode (relay1,OUTPUT);
    pinMode (relay2,OUTPUT);
    pinMode (relay3,OUTPUT);
    pinMode (relay4,OUTPUT);

    digitalWrite (relay1,HIGH);
    digitalWrite (relay2,HIGH);
    digitalWrite (relay3,HIGH);
    digitalWrite (relay4,HIGH);
    myservo.attach(5);
    myservo.write(pos);
    Serial.begin(9600);
    wifi.begin(9600);
      
}

void loop() {

    myservo.attach(5);
    moisture = analogRead(sensorTanah);
    moisture = map (moisture,1023,0,0,100);
    Serial.print ("Kelembaban Tanah : ");
    Serial.print(moisture);
    Serial.println ("%");
    delay (3000);
             
    DHT.read22(sensor);
    humidity=DHT.humidity;
    temperature=DHT.temperature;
    Serial.print("Kelembaban Udara:"); 
    Serial.print(humidity);
    Serial.println("%");
    Serial.print("Suhu:");
    Serial.print(temperature);
    Serial.println("'C \n");
    delay (3000);
      
    hujan = analogRead(sensorhujan);
    hujan = map (hujan,1023,0,100,0);
    Serial.print ("Cuaca : ");
    Serial.print(hujan);
    Serial.println ("%");
    delay (3000);

    ldr = analogRead(LDR);
    ldr = map (ldr,1023,0,100,0);
    Serial.print ("Intensitas Cahaya : ");
    Serial.print(ldr);
    Serial.println ("% \n");
    myservo.detach();
    delay (3000);

   // digitalWrite (relay1, HIGH);
    //digitalWrite (relay4, HIGH);
    delay (1000);
{
    String cmd = "AT+CIPSTART=\"TCP\",\"";
    cmd+= "184.106.153.149"; // api.thingspeak.com
    cmd+="\",80";
    wifi.println(cmd);
    Serial.println(cmd);
    if (wifi.find("Error"));

    delay (2000);
    
    //if (millis() >= time_now + period){time_now += period;
  
    Serial.println ( "Data Update \n");

    String getStr = "GET https://api.thingspeak.com/update?api_key=JKF1XIID057BFF12&field1=0" ;
    getStr +="&field1=";
    getStr += String(humidity);
    getStr +="&field2=";
    getStr += String(temperature);
    getStr +="&field3=";
    getStr += String(moisture);
    getStr +="&field4=";
    getStr += String(hujan);
    getStr +="&field5=";
    getStr += String(ldr);
    getStr += "\r\n\r\n";

//getStr +="HTTP/1.1/1/rn";
//getStr +="\r\n";
    wifi.print("AT+CIPSEND=");
    wifi.println(getStr.length());
    if (wifi.find(">")){
    Serial.print(">");
  } else {
    wifi.println("AT+CIPCLOSE");
    Serial.println("Koneksi Timeout");
    delay(1000);
  }
    wifi.print(getStr);
    delay(1000);
 
    while(wifi.available())
  {
    char c =wifi.read();
    Serial.write(c);
    if (c=='r') Serial.print('n');
  }
}
    
   if (moisture >= 60 && moisture <= 75 )
  {
     digitalWrite (relay4,HIGH);
  }
    else if (moisture <= 59)
  {
     digitalWrite (relay4,LOW);
  }
    else if (moisture >= 76)
  {
     digitalWrite (relay4,HIGH);
  }
    
   if ((temperature >= 22 && temperature <= 28) && (humidity >= 70 && humidity <= 80))
  {
    digitalWrite (relay1,HIGH);
    digitalWrite (relay2,HIGH);
    digitalWrite (relay3,HIGH);
  }
    else if (humidity <= 70 && temperature >= 29)
  {
    digitalWrite (relay1,LOW);
    digitalWrite (relay2,LOW);
    digitalWrite (relay3,HIGH);
  }
    else if (humidity >= 80 && temperature <= 21)
  {
    digitalWrite (relay1,HIGH);
    digitalWrite (relay2,HIGH);
    digitalWrite (relay3,LOW);
  }  
    else if ((temperature >= 22 && temperature <= 28) && humidity >= 80)
  {
    digitalWrite (relay1,HIGH);
    digitalWrite (relay2,HIGH);
    digitalWrite (relay3,LOW);
  }
    else if ((temperature >= 22 && temperature <= 28)&& humidity <= 70)
  {
    digitalWrite (relay1,LOW);
    digitalWrite (relay2,LOW);
    digitalWrite (relay3,HIGH);
  }
    else if ((humidity >= 70 && humidity <= 80) && temperature >= 29)
  {
    digitalWrite (relay1,HIGH);
    digitalWrite (relay2,LOW);
    digitalWrite (relay3,HIGH);
  }
   else if ((humidity >= 70 && humidity <=80 )&& temperature <= 21)
  {
    digitalWrite (relay1,HIGH);
    digitalWrite (relay2,HIGH);
    digitalWrite (relay3,LOW);
  }
    else if (humidity >= 80 && temperature >= 29)
  {
    digitalWrite (relay1,HIGH);
    digitalWrite (relay2,HIGH);
    digitalWrite (relay3,LOW);
  }
    else if (humidity <= 70 && temperature <= 21)
  {
    digitalWrite (relay1,LOW);
    digitalWrite (relay2,HIGH);
    digitalWrite (relay3,LOW);
  }
     
  
   if (hujan >= 51)
  {  
    for (pos = 0; pos == 0; pos -= 1) { 
    myservo.write(pos);             
    delay(15);
         
  }
  }
   else if (hujan <= 50)
  {
    for (pos = 0; pos <= 110; pos += 1) { 
    myservo.write(pos);             
    delay(15);

    }
  }
  

  if (ldr >=26)
  {  
    digitalWrite (LedPin,LOW);   
  }
    else if (ldr <= 25)
  {  
    digitalWrite (LedPin,HIGH);
  }

     delay (3000);
}


  
