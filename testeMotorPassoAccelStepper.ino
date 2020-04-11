#include "config.h"
#include <AccelStepper.h>
#include <Encoder.h>
#include <Button.h>

#ifdef PIN_BTN_ENC
  Button button1(PIN_BTN_ENC);
#endif
#ifdef PIN_BTN_STOP
  Button button2(PIN_BTN_STOP);
#endif

#ifdef IHM_LCD2004
  #include <LiquidCrystal.h>
  LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_EN, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);
  unsigned char opcaoMenu = 0;
#endif

AccelStepper stepper(AccelStepper::DRIVER, PIN_MOTOR_PASSO, PIN_MOTOR_DIRECAO);
#ifdef BTN_EN1
  Encoder myEnc(BTN_EN2, BTN_EN1);
  long oldPosition  = 0;
  unsigned long proximaAtualiza = 0;
#endif

float velocidade[2] = { 800.0, 400.0 };
long quantPassos = 800;
boolean testeON = false;
unsigned long proxStatus = 0;

void setup() {
  /* Inicia pinos */
  pinMode( PIN_MOTOR_HABILITA, OUTPUT );
  pinMode( PIN_MOTOR_DIRECAO, OUTPUT );
  pinMode( PIN_MOTOR_PASSO, OUTPUT );
  digitalWrite( PIN_MOTOR_HABILITA, HIGH ); //inicia motor desabilitado

#ifdef PIN_BTN_STOP
  button2.begin();
#endif

#ifdef PIN_BTN_ENC
  button1.begin();
#endif

#ifdef PIN_BUZZER
  /* bip inicial */
  bibip();
#endif

#ifdef IHM_LCD2004
  /* Inicia LCD2004 IHM */
  lcd.begin(20, 4);
  lcd.print( ">>>Teste de motor");
  lcd.setCursor(0, 1);
  lcd.print( "Alberio lima" );
#endif

  Serial.begin( 9600 );
  while (!Serial);
  Serial.println( F( "Teste de motor de passo") );
  Serial.print( F( "Compilado em: " ) );
  Serial.print(__DATE__);
  Serial.print( F( " ") );
  Serial.println(__TIME__);
  Serial.println(F("Comandos:"));
  Serial.println(F("i inicia"));
  Serial.println(F("f finaliza"));
  Serial.println(F("p999 quantidade de passos"));
  Serial.println(F("v0999 velocidade 0"));
  Serial.println(F("v1999 velocidade 1"));
  Serial.println(F("ex:"));
  Serial.println(F("v0100 //seta velocidade 0 para 100passos/segundo"));
  Serial.println(F("v1125 //seta velocidade 1 para 125passos/segundo"));
  Serial.println(F("p99   //seta a quantidade de passos para 99"));

  delay(2000);
  telaMenu();
  stepper.setMaxSpeed(1000UL);
  stepper.setCurrentPosition(0);
}


void loop() {
  recebeComandoSerial();
  lerEncode();
  lerBotoes();

  if (testeON) {
    executaTeste();
  }  

}

void lerEncode() {
  #ifdef BTN_EN1
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    if ( millis() > proximaAtualiza ) {
      proximaAtualiza = millis() + 100;
      if (opcaoMenu == 0 ) {
        if (newPosition > oldPosition) {
          velocidade[0] += 10;
        } else {
          velocidade[0] -= 10;
        }
      } else if (opcaoMenu == 2) {
        if (newPosition > oldPosition) {
          quantPassos += 10;
        } else {
          quantPassos -= 10;
        }
      } else if (opcaoMenu == 1 ) {
        if (newPosition > oldPosition) {
          velocidade[1] += 10;
        } else {
          velocidade[1] -= 10;
        }
      }
      telaMenu();
    }
    oldPosition = newPosition;
  }
  #endif
}

void recebeComandoSerial() {
  if (Serial.available() > 0) {
    char c = Serial.read();
    if ( c == 'i' ) {
      iniciaTeste();
    } else if (c == 'f') {
      finalizaTeste();
    } else if (c == 'v') {
      delay(2);
      char b = Serial.read();
      if (b == '0') {
        Serial.println("v0");
        velocidade[0] = (float)Serial.parseInt();
      } else if (b == '1') {
        Serial.println("v1");
        velocidade[1] = (float)Serial.parseInt();
      }
    } else if (c == 'p') {
      quantPassos = Serial.parseInt();
    }
    telaMenu();
  }
}

void iniciaTeste() {
  testeON = true;
  /* Habilita motor */
  digitalWrite( PIN_MOTOR_HABILITA, LOW );

  Serial.println( F("Teste iniciado"));
}

void finalizaTeste() {
  testeON = false;
  /* desabilita motor */
  digitalWrite( PIN_MOTOR_HABILITA, HIGH );

  Serial.println( F("Teste finalizado"));
}

void executaTeste() {
  if ( stepper.currentPosition() == 0 ) {
    Serial.print("Direção + ");
    stepper.moveTo(quantPassos);
    stepper.setSpeed(velocidade[0]);
  } else {
    Serial.print("Direção - ");
    stepper.moveTo(0);
    stepper.setSpeed(-velocidade[1]);
  }
  unsigned long tempoInicia = millis();
  while (stepper.distanceToGo() != 0 ) {
    stepper.runSpeed();
  }
  unsigned long tempo = millis() - tempoInicia;
  Serial.println(tempo);
  statusTempo(tempo);
}

void statusTempo(unsigned long t) {
#ifdef IHM_LCD2004
  if ( stepper.currentPosition() == 0 ) {
    lcd.setCursor(14, 3);
  } else {
    lcd.setCursor(8, 3);
  }
  lcd.print(t);
#endif
}

void beep() {
  tone( PIN_BUZZER, 1000, 100);
}

void bibip() {
  beep();
  delay(110);
  beep();
}

void telaMenu() {
#ifdef IHM_LCD2004
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Velocidade 0: "));
  lcd.print((int)velocidade[0]);
  lcd.setCursor(1, 1);
  lcd.print(F("Velocidade 1: "));
  lcd.print((int)velocidade[1]);
  lcd.setCursor(1, 2);
  lcd.print(F("Q.Passos    : "));
  lcd.print(quantPassos);
  lcd.setCursor(1, 3);
  lcd.print(F("Tempo: "));
  lcd.setCursor(0, opcaoMenu);
  lcd.print(F(">"));
#endif
}

void lerBotoes() {
#ifdef PIN_BTN_STOP
  if (button1.pressed()) {
    opcaoMenu++;
    if ( opcaoMenu >= 3 ) {
      opcaoMenu = 0;
    }
    telaMenu();
  }
#endif

#ifdef PIN_BTN_ENC
  if (button2.pressed()) {
    testeON = !testeON;
    if (testeON) {
      iniciaTeste();
    } else {
      finalizaTeste();
      telaMenu();
    }
  }
#endif
}
