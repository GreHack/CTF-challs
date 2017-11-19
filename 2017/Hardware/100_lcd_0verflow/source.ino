#include <LiquidCrystal.h>

#define rs 11
#define rw 12
#define enable 13
#define d0 2
#define d1 3
#define d2 4
#define d3 5
#define d4 6
#define d5 7
#define d6 8
#define d7 9

LiquidCrystal lcd(rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7);

void setup() {
  lcd.begin(16, 2);
  lcd.print("flag> GH17{LcDII_3az1Er_Re4D_7haN_C4p}");
}

void loop() {
}
