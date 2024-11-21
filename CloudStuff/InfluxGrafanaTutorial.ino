//InfluxDB & Grafana Tutorial
//The DIY Life by Michael Klements
//21 January 2022

#include <Wire.h>                                                   //Import the required libraries
//#include "DHT.h"
//#include "Seeed_BMP280.h"
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#define DEVICE "ESP32"

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

// WiFi AP SSID
  #define WIFI_SSID "CapstoneWifi"
  // WiFi password
  #define WIFI_PASSWORD "RuleNumber9"
  
  #define INFLUXDB_URL "https://us-east-1-1.aws.cloud2.influxdata.com"
  #define INFLUXDB_TOKEN "K4sK5y916JhDnrGo8P_XslKucm3rBj08Zc8tE0f9r800jZy4tWJGhkydZpDVBrX1mBPI85UQ9gB7XvRabl0f4Q=="
  #define INFLUXDB_ORG "714aceffc77c4d03"
  #define INFLUXDB_BUCKET "Bucket1Testing"
  
  // Time zone info
  #define TZ_INFO "UTC-7"
//DHT dht(4,DHT11);                                                   //DHT and BMP sensor parameters
//BMP280 bmp280;

int temp = 0;                                                       //Variables to store sensor readings
int humid = 0;
int pressure = 0;

float latitude = 40.00749;
float longitude = -105.26265;
// defaultCoords = (40.00749, -105.26265) //(lat, long)

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);                 //InfluxDB client instance with preconfigured InfluxCloud certificate

Point sensor("weather");                                            //Data point

void setup() 
{
  Serial.begin(115200);                                             //Start serial communication
  
  /*dht.begin();                                                      //Connect to the DHT Sensor
  if(!bmp280.init())                                                //Connect to pressure sensor
    Serial.println("bmp280 init error!");
*/
  WiFi.mode(WIFI_STA);                                              //Setup wifi connection
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to wifi");                               //Connect to WiFi
  while (wifiMulti.run() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  sensor.addTag("device", DEVICE);                                   //Add tag(s) - repeat as required
  sensor.addTag("SSID", WIFI_SSID);

  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");                 //Accurate time is necessary for certificate validation and writing in batches

  if (client.validateConnection())                                   //Check server connection
  {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } 
  else 
  {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }
}

void loop()                                                          //Loop function
{
  temp = random(300);//dht.readTemperature();                                      //Record temperature
  humid = random(300);//dht.readHumidity();                                        //Record temperature
  pressure = random(300);//bmp280.getPressure()/100;                               //Record pressure
  //latitude = 40.00749;
  //longitude = -105.26265;
  Point sensor("weather");                                            //Data point

  sensor.clearFields();                                              //Clear fields for reusing the point. Tags will remain untouched
  sensor.addField("temperature", temp);                              // Store measured value into point
  sensor.addField("humidity", humid);                                // Store measured value into point
  sensor.addField("pressure", pressure);                             // Store measured value into point
  sensor.addField("latitude",latitude);
  sensor.addField("longitude",longitude);
  
  Point sensor("weather2");                                            //Data point

  sensor.clearFields();                                              //Clear fields for reusing the point. Tags will remain untouched
  sensor.addField("temperature", temp);                              // Store measured value into point
  sensor.addField("humidity", humid);                                // Store measured value into point
  sensor.addField("pressure", pressure);                             // Store measured value into point
  sensor.addField("latitude",latitude);
  sensor.addField("longitude",longitude);

  if (wifiMulti.run() != WL_CONNECTED)                               //Check WiFi connection and reconnect if needed
    Serial.println("Wifi connection lost");

  if (!client.writePoint(sensor))                                    //Write data point
  {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }
  
  Serial.print("Temp: ");                                            //Display readings on serial monitor
  Serial.println(temp);
  Serial.print("Humidity: ");
  Serial.println(humid);
  Serial.print("Pressure: ");
  Serial.println(pressure);
  Serial.print("(latitude,longitude): ");
  Serial.print(latitude);
  Serial.print(' , ');
  Serial.println(longitude);
  delay(60000);                                                      //Wait 60 seconds
}
