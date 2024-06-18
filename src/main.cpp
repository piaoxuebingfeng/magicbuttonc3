/*
ESP32C3 Magic Button
ble test
通过蓝牙控制手机拍照
*/
#include <BleKeyboard.h>
#include <Arduino.h>
#include "OneButton.h"


// BleKeyboard bleKeyboard("magicbuttonc3","ESPRESSIF",100);
BleKeyboard bleKeyboard;


#define POWER_PIN 7
#define LED0_GPIO3 3
#define LED1_GPIO6 6

#define MAGIC_BUTTON 10

static int power_shutdown_check=0;

OneButton MagicButton(MAGIC_BUTTON, true);


void system_shutdown()
{
  Serial.println("system shutdown");
  digitalWrite(POWER_PIN, LOW);
}


void BleKeyboard_SendMediaKey(const uint8_t *media_key)
{
    if(bleKeyboard.isConnected())
    {
        bleKeyboard.write(media_key);
        delay(10);
    }
}
void BleKeyboard_Send_PlayMusic()
{
    if(bleKeyboard.isConnected())
    {
        Serial.println("ble send media play_pause");
        BleKeyboard_SendMediaKey(KEY_MEDIA_PLAY_PAUSE);
    }
}

void BleKeyboard_Send_Volumeup()
{
    if(bleKeyboard.isConnected())
    {
        Serial.println("ble send volumeup");
        BleKeyboard_SendMediaKey(KEY_MEDIA_VOLUME_UP);
    }
}


void magic_button_click()
{
    Serial.println("magic button click");
    digitalWrite(LED1_GPIO6, HIGH);
    vTaskDelay(20);
    digitalWrite(LED1_GPIO6, LOW);
    power_shutdown_check=0;
    BleKeyboard_Send_PlayMusic();
}

void magic_button_doubleclick()
{
    Serial.println("magic button double click");
    digitalWrite(LED1_GPIO6, HIGH);
    vTaskDelay(50);
    digitalWrite(LED1_GPIO6, LOW);
    vTaskDelay(50);
    digitalWrite(LED1_GPIO6, HIGH);
    vTaskDelay(50);
    digitalWrite(LED1_GPIO6, LOW);
    power_shutdown_check=0;
    BleKeyboard_Send_Volumeup();
}


void magic_btn_long_press()
{
  Serial.println("magic button long press");
  // 断电关机
  digitalWrite(LED1_GPIO6, HIGH);
  vTaskDelay(20);
  digitalWrite(LED1_GPIO6, LOW);
  vTaskDelay(20);
  digitalWrite(LED1_GPIO6, HIGH);
  vTaskDelay(20);
  digitalWrite(LED1_GPIO6, LOW);
  vTaskDelay(20);
  digitalWrite(LED1_GPIO6, HIGH);
  vTaskDelay(20);
  digitalWrite(LED1_GPIO6, LOW);
  vTaskDelay(20);
  system_shutdown();
}


void TaskBtn(void* parameter)
{
  while(1)
  {
    MagicButton.tick();
    vTaskDelay(5);
  }

}

void setup() {
  // 上电 电源控制
  pinMode(POWER_PIN,OUTPUT);
  digitalWrite(POWER_PIN, HIGH);

   Serial.begin(115200);
  pinMode(LED0_GPIO3,OUTPUT);
  pinMode(LED1_GPIO6,OUTPUT);


  digitalWrite(LED0_GPIO3, HIGH);
  digitalWrite(LED1_GPIO6, LOW);
  delay(500);
  digitalWrite(LED0_GPIO3, LOW);
  digitalWrite(LED1_GPIO6, HIGH);
  delay(500);
  digitalWrite(LED0_GPIO3, HIGH);
  digitalWrite(LED1_GPIO6, LOW);
  delay(500);
  digitalWrite(LED0_GPIO3, LOW);
  digitalWrite(LED1_GPIO6, HIGH);
  delay(500);
  digitalWrite(LED0_GPIO3, LOW);
  digitalWrite(LED1_GPIO6, LOW);
   Serial.println("magic button ,system start!");
   bleKeyboard.setName("magicbtn");
   bleKeyboard.begin();

  MagicButton.attachClick(magic_button_click);
  MagicButton.attachDoubleClick(magic_button_doubleclick);
  MagicButton.attachDuringLongPress(magic_btn_long_press);

  xTaskCreate(&TaskBtn, "BtnTaskThread", 8192, NULL, 8, NULL);  


}


void loop() {
  // if(bleKeyboard.isConnected()) {
  
  //   bleKeyboard.releaseAll();
  
  // }
  delay(1000);
  power_shutdown_check++;
  if(power_shutdown_check>120)
  {
    Serial.println("no action,system will shutdown now");
    system_shutdown();
    power_shutdown_check=0;
  }
  else{
    Serial.printf("power_shutdown_check:%d s,default 120s\n",power_shutdown_check);
    if(bleKeyboard.isConnected())
    {
      Serial.println("ble is connected");
      digitalWrite(LED0_GPIO3, HIGH);
    }
    else
    {
      Serial.println("ble not connect");
      digitalWrite(LED0_GPIO3, HIGH);
      delay(20);
      digitalWrite(LED0_GPIO3, LOW);
      delay(20);
    }
  }

}
