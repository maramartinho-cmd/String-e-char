#include <Arduino.h>
#include <BluetoothSerial.h>

BluetoothSerial BT;

//78:42:1C:6C:54:AE
uint8_t endSlave[] = {0x78, 0x42, 0x1C, 0x6C, 0x54, 0xAE};

void setup(){
    Serial.begin(9600);
    Serial.setTimeout(5000);

    if (BT.begin("EspMasterOdirlei", true))
    {
        Serial.println("Bluetooth iniciado com sucesso");
    }else
    {
        Serial.println("Erro ao iniciar o bluetooth");
    }

    if (BT.connect(endSlave))
    {
        Serial.println("Conectado com sucesso");
    }else{
        Serial.println("Erro ao conectar");
    }    
}

void loop(){
    if (BT.available())
    {
        String mensagemRecebida = BT.readStringUntil('\n');
        //mensagemRecebida.trim();
        Serial.printf("Temperatura Atual Recebida: %s\n", mensagemRecebida);
    }

    if (Serial.available())
    {
        String mensagemEnviar = Serial.readStringUntil('\n');
        mensagemEnviar.trim();
        BT.println(mensagemEnviar);
    }
}