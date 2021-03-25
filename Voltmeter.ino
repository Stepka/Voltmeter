#include <Wire.h>

/*---------------------- Display --------------------------*/
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 7
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


/*---------------------- Voltmeter --------------------------*/
int analogInput = 1;
float Vout = 0.00;
float Vin = 0.00;
float Vref = 5.0;
float Vavg = 0.00;
float R1 = 10130; //
float R2 = 1091; //
//float R1 = 9800; //
//float R2 = 1093; //
float resistor_ratio = 0.0;
float val = 0;
int val_avg = 0;
float charged_percent = 0; //
int charged_percent_avg = 0; //


byte sync_frame = 0;
#define SYNC_FRAMES 8


/*---------------------- Moving average --------------------------*/
#include <movingAvg.h>
movingAvg avg_voltage(20);    // use 10 data points for the moving average
movingAvg avg_charged(50);    // use 10 data points for the moving average

void setup() {
  pinMode(analogInput, INPUT); //assigning the input port
  Serial.begin(9600); //BaudRate

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // инициализация дисплея по интерфейсу I2C, адрес 0x3C (для OLED 128x32)
  display.clearDisplay(); // очистка дисплея
  display.setTextColor(WHITE); // установка цвета текста

  avg_voltage.begin();
  avg_charged.begin();

  resistor_ratio = (R2 / (R1 + R2));
}


void loop() {

  val = 0;//reads the analog input
  for (int i = 0; i < 20 ; i++) 
  { 
    val = val + analogRead(analogInput);
    delay(3); 
  } 
  val = val / 20; 
  
  Vout = (val * Vref) / 1024.00; // formula for calculating voltage out i.e. V+, here 5.00
  if (Vout < 0.09) //condition
  {
    Vout = 0.00; //statement to quash undesired reading !
  }
  Vin = Vout / resistor_ratio; // formula for calculating voltage in i.e. GND
  Vavg = float(avg_voltage.reading(Vin * 100)) / 100.0;

  Serial.print(val);
  Serial.print(" ");
  Serial.print(Vout);
  Serial.print(" ");
  Serial.print(Vin);
  Serial.println("");

  display.clearDisplay(); // очистить дисплей
  display.setTextSize(1); // установка размера шрифта
  display.setCursor(7, 2); // установка курсора в позицию X = 0; Y = 0
  display.println("Cyber Culture Motors"); // знак цельсия


//  display.setCursor(0, 40); // установка курсора в позицию X = 0; Y = 0
  //  sync_frame++;
  //  if (sync_frame >= SYNC_FRAMES)
  //  {
  //    sync_frame = 0;
  //  }
  //  for ( int i = 0 ; i < sync_frame; i++) // make 5 time 200ms loop, for faster Button response
  //  {
  //    display.print(".");
  //  }

  if (Vavg > 12.7)
  {
    charged_percent = 1.0;
  }
  else if (Vavg > 12.64)
  {
    charged_percent = 0.95;
  }
  else if (Vavg > 12.58)
  {
    charged_percent = 0.9;
  }
  else if (Vavg > 12.52)
  {
    charged_percent = 0.85;
  }
  else if (Vavg > 12.46)
  {
    charged_percent = 0.8;
  }
  else if (Vavg > 12.40)
  {
    charged_percent = 0.75;
  }
  else if (Vavg > 12.36)
  {
    charged_percent = 0.7;
  }
  else if (Vavg > 12.32)
  {
    charged_percent = 0.65;
  }
  else if (Vavg > 12.28)
  {
    charged_percent = 0.6;
  }
  else if (Vavg > 12.24)
  {
    charged_percent = 0.55;
  }
  else if (Vavg > 12.20)
  {
    charged_percent = 0.5;
  }
  else if (Vavg > 12.12)
  {
    charged_percent = 0.4;
  }
  else if (Vavg > 12.04)
  {
    charged_percent = 0.3;
  }
  else if (Vavg > 11.98)
  {
    charged_percent = 0.2;
  }
  else if (Vavg > 11.94)
  {
    charged_percent = 0.1;
  }
  else
  {
    charged_percent = 0.0;
  }
  
  charged_percent_avg = avg_charged.reading(charged_percent * 100);
  
  display.setCursor(20, 16); // установка курсора в позицию X = 0; Y = 0
  display.setTextSize(3); // установка размера шрифта

  if (charged_percent_avg < 10)
  {
    display.print("  "); // выводим значение температуры на экран
  }
  else if (charged_percent_avg < 100)
  {
    display.print(" "); // выводим значение температуры на экран
  }
  display.print(charged_percent_avg);
  display.setTextSize(1); // установка размера шрифта
  display.setCursor(74, 29); // установка курсора в позицию X = 0; Y = 0
  display.print("%");
  

  display.print("  ");
  if (Vavg > -10 && Vavg < 10)
  {
    display.print("0"); // выводим значение температуры на экран
  }
  display.print(Vavg);
//  display.setTextSize(1); // установка размера шрифта
//  display.setCursor(110, 29); // установка курсора в позицию X = 0; Y = 0
  display.print("V");

  display.drawRoundRect(0, display.height() - 20, display.width(), 20, 5, WHITE);
  display.fillRoundRect(3, display.height() - 17, (display.width() - 6) * (charged_percent_avg / 100.0), 14, 3, WHITE);

  display.display(); // всё это отображаем на экране

  delay(1000); //for maintaining the speed of the output in serial moniter
}
