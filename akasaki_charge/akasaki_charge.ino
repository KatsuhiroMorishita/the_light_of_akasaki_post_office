/* name: akasaki_charge             */
/* purpose: 鉛蓄電池の充放電を制御する  */
/* author: original: Sekihara       */
/* platform: Arduino UNO R3         */
/* lisence: MIT                     */
/* created: 2014-10 (?)             */
// ------------------------------
// [Port map]
// No data
// 
// 
// 
// ------------------------------

// 充放電制御パラメータ
const float charge_cut_voltage = 14.0;          // 充電カット電圧, 定数類は大文字のほうがいいかも。
const float discharge_cut_voltage = 10.8;       // 放電終止電圧
const float discharge_possible_voltage = 11.5;  // 放電可能電圧

// port
const int charge_control_port = 1;
const int discharge_control_port = 2;
const int adport_for_solar_cell_voltage = A1;
const int adport_for_battery_voltage = A3;

// other
const long check_cycle = 5000l;
const int charging = HIGH;
const int discharging = HIGH;
int charge_condition = !charging;           // 充電の状況, 初期設定と一貫性を持たせるためにグローバル変数とした。
int discharge_condition = !discharging;     // 放電の状況


// タイムスタンプをシリアルで出力
void print_timestamp()
{
  Serial.print("time stamp,");
  Serial.println(millis());
  return;
}


void setup()
{
  // port setting
  pinMode(charge_control_port, OUTPUT);        // HIGHでON・・・本当？
  pinMode(discharge_control_port, OUTPUT);
  digitalWrite(charge_control_port, charge_condition);
  digitalWrite(discharge_control_port, discharge_condition);

  // serial setting
  Serial.begin(9600);

  // other
  //digitalWrite(discharge_control_port, HIGH); // 強制的に放電させる場合はコメントアウトを解く
  //while(1);

  Serial.println("-- setup fin. --");
}

void loop()
{
  // 電圧の計測（ここではAD変換値）
  double vol_batt = analogRead(adport_for_battery_voltage);
  double vol_cell = analogRead(adport_for_solar_cell_voltage);
  // AD変換値を電圧に変換する
  // 回路構成が不明なので記述不可能（ポートに入力される電圧は計算可能だが、計測したい電圧の分圧電圧にすぎない（はず））, もし、直接つないでいたらポートが焼けている。

  // 充電の可否を判断
  if(charge_condition == charging){
    if(vol_batt > charge_cut_voltage){
      print_timestamp();
      Serial.println("-- charging fin. --");
      charge_condition = !charging;
    }
  }
  else{
    if(vol_batt < vol_cell && vol_batt < charge_cut_voltage){
      print_timestamp();
      Serial.println("-- charging start --");
      charge_condition = charging;
    }
  }
  
  // 放電の可否を判断
  if(discharge_condition == discharging){
    if(vol_batt < discharge_cut_voltage){
      print_timestamp();
      Serial.println("-- discharge stop --");
      discharge_condition = !discharging;
    }
  }
  else{
    if(vol_batt > discharge_possible_voltage){
      print_timestamp();
      Serial.println("-- discharge start --");
      discharge_condition = discharging;
    }
  }
  
  // 判断の結果を反映させる（両方共判断の後に反映させることで過渡状態での判断を避ける）
  digitalWrite(charge_control_port, charge_condition);
  digitalWrite(discharge_control_port, discharge_condition);

  delay(check_cycle);           // この待ち時間が短いと短時間の内にスイッチがON−OFFされてしまう可能性がある。
}



