#include <SPI.h>
#include <Ethernet.h>
#include "MgsModbus.h"

MgsModbus Mb;
//MgsModbus FC;


// Ethernet settings (depending on MAC and Local network)
byte mac[] = {0x90, 0xA2, 0xDA, 0x0E, 0x94, 0xB5 };
IPAddress ip(192, 168, 1, 160);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

//word Coil;
int contador=0;
int contador1=0;
int contador2=0;
int contador3=0;
int contador4=0;
int contadorproceso=0;
int contadorproceso3=0;
int cont=0;
int contadParo=0;
int llenado_maximo=0;
int llenado_minimo=0;
float peso= 0;
int visor=0;
int material=0;
int material_total=0;

void setup(){
  Serial.begin(9600);
  Serial.println("Serial interface started");
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
 pinMode(8,OUTPUT);
 pinMode(9,OUTPUT);
   
  Ethernet.begin(mac, ip, gateway, subnet);  
  
}

void loop(){
 
  while(true){  
  digitalWrite(6,bitRead( Mb.GetBit(0x00),0));//start
  digitalWrite(7,bitRead( Mb.GetBit(0x01),0));//stop
  digitalWrite(5,bitRead( Mb.GetBit(0x05),0));//sensor vision
  digitalWrite(8,bitRead( Mb.GetBit(0x03),0));//material
  digitalWrite(9,bitRead( Mb.GetBit(0x04),0));

 
 
  if(digitalRead(6)==1||contador==1){
     Mb.MbData[0] = 1023; 
     Mb.SetBit(0xA,true);
    //entry conveyor
     contador=1;
     contadorproceso=0;
  } 

  llenado_maximo=Mb.MbData[2];
  peso=Mb.MbData[6];
  visor=Mb.MbData[5];
    //Serial.println(llenado_maximo);
  //Serial.println(peso);
//  Serial.println(visor);

  if(visor==2 || visor==5 || visor==7){
    Mb.SetBit(0x06,false);  
    Mb.SetBit(0x0E,false);
    Mb.SetBit(0x0F,false);
    Mb.SetBit(0x10,false);
    Mb.SetBit(0x13,false);
    Mb.SetBit(0x12,true);    
     
    material_total=material_total+1;        
    }

    if(visor==2 ){
      Mb.MbData[7] = 1023;
      Mb.SetBit(0x07,true);  
      
      }
      if(visor==5 ){
      Mb.MbData[7] = 900;
      Mb.SetBit(0x08,true);
      
      }
      if(visor==7 ){
      Mb.MbData[7] = 950;
      Mb.SetBit(0x09,true);
      
      }

  if (llenado_maximo==1000){

    Mb.MbData[0] = 0; 
    contador=2;
    } 

  
  if (llenado_maximo > 200 && contador==2) {
        
    
    Mb.SetBit(0x06,true);  
    Mb.SetBit(0x0E,true);
    Mb.SetBit(0x0F,true);
    Mb.SetBit(0x10,true);
    Mb.SetBit(0x13,true); 
    Mb.SetBit(0xA,true); 
       
 } 

 if (digitalRead(8)== 1 &&  contador==2){
  
    Mb.SetBit(0x12,true);
  }

 

     if (digitalRead(8)== 1)
    { material++;
     // Mb.MbData[4]= material;
     // Serial.println(material);
      
    } 
     
  Mb.MbsRun();
  }
}
