const int MAX_LENGTH = 21;

extern U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;
extern Time T;
extern String TimeMinHour;

class COM_console {

    String text [7];
    long int pushesCounter;
    String buff;

  public:
    COM_console();
    void showStr(String NewStr);
    String getStr();
    void sendStr(String NewStr);
    void drawStr(String buff);
    void upDate();
};


COM_console::COM_console() {

  for (int i = 0; i < 7; i++) {
    text[i] = "";
  }
  pushesCounter = 0;
  buff = "";
}

void COM_console:: showStr(String NewStr) {
  for (int i = 0; i < 6; i++) {
    text[i] = text[i + 1];
  }
  text[6] = NewStr;

  u8g2.clearBuffer();
  
  u8g2.setFont(u8g2_font_6x10_tr);
  for (int y = 0; y <= 6; y++) {
    u8g2.drawStr(0, 8 + (pushesCounter > 5) * (y * 10 - 6) + (pushesCounter <= 5) * (y - 6 + pushesCounter) * 10, text[y].c_str());
  }
  TimeMinHour = T.getTimeMinHour();
  u8g2.setFont(u8g2_font_osb18_tn);
  int MinHourLenth = u8g2.getStrWidth(TimeMinHour.c_str());
  u8g2.setDrawColor(0);
  u8g2.drawBox(58 - MinHourLenth/2, 0, MinHourLenth + 10*( MinHourLenth != 0) , 22);
  u8g2.setDrawColor(1);
  u8g2.drawStr(63 - MinHourLenth/2, 20, TimeMinHour.c_str());
 /* if (T.getSec()%2){
     u8g2.setDrawColor(0);
     u8g2.drawBox(60, 0, 6 , 20);
     u8g2.setDrawColor(1);
  }*/
  u8g2.sendBuffer();
  pushesCounter++;
}

String COM_console :: getStr() {
  while (1)
  {
    if (Serial.available()) {
      char tmp = Serial.read();
      if (tmp == '\n') {

        int l = buff.length();
        int numbStrok = int(l / MAX_LENGTH) + (l % MAX_LENGTH != 0);
        if (numbStrok == 0) numbStrok = 1;
        for (int strok = 0; strok < numbStrok; strok++) {
          showStr(buff.substring(MAX_LENGTH * strok, MAX_LENGTH * (strok + 1)));
        }

        String temp = buff;
        buff = "";
        return temp;
      }
      else
      {
        buff += tmp;
      }
    }
  }
}

void COM_console :: drawStr(String buff) {
  u8g2.setFont(u8g2_font_6x10_tr);
  int l = buff.length();
  int numbStrok = int(l / MAX_LENGTH) + (l % MAX_LENGTH != 0);
  if (numbStrok == 0) numbStrok = 1;
  for (int strok = 0; strok < numbStrok; strok++) {
    showStr(buff.substring(MAX_LENGTH * strok, MAX_LENGTH * (strok + 1)));
  }
}

void COM_console:: sendStr(String NewStr) {
  Serial.println(NewStr.c_str());
  showStr("> " + NewStr);
}

void COM_console:: upDate(){
  u8g2.clearBuffer();
  
  u8g2.setFont(u8g2_font_6x10_tr);
  for (int y = 0; y <= 6; y++) {
    u8g2.drawStr(0, 8 + (pushesCounter > 5) * (y * 10 - 6) + (pushesCounter <= 5) * (y - 6 + pushesCounter) * 10, text[y].c_str());
  }
  TimeMinHour = T.getTimeMinHour();
  u8g2.setFont(u8g2_font_osb18_tn);
  int MinHourLenth = u8g2.getStrWidth(TimeMinHour.c_str());
  u8g2.setDrawColor(0);
  u8g2.drawBox(58 - MinHourLenth/2, 0, MinHourLenth + 10*( MinHourLenth != 0) , 22);
  u8g2.setDrawColor(1);
  u8g2.drawStr(63 - MinHourLenth/2, 20, TimeMinHour.c_str());
  
  u8g2.sendBuffer();
}
