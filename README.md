# sound-recognition-and-barrier-control
Automatic barrier opening by the sound of the alarm of rescue services and barrie opening via the "BLYNK" mobile app.

The software and hardware identification system for the Internet of Things technology is designed to automatically open the road barrier when recognizing the service alarms. This system is designed to save time for ambulances, police and firefighters. Additionally, the barrier can be opened from the BLYNK app installed on mobile phones (Android or iOS).

The software and hardware system will recognize the emergency alarm within 2 seconds, also with the help of the BLYNK application will be able to track the opening time of the barrier and open the barrier through a button.
The main module of the system is the Arduino Mega 2560, which connects to the Voice Recognition Module V.3, the GSM SIM800L module that connects to the Internet, the OLED Display SH1106 message output display, and the Micro Servo SG90 micro-servo drive to simulate the opening of the barrier.
A standalone emulator of sound playback based on the Mini DFPlayer module will be created to play the sounds, which includes sounds using speakers and has volumes control, pause and turn on all sounds completely buttons.
The software for initializing the sound was developed in the Arduino IDE environment in the C language.

 Practical value of the project:
• The system that supplies 220V power;
• Autonomous system emulator can have up to 14 sounds;
• The system is able to play only 7 sounds at a time;
• Sound duration can be up to 2.5 seconds;
• Receiving frequency – 800-2500 Hz.
The database is located directly in the Voice Recognition V.3 module, so no additional software is required.
The system must have the Internet to perform the work. It is provided by GSM module SIM800L, to which a SIM is inserted with the tariff "Technika", providing 2 GB of Internet. This is enough to keep the system running smoothly, otherwise you can connect the system to WI-FI via the ESP8266 module.

Pin connection:
![image](https://github.com/mirabud/sound-recognition-and-barrier-control/assets/52883647/6de37f59-9d0c-452a-9461-66d9654ff6bc)

UPD: BLYNK removed the free version. However, instead of the app, the barrier can also be controlled by the Telegram chat bot.

If you want to know more about the deployment or have questions, feel free to email mira.bud01@gmail.com
