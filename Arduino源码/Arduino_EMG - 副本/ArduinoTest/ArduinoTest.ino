/*
 * 无锡市思知瑞科技有限公司
 * 淘宝店铺：大脑实验室
 * 店铺网址：http://brainlab.taobao.com
 * 工作日联系电话：0510-66759621
 */
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "EMGFilters.h"
#define  SensorInputPin A0       //sensor input pin number
unsigned long threshold = 0;      // threshold: Relaxed baseline values.(threshold=0:in the calibration process)
unsigned long EMG_num = 0;      // EMG_num: The number of statistical signals
EMGFilters myFilter;
SAMPLE_FREQUENCY sampleRate = SAMPLE_FREQ_500HZ;
NOTCH_FREQUENCY humFreq = NOTCH_FREQ_50HZ;
void setup() 
{
  myFilter.init(sampleRate, humFreq, true, true, true);
  Serial.begin(115200);
}
void loop() 
{
  int data = analogRead(SensorInputPin);
  int dataAfterFilter = myFilter.update(data);  
  int envelope = sq(dataAfterFilter);   
  envelope = (envelope > threshold) ? envelope : 0;    
  if (threshold > 0) 
  {
    if (getEMGCount(envelope)) 
    {  EMG_num++;
      Serial.print("EMG_num: ");
      Serial.println(EMG_num);
    }
  }
    /*
 * 无锡市思知瑞科技有限公司
 * 淘宝店铺：大脑实验室
 * 店铺网址：http://brainlab.taobao.com
 * 工作日联系电话：0510-66759621
 */
  else {
    Serial.println(envelope);
  }
  delayMicroseconds(500);
}
int getEMGCount(int gforce_envelope)
{
  static long integralData = 0;
  static long integralDataEve = 0;
  static bool remainFlag = false;
  static unsigned long timeMillis = 0;
  static unsigned long timeBeginzero = 0;
  static long fistNum = 0;
  static int  TimeStandard = 200;
  integralDataEve = integralData;
  integralData += gforce_envelope;  
  if ((integralDataEve == integralData) && (integralDataEve != 0)) 
  {
    timeMillis = millis();
    if (remainFlag) 
    {
      timeBeginzero = timeMillis;
      remainFlag = false;
      return 0;
    }
      /*
 * 无锡市思知瑞科技有限公司
 * 淘宝店铺：大脑实验室
 * 店铺网址：http://brainlab.taobao.com
 * 工作日联系电话：0510-66759621
 */
    if ((timeMillis - timeBeginzero) > TimeStandard) 
    {
      integralDataEve = integralData = 0;
      return 1;
    }
    return 0;
  }
  else {
    remainFlag = true;
    return 0;
   }
}
/*
 * 无锡市思知瑞科技有限公司
 * 淘宝店铺：大脑实验室
 * 店铺网址：http://brainlab.taobao.com
 * 工作日联系电话：0510-66759621
 */
