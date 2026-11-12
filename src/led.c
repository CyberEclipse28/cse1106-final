#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>

#define PIN 17
int main(int argc, char* argv[])
{
	gpioInitialise();
	gpioSetMode(PIN,PI_OUTPUT);
	gpioWrite(PIN,1);// LED ON
	getchar(); //waits for Enter key
	gpioWrite(PIN,0); //LED OFF
	getchar(); //waits for Enter key
	gpioTerminate();
	return 0;
}
