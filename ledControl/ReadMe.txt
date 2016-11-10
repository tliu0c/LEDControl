The Acer ac100 is a cool little server that fits on any desk. It has 8 LED indicators, 4 harddrive LEDs and 4 status LEDs. However Acer's software does not seem to be making use of them. It would be nice to put then to full use. So I wrote this little program.

HDD LEDs
This program will change the color of each HDD's LED based on activity. Green indicates idle, amber inidcates light avitivity and red inidcates heavy acitivty. Refresh time(33ms) and data R/W threshold can be changed in code. You should change the HDD drive letter in HDDStatus.h.

CPU LED
The (i) iniformation LED will flash and change color based on CPU load and temperature. When the CPU is idle, it will stay solid. When CPU load is > 5%, it will flash and the flash frequency will increase as CPU load increases. The color of this LED will change from green to amber(55 C) to red(75 C) as CPU temperature increases.


To use this, you must install CoreTemp for getting CPU info. http://www.alcpu.com/CoreTemp/
You can have this program start automatically during bootup by adding LedStartupInit.bat to Task scheduler with trigger on "At log on of any user". Remeber to open it up and edit the paths to fit your system.
