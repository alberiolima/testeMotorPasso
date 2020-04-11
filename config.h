#ifndef _CONFIG_H_
#define _CONFIG_H_
  /* Configurações específicas para arduino MEGA 2560 */
  #ifdef ARDUINO_AVR_MEGA2560
    #define SHIELD_RAMPS //Usar pinout da RAMPS 1.4 para controle de motor e LCD2004
    #define IHM_LCD2004  //Usar IHM LCD2004 usado normalmente na RAMPS em impressora 3D
  #endif  
  
  /* Configurações específicas para arduino UNO */
  #ifdef ARDUINO_AVR_UNO 
    #define SHIELD_CNC
    #define PIN_BUZZER 5
  #endif  

  /* Portas para driver do motor, usando eixo Y */
  #ifdef SHIELD_RAMPS
    #define PIN_MOTOR_HABILITA A2 
    #define PIN_MOTOR_DIRECAO  A7 
    #define PIN_MOTOR_PASSO    A6
  #endif  
  #ifdef SHIELD_CNC
    #define PIN_MOTOR_HABILITA 8 
    #define PIN_MOTOR_DIRECAO  6 
    #define PIN_MOTOR_PASSO    3 
  #endif 
  

  #ifdef SHIELD_RAMPS

    #define PIN_BTN_STOP 41
    #define BTN_EN1 31
    #define BTN_EN2 33
    #define PIN_BTN_ENC 35
    
    #define PIN_BUZZER 37 //pino do buzzer, no IHM compativel com RAMPS 1.4
    
    /* porta do LCD2004, no IHM compativel com RAMPS 1.4 */    
    #define PIN_LCD_RS 16
    #define PIN_LCD_EN 17
    #define PIN_LCD_D4 23
    #define PIN_LCD_D5 25
    #define PIN_LCD_D6 27
    #define PIN_LCD_D7 29
  #endif  

#endif
