#include <U8g2lib.h>
#include <Wire.h>
#define sda 21
#define sdc 22

U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0);
class pos {
public:
  uint8_t x : 7;
  uint8_t y : 6;
};
class ball : public pos {
public:
  bool vx : 1;
  bool vy : 1;
  void update() {
    x += vx ? 1 : -1;
    y += vy ? 1 : -1;
  }
};
class player : public pos {
public:
  bool vy : 1;
  void update() {
    y += vy ? 1 : -1;
  }
};
ball b;
player p[2];
uint8_t score[2] = { 0, 0 };
void setup() {
  Wire.begin(sda, sdc);
  Wire.setClock(100000);
  b.x = 64;
  b.y = 32;
  b.vx = true;
  b.vy = true;
  p[0].x = 0;
  p[0].y = 32;
  p[1].x = 127;
  p[1].y = 32;
  display.begin();
  display.clearBuffer();
  display.setDrawColor(1);
  display.setFont(u8g2_font_5x8_mr);
}
void loop() {
  delay(20);
  display.clearBuffer();
  display.drawCircle(b.x, b.y, 1);
  display.drawBox(p[0].x, p[0].y - 7, 1, 15);
  display.drawBox(p[1].x, p[1].y - 7, 1, 15);
  char buf[4];
  sprintf(buf, "%d", score[0]);
  display.drawStr(10, 10, buf);

  sprintf(buf, "%d", score[1]);
  display.drawStr(74, 10, buf);
  display.sendBuffer();
  for (uint8_t i = 0; i < 2; i++) {
    if (b.x == p[i].x && b.y >= p[i].y - 7 && b.y <= p[i].y + 7) {
      b.vx = !b.vx;
      b.x += b.vx ? 1 : -1;
      score[i]++;
    }
    if (b.y > p[i].y) p[i].vy = true;
    else p[i].vy = false;
    if (p[i].y <= 7) p[i].y = 7;
    if (p[i].y >= 56) p[i].y = 56;
    p[i].update();
  }
  if (b.y == 0 || b.y == 63) b.vy = !b.vy;
  b.update();
}