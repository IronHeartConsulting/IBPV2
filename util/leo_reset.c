#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <asm-generic/termios.h>
#include <fcntl.h>
#include <asm-generic/ioctls.h>

int main(int argc, char** argv) {

	int fd;
	struct termios tOptions;
	int modemStatus;
	if (argc <2) {
		printf("Usage: %s <dev name>\n", argv[0]);
	}
	fd = open(argv[1],O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1) {
		perror("couldn't open serial port: ");
		return(3);
	}
	
	fcntl(fd, F_SETFL, 0);
//  ****	tcgetattr(fd,&tOptions);
	cfsetispeed(&tOptions, B1200);
	cfsetospeed(&tOptions, B1200);
	tOptions.c_cflag &= ~PARENB;
	tOptions.c_cflag &= ~CSTOPB;
	tOptions.c_cflag &= ~CSIZE;
	tOptions.c_cflag |= CS8;
	tcsetattr(fd, TCSANOW, &tOptions);
	ioctl(fd, TIOCMGET, &modemStatus);
	modemStatus &= ~TIOCM_DTR;
	ioctl(fd, TIOCMSET, &modemStatus);
	close(fd);
	return(0);

}
