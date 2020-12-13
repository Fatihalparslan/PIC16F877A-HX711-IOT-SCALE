#include <16F877A.h>

#fuses XT, NOWDT, NOPROTECT, NOBROWNOUT, NOLVP, NOPUT, NOWRT, NODEBUG, NOCPD
#use delay(clock = 4 MHz)
#define DT1 pin_c0
#define SCK pin_c1

#define use_portb_lcd TRUE // LCD B portuna baðlý

#include <lcd.c>   // lcd.c dosyasý tanýtýlýyor
 //
//double SCALE = 72118.4;
double SCALE = 36;
unsigned int32 readCount(void) {
  unsigned int32 data;
  unsigned int8 j;
  output_bit(DT1, 1);
  output_bit(SCK, 0);
  data = 0;

  while (input(DT1));
  for (j = 0; j < 24; j++) {
    output_bit(SCK, 1);
    data = data << 1;
    output_bit(SCK, 0);
    if (input(DT1)) {
      data++;
    }
  }
  output_bit(SCK, 1);
  data = data ^ 0x800000;
  output_bit(SCK, 0);
  return data;
}

int32 readAverage(void) {
  unsigned int32 sum = 0;
  for (int k = 0; k < 20; k++) {
    sum = sum + readCount();
  }
  sum = sum / 20;
  return sum;
}

void main() {
  lcd_init(); // LCD'yi hazýrla
  printf(lcd_putc, "\f Teraziyi Bos \nbirakiniz..");
  delay_ms(5000);
  unsigned int32 read1 = 0, offset = 0;
  float gram = 0;
  offset = readAverage();
  while (TRUE) {
    read1 = readAverage();
    if (offset >= read1) {
      float val1 = (offset - read1);
      gram = val1;
    } else {
      float val2 = (read1 - offset);
      gram = val2;

    }
     printf(lcd_putc, "\f%f", gram / SCALE);
    //  printf(lcd_putc, "\f%f", gram);
    delay_ms(500);
  }
}
