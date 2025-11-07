

#include <Arduino.h>
#include <BluetoothSerial.h>

// -------- Bluetooth Master --------

/*unsigned long tempo;
bool pisca;
bool led;
BluetoothSerial BTG;

uint8_t endG[] = {0xF4, 0x65, 0x0B, 0x48, 0xA6, 0xCA};

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(2000);
  pinMode(2, OUTPUT);
  if (BTG.begin("ESP32_Master_G", true))
  {
    Serial.println("Bluetooth (G) iniciado com sucesso!");
  }
  else
  {
    Serial.print("erro ao iniciar o bluetooth");
  }

  if (BTG.connect(endG))
  {
    Serial.println(" Conectado ao receptor G!");
  }
  else
  {
    Serial.println(" Falha ao conectar ao receptor G.");
  }

  Serial.println("Digite o comando: liga / desliga / pisca /");
}

void loop()
{
  if (BTG.available())
    {
        String mensagemRecebida = BTG.readStringUntil('\n');
        mensagemRecebida.trim();
        Serial.printf("Mensagem recebida: %s", mensagemRecebida);
    }
  String comando;
  if (Serial.available())
  {
    comando = Serial.readStringUntil('\n');
    comando.trim();
    BTG.println(comando);
  }

  if (comando.equalsIgnoreCase("ligar"))
  {
    led = true;
    pisca = false;
    Serial.println("led ligado");
  }

  if (comando.equalsIgnoreCase("desligar"))
  {
    led = false;
    pisca = false;
    Serial.println("led desligado");
  }

  if (comando.equals("pisca"))
  {
    Serial.println("faz o Led piscar");
    pisca = true;
  }

  if (pisca)
  {
    if (millis() - tempo >= 500)
    {
      led = !led;
      tempo = millis();
    }
  }

  digitalWrite(12, led);
}
*/

// atividade 2



#include "BluetoothSerial.h"
#include "Bounce2.h"

BluetoothSerial BTG;

unsigned long tempoAtual = 0;
unsigned long tempoAnterior = 0;
bool estadoPisca = false;

bool pisca = false;
bool led = false;
const int pinoBotao = 0;
const int pinled = 2;

bool botaoPressionado = false;
int duracao;
bool estadoLed = false;

uint8_t endG[] = {0xF4, 0x65, 0x0B, 0x48, 0xA6, 0xCA};

Bounce botao = Bounce();

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(2000);

  pinMode(2, OUTPUT);
  botao.attach(pinoBotao, INPUT_PULLUP);

  // Bluetooth opcional (mantido, mas não interfere)
  if (BTG.begin("ESP32_Master_G", true))
  {
    Serial.println("Bluetooth (G) iniciado com sucesso!");
  }
  else
  {
    Serial.println("Erro ao iniciar o Bluetooth!");
  }

  if (BTG.connect(endG))
  {
    Serial.println("Conectado ao receptor G!");
  }
  else
  {
    Serial.println("Falha ao conectar ao receptor G.");
  }

  Serial.println("=== Sistema iniciado ===");
  Serial.println("Clique curto: liga/desliga LED");
  Serial.println("Pressão > 2s: modo piscar");
  Serial.println("Se piscando, pressione novamente: desliga LED");
}

void loop()
{
  tempoAtual = millis();
  botao.update();

  // Quando o botão é solto
  if (botao.rose())
  {
    // unsigned long duracao = botao.previousDuration(); // quanto tempo ficou pressionado

    if (botao.previousDuration() >= 2000)
    {
      // Se segurou mais de 2s → alterna o modo pisca
      estadoPisca = !estadoPisca;
      Serial.println(estadoPisca ? "Modo piscar ativado!" : "Modo piscar desligado!");
      if (!estadoPisca)
      {
        estadoLed = false;
        digitalWrite(pinled, LOW);
      }
    }
    else
    {
      // Clique curto → liga/desliga o LED (somente se não estiver piscando)
      if (!estadoPisca)
      {
        estadoLed = !estadoLed;
        digitalWrite(pinled, estadoLed);
        Serial.println(estadoLed ? "LED ligado." : "LED desligado.");
      }
    }
  }

  // Se estiver no modo pisca → alterna o LED a cada 2s
  if (estadoPisca && (tempoAtual - tempoAnterior >= 2000))
  {
    tempoAnterior = tempoAtual;
    estadoLed = !estadoLed;
    digitalWrite(pinled, estadoLed);
  }
}

