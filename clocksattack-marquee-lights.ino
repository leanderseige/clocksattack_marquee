#define RUNRIGHT 0
#define RUNLEFT 1
#define ALLBLINK 2
#define KNIGHTRIDER 3
#define DOUBLERIDER 4
#define ALLRANDOM 5
#define ALLON 6
#define ALLOFF 7

#define GLOBAL_ANIM 0
#define GLOBAL_ON 1
#define GLOBAL_OFF 2

uint8 LEDS[] = {D0, D1, D2, D3, D4, D5, D6, D7};
byte onxt = 0x00;
byte oold = 0x00;
int globalmode = GLOBAL_ANIM;
int ledmode = ALLBLINK;
int lastmode = -1;
long starttime = 0;

void setup()
{
  for (int x = 0; x < 8; x++) {
    pinMode(LEDS[x], OUTPUT);
  }

  pinMode(D0, INPUT_PULLUP); 
  starttime = millis();
}

void loop()
{
  float v;

  int switchStatus = digitalRead(D8);   
  if (switchStatus != LOW) {
    globalmode = (globalmode+1)%3;
  }
  
  switch (globalmode) {

    case GLOBAL_ANIM:
      ledmode = ((millis() - starttime) / 9000) % 6;
      if (ledmode != lastmode) {
        onxt = 0x00;
        oold = 0x00;
        lastmode = ledmode;
      }
      break;

    case GLOBAL_OFF:
      ledmode = ALLOFF;
      break;

    case GLOBAL_ON:
    default:
      ledmode = ALLON;
      break;
  }

  switch (ledmode) {

    case RUNRIGHT:
      oold = onxt;
      onxt = onxt << 1;
      if (onxt == 0x00) {
        onxt = 0x01;
      }
      break;

    case RUNLEFT:
      oold = onxt;
      onxt = onxt >> 1;
      if (onxt == 0x00) {
        onxt = 0x80;
      }
      break;

    case KNIGHTRIDER:
      oold = onxt;
      v = 1.0 + sin(TWO_PI * millis() / 2000);
      onxt = 0x01 << (int)round(7 * (v / 2));
      break;

    case DOUBLERIDER:
      oold = onxt;
      v = 1.0 + sin(TWO_PI * millis() / 1000);
      onxt = 0x01 << (int)round(3 * (v / 2));
      onxt |= (onxt & 0x01) << 7;
      onxt |= (onxt & 0x02) << 5;
      onxt |= (onxt & 0x04) << 3;
      onxt |= (onxt & 0x08) << 1;
      break;

    case ALLRANDOM:
      onxt = (byte)random(0, 255);
      oold = onxt;
      break;

    case ALLON:
      onxt = 0xFF;
      oold = 0xFF;
      break;

    case ALLOFF:
      onxt = 0x00;
      oold = 0x00;
      break;

    case ALLBLINK:
    default:
      onxt = ~onxt;
      oold = onxt;
      break;
  }

  setAll();
  delay(100); // animation delay and button debounce
}


void setAll() {
  byte temp;
  temp = onxt | oold;
  for (int x = 0; x < 8; x++) {
    digitalWrite(LEDS[x], (temp >> x & 0x01 == 1) ? HIGH : LOW );
  }
}

