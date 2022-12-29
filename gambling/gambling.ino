曾柏瑜
#include "BluetoothSerial.h"
#include "LiquidCrystal.h"             // 引入 LCD 函式庫

LiquidCrystal lcd(23,22,21,19,18,5);   // 初始化 LCD 模組
BluetoothSerial myBT;

int rnd;
int num =0;
int money =1000;
int gusstime =0;
int incomeData =0;  //接收資料用的變數，賭注
int incomeData2 =0;  //接收資料用的變數，猜測數字
const byte channel = 0;            //選定PWM通道0
float keep_big;            //保留每一次猜測較大數的緩存
float keep_small;          //保留每一次猜測較小數的緩存


//setup函式//
void setup(){
Serial.begin(9600);
pinMode(15, OUTPUT);
ledcSetup(channel, 10000, 8);    //PWM 通道 0，頻率10000Hz，解析度8位元
ledcAttachPin(25, channel);      //將 PWM 通道 0，接於 GPIO 25 腳
myBT.begin("坐殺博徒");       //定義此藍牙的名字
Serial.setTimeout(10); // 設定為每10毫秒結束一次讀取(數字愈小愈快)
rnd=random(0, 1000); //隨機抽數
  lcd.begin(16, 2);          // 設定LCD字幕為 16*2
  lcd.cursor();           // 顯示游標
  lcd.blink();              // 游標閃爍
  lcd.home();             // 將游標移至左上角
  lcd.print("money:");  // 顯示字串
  lcd.print(money);    // 顯示字串
  lcd.setCursor(0,1);                     // 換行
  lcd.print("rate is ");  // 顯示字串
  lcd.print("0.1%");    // 顯示字串
  float keep_big=0;          //保留每一次猜測較大數的緩存
  float keep_small=0;          //保留每一次猜測較小數的緩存
}

//loop函式//
void loop(){
  
if(money==0){ //假如輸光
  myBT.println("you have no money!!");
  lcd.clear();                // 清除LCD螢幕
  lcd.print("Your money:");  // 顯示字串
  lcd.print(money);    // 顯示字串
  lcd.setCursor(0,1);                     // 換行
  lcd.print("gameover");  // 顯示字串
  money=1000; //重置金錢
  keep_big=0;
  keep_small=0;
  rnd=random(0,1000); //隨機抽數
  gusstime=0; //重置猜的次數
}

//藍牙區//
if (myBT.available()>0 && incomeData ==0) {
  incomeData = myBT.parseInt(); //手機輸入字元存進
  Serial.print("從藍牙接收到測試：");
  Serial.println(incomeData);      //從序列埠印出手機傳來的字元
}
if (myBT.available()>0 && incomeData !=0) {
  incomeData2 = myBT.parseInt(); //手機輸入字元存進
  Serial.print("從藍牙接收到測試2：");
  Serial.println(incomeData2);
}
if (incomeData !=0 && incomeData2 !=0) {             //如果接收到非0
  thansize(incomeData2,incomeData);  //引用比大小
  lcdlook(incomeData2);              //引用lcd顯示
  rate(incomeData2);                 //引用機率顯示
  Serial.print("從藍牙接收到：");
  Serial.println(incomeData);      //從序列埠印出手機傳來的字元
  Serial.print("從藍牙接收到2：");
  Serial.println(incomeData2);
  buzzerF(gusstime); //引用蜂鳴器函數
  incomeData =0;          //輸入數歸0
  incomeData2 =0;          //輸入數歸0
 }
}

//比大小函式//
void thansize(int t,int b){
  if(t<rnd){
    myBT.println("too small");   //回傳訊息給手機
    myBT.println(rnd);   //回傳訊息給手機
    money=money-b;
    gusstime++;
  }
  else if(t>rnd){
    myBT.println("too big");   //回傳訊息給手機
    myBT.println(rnd);   //回傳訊息給手機
    money=money-b;
    gusstime++;
  }
  else if(t==rnd){
    myBT.println("bingo! the number is ...");   //回傳訊息給手機
    myBT.println(rnd);
    if(gusstime<15){                   
    money=money+(b*(15-gusstime-1));
    }                                          //獎金=賭注*(15-猜測次數)
    else if(gusstime>=15){
      money=money;                            //獎金為只能拿回賭注
    }
    myBT.println("money:");
    myBT.println(money);
  }
}

//蜂鳴器函數//
void buzzerF(int gusstime){
  if(gusstime==1){
    ledcWriteTone(channel, 500);   //從通道0發聲，頻率為500Hz
    delay(650);
    ledcWriteTone(channel, 0);     //從通道0發聲，頻率0Hz代表靜音
    delay(650);                    //持續發聲1.3秒
  }
  else if(gusstime==2){
    ledcWriteTone(channel, 500);   //從通道0發聲，頻率為500Hz
    delay(550);                    //持續發聲1.1秒
    ledcWriteTone(channel, 0);     //從通道0發聲，頻率0Hz代表靜音
    delay(550);                    //持續發聲1.3秒
  }
  else if(gusstime==3){
    ledcWriteTone(channel, 500);   //從通道0發聲，頻率為500Hz
    delay(450);                    //持續發聲0.9秒
    ledcWriteTone(channel, 0);     //從通道0發聲，頻率0Hz代表靜音
    delay(450);                    
  }
  else if(gusstime==4 || gusstime==5){
    ledcWriteTone(channel, 600);   //從通道0發聲，頻率為500Hz
    delay(350);                    //持續發聲0.7秒
    ledcWriteTone(channel, 0);     //從通道0發聲，頻率0Hz代表靜音
    delay(350);                    
  }
  else if(gusstime==6 || gusstime==7){
    ledcWriteTone(channel, 800);   //從通道0發聲，頻率為500Hz
    delay(200);                    //持續發聲0.4秒
    ledcWriteTone(channel, 0);     //從通道0發聲，頻率0Hz代表靜音
    delay(200);                    
  }
  else if(gusstime==9 || gusstime==8){
    ledcWriteTone(channel, 800);   //從通道0發聲，頻率為500Hz
    delay(50);                    //持續發聲0.1秒
    ledcWriteTone(channel, 0);     //從通道0發聲，頻率0Hz代表靜音
    delay(50);                    
  }
  else if(gusstime>=10){
    ledcWriteTone(channel, 900);   //從通道0發聲，頻率為500Hz
    delay(5);
  }
}

//lcd顯示
void lcdlook(int t){
  
  if(t<rnd){
    lcd.clear();                            // 清除LCD螢幕
    lcd.print("money:");                    // 顯示字串
    lcd.print(money);                       // 顯示字串
    lcd.setCursor(0,1);                     // 換行
    lcd.print(t);    
    lcd.print(" ");  
    lcd.print("small");    
    lcd.print(" ");         
  }
  else if(t>rnd){
    lcd.clear();                            // 清除LCD螢幕
    lcd.print("money:");                    // 顯示字串
    lcd.print(money);                      // 顯示字串
    lcd.setCursor(0,1);                    // 換行
    lcd.print(t);
    lcd.print(" ");
    lcd.print("big");
    lcd.print(" ");
  }
  else if(t==rnd){
    lcd.clear();                     // 清除LCD螢幕
    lcd.print("money : ");           // 顯示字串
    lcd.print(money);                // 顯示字串
    lcd.setCursor(0,1);              // 換行
    lcd.print("bingo!!!");           // 換行
    lcd.print("         ");
  }
}

//lcd顯示機率
void rate(int t){
  float choice;
  float t_float = t;
  float rnd_float = rnd;
  if(keep_big==0 && keep_small==0){
    if(t<rnd){
      choice = 100/ (1000-t_float);
      lcd.print(choice);
      lcd.print("%");
      lcd.print("   ");
      keep_small = t;
    }
    if(t>rnd){
      choice = 100/(t_float-1);
      lcd.print(choice);
      lcd.print("%");
      lcd.print("   ");
      keep_big = t;
    }
  }
  if(keep_small != 0 && (rnd<t) && keep_big==0){
    keep_big = t;
    choice = 100/(keep_big - keep_small);
    lcd.print(choice);
    lcd.print("%");
    lcd.print("   ");
  }
  if(keep_small != 0 && (rnd>t) && keep_big==0){
    keep_small = t;
    choice = 100/(1000 - keep_small);
    lcd.print(choice);
    lcd.print("%");
    lcd.print("   ");
  }
  if(keep_big != 0 && (rnd>t) && keep_small==0){
    keep_small = t;
    choice = 100/(keep_big - keep_small);
    lcd.print(choice);
    lcd.print("%");
    lcd.print("   ");
  }
  if(keep_big != 0 && (rnd<t) && keep_small==0){
    keep_big = t;
    choice = 100/keep_big;
    lcd.print(choice);
    lcd.print("%");
    lcd.print("   ");
  }
  if(keep_big != 0 && (t>rnd) && keep_small != 0){
    keep_big = t;
    choice = 100/(keep_big - keep_small);
    lcd.print(choice);
    lcd.print("%");
    lcd.print("   ");
  }
  if(keep_small != 0 && (t<rnd) && keep_big != 0){
    keep_small = t;
    choice = 100/(keep_big - keep_small);
    lcd.print(choice);
    lcd.print("%");
    lcd.print("   ");
  }
  if(t==rnd){
    gusstime=0; //重置猜的次數
    rnd=random(0,1000);              //隨機抽數
    keep_small=0;
    keep_big=0;
  }
}
