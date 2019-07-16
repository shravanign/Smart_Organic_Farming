#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main() 
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, AD5259 I2C address is 0x18(24)
	ioctl(file, I2C_SLAVE, 0x18);

	// Send Start Command(0x00)
	// Input resistance value
	char config[2] = {0};
	config[0] = 0x00;
	config[1] = 0x80;
	write(file, config, 2);
	sleep(1);

	// Read 1 bytes of data from the register(0x00)
	// temp msb, temp lsb
	char reg[1] = {0x00};
	write(file, reg, 1);
	char data[1] = {0};
	if(read(file, data, 1) != 1)
	{
		printf("Erorr : Input/output Erorr \n");
	}
	else
	{
		// Convert the data
		float resistance = (data[0] / 256.0) * 10.0;

		// Output data to screen
		printf("Resistance : %.2f K \n", resistance);
	}
}
