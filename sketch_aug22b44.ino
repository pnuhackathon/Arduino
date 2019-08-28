const int motor_L = 5;
const int motor_R = 6;
int green = 2;
int yellow = 4;
int onefloor = 750; // ÇÑ Ãþ °¡ŽÉµ¥ ÇÊ¿äÇÑ ÈžÀüŒö, ÃøÁ€ÇØŸß ÇÔ
int ele_state = 0; // 0Àº žØÃã, 1ŽÂ À§·Î, 2ŽÂ ŸÆ·¡·Î
int ele_loc = 1; // ¿€ž®º£ÀÌÅÍ À§Ä¡
boolean chkfloor[5][2]; // ¹öÆ°ÀÌ Ž­·¯Á® ÀÖŽÂ°¡. Ž­·¯Á® ÀÖÀžžé true [0]ÀÌ À§, [1]ÀÌ ŸÆ·¡
boolean in_floor[5]; // ¿€ž®º£ÀÌÅÍ ³»ºÎ ¹öÆ°
int callSign[4]; // ¿€ž®º£ÀÌÅÍ ¿ìŒ± È£ÃâœÃ ÃþŒö ÀúÀå žÕÀú ºÎž¥ »ç¶÷ÀÌ ŸÕ¿¡ µéŸî°¡ ÀÖÀœ
boolean isCalled = false; // Ž©°¡ ¿ìŒ± È£ÃâÇßŽÂÁö ¿©ºÎ È®ÀÎ
boolean MoveToCall = false; // ¿ìŒ± È£ÃâÇÑ °÷Àž·Î ÀÌµ¿ Áß?
int numCalled = 0; // ¿ìŒ± È£Ãâ ŒýÀÚ

unsigned long previousMillis = 0;
const long delayTime = 2000;
int sigcnt = 0;
char sigarr[2];
 void chkSignal() {
    if (Serial.available())
    {
   	 sigarr[sigcnt] = Serial.read();
   	 sigcnt++;
         
   	 if (sigcnt == 1 && sigarr[0] >= 49 && sigarr[0] <= 53) {
   		 in_floor[sigarr[0] - 48] = 1;
   		 sigcnt = 0;
   	 }
   	 if (sigcnt == 2) {
                 
   		 if (sigarr[0] == 'u') {
   			 chkfloor[sigarr[1] - 48][0] = 1;
                      
   		 }
   		 if (sigarr[0] == 'd') {
   			 chkfloor[sigarr[1] - 48][1] = 1;
                        
   		 }
   		 if (sigarr[0] == 'c') {
   			 callSign[numCalled] = sigarr[1] - 48;
   			 numCalled ++;
   			 isCalled = true;
                         digitalWrite(green, LOW);
                         digitalWrite(yellow, HIGH);
                       Serial.print("4");
   		 }
   		 sigcnt = 0;
   	 }
    }
}

void setup() {
  Serial.begin(9600);
  pinMode(motor_L, OUTPUT);                  // L_motor_LÀ» Ãâ·Âžðµå·Î Œ³Á€ÇÕŽÏŽÙ.
  pinMode(motor_R, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  digitalWrite(green, HIGH);
  digitalWrite(yellow, LOW);
  //chkfloor[][1] = 1;
}

void loop() {
    chkSignal();
    if (ele_state == 0) {
      analogWrite(motor_L, 0);                    // L_motor_L¿¡ 0(0V)ÀÇ œÅÈ£žŠ Ãâ·ÂÇÕŽÏŽÙ.
      analogWrite(motor_R, 0);
    }
    if (ele_state == 2)   {
      analogWrite(motor_L, 0);                    // L_motor_L¿¡ 0(0V)ÀÇ œÅÈ£žŠ Ãâ·ÂÇÕŽÏŽÙ.
      analogWrite(motor_R, 150);
    }
    if (ele_state == 1)   {
          analogWrite(motor_L, 150);                    // L_motor_L¿¡ 0(0V)ÀÇ œÅÈ£žŠ Ãâ·ÂÇÕŽÏŽÙ.
          analogWrite(motor_R, 0);
    }
    unsigned long currentMillis = millis();
    previousMillis = currentMillis;
    while (ele_state == 1 && currentMillis - previousMillis < 900) {
 	chkSignal();
        currentMillis = millis();
    }
    
    while (ele_state == 2 && currentMillis - previousMillis < 390) {
 	chkSignal();
        currentMillis = millis();
    }

    // ÇöÀç Ãþ¿¡Œ­ Á€Â÷?
    if (ele_state == 1) { // ÇöÀç ¿€ž®º£ÀÌÅÍ°¡ ¿Ã¶ó°¡°í ÀÖÀ» ¶§
   	 ele_loc++;
   	 if (chkfloor[ele_loc][0] == 1 || in_floor[ele_loc] == 1) { // žØÃçŸßÇÒ Ãþ
   		 analogWrite(motor_L, 0);                    // L_motor_L¿¡ 0(0V)ÀÇ œÅÈ£žŠ Ãâ·ÂÇÕŽÏŽÙ.
                 analogWrite(motor_R, 0);
                 chkfloor[ele_loc][0] = 0;
   		 in_floor[ele_loc] = 0;
   		 currentMillis = millis();
                 previousMillis = currentMillis;
   		 while (currentMillis - previousMillis < delayTime) {
   			
                         chkSignal();
                         currentMillis = millis();
   		 }
   		 //delay(3000); // Á€Â÷
   	 }
         boolean CanStop = true;
         for (int i = ele_loc + 1; CanStop && i <= 4; i++) {
   		 chkSignal();
   		 if (in_floor[i] == 1) CanStop = false;
   		 if (chkfloor[i][0] == 1) CanStop = false;
   	 }
       if (CanStop && chkfloor[ele_loc][1] == 1) {
                ele_state=2;
		chkfloor[ele_loc][1] = 0;
                 analogWrite(motor_L, 0);                    // L_motor_L¿¡ 0(0V)ÀÇ œÅÈ£žŠ Ãâ·ÂÇÕŽÏŽÙ.
                 analogWrite(motor_R, 0);
                 previousMillis = currentMillis;
       		 while (currentMillis - previousMillis < delayTime) {
       			 chkSignal();
                         currentMillis = millis();
       		 }
	}
    }
    else if (ele_state == 2) { // ¿€ž®º£ÀÌÅÍ°¡ ³»·Á°¡°í ÀÖÀ» ¶§
   	 ele_loc--;
   	 if (chkfloor[ele_loc][1] == 1 || in_floor[ele_loc] == 1) { // žØÃçŸßÇÒ Ãþ
   		 analogWrite(motor_L, 0);                    // L_motor_L¿¡ 0(0V)ÀÇ œÅÈ£žŠ Ãâ·ÂÇÕŽÏŽÙ.
                 analogWrite(motor_R, 0);
                 chkfloor[ele_loc][1] = 0;
   		 in_floor[ele_loc] = 0;
                 currentMillis = millis();
                 previousMillis = currentMillis;
   		 while (currentMillis - previousMillis < delayTime) {
   			 chkSignal();
                         currentMillis = millis();
   		 }
   		 //delay(3000); // Á€Â÷
   	 }
       boolean CanStop = true;
       for (int i = ele_loc - 1; CanStop && i >= 1; i--) {
   		 chkSignal();
   		 if (in_floor[i] == 1) CanStop = false;
   		 if (chkfloor[i][1] == 1) CanStop = false;
   	 }
	if (CanStop && chkfloor[ele_loc][0] == 1) {
                ele_state=1;
		chkfloor[ele_loc][0] = 0;
                 analogWrite(motor_L, 0);                    // L_motor_L¿¡ 0(0V)ÀÇ œÅÈ£žŠ Ãâ·ÂÇÕŽÏŽÙ.
                 analogWrite(motor_R, 0);
                 previousMillis = currentMillis;
       		 while (currentMillis - previousMillis < delayTime) {
       			 chkSignal();
                         currentMillis = millis();
       		 }
	}
	
    }

    // Žõ °¥Áö, žØÃâÁö, ¹æÇâ ¹Ù²ÜÁö
    // ¹æÇâ À¯Áö?
    boolean CanStop = true;
    if (ele_state == 1) {
   	 for (int i = ele_loc + 1; CanStop && i <= 4; i++) {
   		 chkSignal();
   		 if (in_floor[i] == 1) CanStop = false;
   		 if (chkfloor[i][0] == 1) CanStop = false;
   	 }
	if (CanStop && chkfloor[ele_loc][1] == 1) {
                ele_state=2;
		chkfloor[ele_loc][1] = 0;
                 analogWrite(motor_L, 0);                    // L_motor_L¿¡ 0(0V)ÀÇ œÅÈ£žŠ Ãâ·ÂÇÕŽÏŽÙ.
                 analogWrite(motor_R, 0);
                 previousMillis = currentMillis;
       		 while (currentMillis - previousMillis < delayTime) {
       			 chkSignal();
                         currentMillis = millis();
       		 }
	}
    }
    else if (ele_state == 2) {
   	 for (int i = ele_loc - 1; CanStop && i >= 1; i--) {
   		 chkSignal();
   		 if (in_floor[i] == 1) CanStop = false;
   		 if (chkfloor[i][1] == 1) CanStop = false;
   	 }
	if (CanStop && chkfloor[ele_loc][0] == 1) {
                ele_state=1;
		chkfloor[ele_loc][0] = 0;
                 analogWrite(motor_L, 0);                    // L_motor_L¿¡ 0(0V)ÀÇ œÅÈ£žŠ Ãâ·ÂÇÕŽÏŽÙ.
                 analogWrite(motor_R, 0);
                 previousMillis = currentMillis;
       		 while (currentMillis - previousMillis < delayTime) {
       			 chkSignal();
                         currentMillis = millis();
       		 }
	}

    }

    // ¹ÝŽë ¹æÇâ?
   // ¿ø·¡ ³»ºÎ ¹öÆ°Àº ŽÙ ²šÁö±ä ÇÏŽÂµ¥...
    if (CanStop && ele_state == 1) {
   	 for (int i = ele_loc - 1; CanStop && i >= 1; i--) {
   		 chkSignal();
   		 if (in_floor[i] == 1) CanStop = false;
   		 if (chkfloor[i][1] == 1) CanStop = false;
   	 }
   	 if (!CanStop) ele_state = 2;
    }
    else if (CanStop && ele_state == 2) {
   	 for (int i = ele_loc + 1; CanStop && i <= 4; i++) {
   		 chkSignal();
   		 if (in_floor[i] == 1) CanStop = false;
   		 if (chkfloor[i][0] == 1) CanStop = false;
   	 }
   	 if (!CanStop) ele_state = 1;
    }
    if (CanStop) { //Á€Áö?
      in_floor[ele_loc] =0;
      chkfloor[ele_loc][0] = 0;
      chkfloor[ele_loc][1] = 0;
   	 ele_state = 0;
      analogWrite(motor_L, 0);                    // L_motor_L¿¡ 0(0V)ÀÇ œÅÈ£žŠ Ãâ·ÂÇÕŽÏŽÙ.
      analogWrite(motor_R, 0);
    }

    if (ele_state == 0) {
           analogWrite(motor_L, 0);                    // L_motor_L¿¡ 0(0V)ÀÇ œÅÈ£žŠ Ãâ·ÂÇÕŽÏŽÙ.
           analogWrite(motor_R, 0); // žØÃá ¿€ž®º£ÀÌÅÍ¿¡ ¹öÆ° Ž©žŠ¶§, µ¿œÃ¿¡ Ž©ž£ŽÂ ÀÏÀº ŸøŽÂ °É·Î °¡Á€
   	 for (int i = ele_loc + 1; i <= 4; i++) {
   		 chkSignal();
   		 if (in_floor[i] == 1 || chkfloor[i][0] || chkfloor[i][1]) ele_state = 1;
   	 }
   	 for (int i = ele_loc - 1; i >= 1; i--) {
   		 chkSignal();
   		 if (in_floor[i] == 1 || chkfloor[i][0] || chkfloor[i][1]) ele_state = 2;
   	 }
    }

    // À§¿¡ŽÂ ¿ìŒ± È£ÃâÀÌ ŸøŽÙŽÂ °¡Á€ÀÏ ¶§ ŸË°íž®Áò, ³ªÁß¿¡ °íÃÄŸß ÇÔ
    if (isCalled) { // Ž©°¡ È£ÃâœÃ ³»ºÎ ¹öÆ°žž Ã³ž®ÇÏ°í ÅÂ¿ì·¯ °š
   	 boolean isNoOne = true; // ¿€ž®º£ÀÌÅÍ¿¡ ¹öÆ° Ž­·¯Á® ÀÖŽÂÁö °Ë»ç
   	 //if (ele_state == 1) {
   		 for (int i = ele_loc + 1; i <= 4 && isNoOne; i++) {
   			 chkSignal();
   			 if (in_floor[i] == 1) isNoOne = false;
                 Serial.print("2");
   		 }
   	 //}
   	 //else if (ele_state == 2) {
   		 for (int i = ele_loc - 1; i >= 1 && isNoOne; i--) {
   			 chkSignal();
   			 if (in_floor[i] == 1) isNoOne = false;
                         Serial.print("!");
   		 }
   	 //}
         if (ele_state!=0) isNoOne = false;
   	 if (isNoOne) {
   		 if (ele_loc > callSign[0]) {
   		      analogWrite(motor_L, 0);                    // L_motor_L¿¡ 0(0V)ÀÇ œÅÈ£žŠ Ãâ·ÂÇÕŽÏŽÙ.
                      analogWrite(motor_R, 150);
   		 }
   		 else if (ele_loc < callSign[0]) {
                      analogWrite(motor_L, 150);                    // L_motor_L¿¡ 0(0V)ÀÇ œÅÈ£žŠ Ãâ·ÂÇÕŽÏŽÙ.
                      analogWrite(motor_R, 0);
   		 }
                currentMillis = millis();
                previousMillis = currentMillis;
                int movetime = 900;
                if (ele_loc - callSign[0] > 0) movetime = 390;
                while (currentMillis - previousMillis < movetime * abs(ele_loc - callSign[0])) {
               	    chkSignal();
                    currentMillis = millis();
                }
                ele_loc = callSign[0];
                 analogWrite(motor_L, 0);                    // L_motor_L¿¡ 0(0V)ÀÇ œÅÈ£žŠ Ãâ·ÂÇÕŽÏŽÙ.
                 analogWrite(motor_R, 0);
                 
                 for (int i=numCalled-1;i>=1;i--) {
                 callSign[i-1] = callSign[i];
               }
               callSign[numCalled-1] = 0;
               if (!callSign[numCalled-1]) {
                 isCalled = false;
                 digitalWrite(green, HIGH);
                 digitalWrite(yellow, LOW);
               }
                 
                 
                 previousMillis = currentMillis;
       		 while (currentMillis - previousMillis < delayTime) {
       			 chkSignal();
                             currentMillis = millis();
             }
           
               numCalled--;
       	 }
           
    }


}

