/* name: akasaki_charge             */
/* purpose: 生血蓄電池の充放電を制御する */
/* author: original: Sekihara       */
/* lisence: MIT                     */
/* created: 2014-10 (?)             */
// [Port map]
// No data
// 
// 
// 
// 
#include<stdlib.h>

#define limit_high_cut       14.0 // 充電カット電圧
#define limit_high_reconnect 13.5 // 充電復帰電圧
#define limit_low_cut        10.8 // 放電終止電圧
#define limit_low_reconnect  11.5 // 復帰電圧

#define sw_in_pin 1   // 謎のポート
#define sw_out_pin 2  // 



double vol_b, vol_p;
bool sw_in, sw_out;
bool flag;

void setup() {
  pinMode(sw_in_pin, OUTPUT);
  pinMode(sw_out_pin, OUTPUT);
  //Serial.begin(9600);
  sw_in = sw_out = true;//フラグ建設
  flag = 1;
}

void loop() {
  /*最悪は使う
   if(flag){
   digitalWrite(sw_out_pin,HIGH);
   delay(1000*60*60*48);
   flag = 0;
   }
   */
  //値のインプット
  //バッテリ
  vol_b = analogRead(3);
  //太陽光パネル
  vol_p = analogRead(1);

  //条件判断：充電回路スイッチ
  //充電止め
  if(vol_b > vol_p && !sw_in )
    sw_in = false;
  else if(vol_b > limit_high_cut)
    sw_in = false;
  //充電開始
  else if(vol_b < limit_high_reconnect)
    sw_in = true;

  //放電回路スイッチ
  //放電止め
  if(vol_b < limit_low_cut )
    sw_out = false;
  //放電開始
  else if(vol_b > limit_low_reconnect)
    sw_out = true;

  //回路のスイッチ制御
  if(sw_in == true)
    digitalWrite(sw_in_pin, HIGH);
  else
    digitalWrite(sw_in_pin, LOW);

  if(sw_out == true)
    digitalWrite(sw_out_pin, HIGH);
  else
    digitalWrite(sw_out_pin, LOW);

  /*
      Serial.print("vol_b=");
   Serial.print(vol_b/(double)1024*(double)5);
   Serial.print("     vol_p=");
   Serial.println(vol_p/(double)1024*(double)5);
   */
}

