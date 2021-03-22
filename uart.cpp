#include "uart.h"

/**
* openUart
* @param  comport ��Ҫ�򿪵Ĵ��ں�
* return  ʧ�ܷ���-1
*/
int openUart(int comport)
{
	int fd;
	const char *dev[] = { "/dev/ttyUSB0", "/dev/ttyTHS2" };
	//��̩�Ƽ�ֻ��������·���ڣ�UART0Ϊ���Կڣ�UART1Ϊ��ͨ���ڣ���������ʹ��UART1
	if (comport == 0)
	{
		fd = open(dev[0], O_RDWR | O_NOCTTY | O_NDELAY);
		if (-1 == fd)
		{
			perror("Can't Open Serial Port");
			return (-1);
		}
	}
	else if (comport == 1)
	{
		fd = open(dev[1], O_RDWR | O_NOCTTY | O_NDELAY);
		if (-1 == fd)
		{
			perror("Can't Open Serial Port");
			return (-1);
		}
	}
	printf("fd-open=%d\n", fd);
	return fd;
}

/**
* uartInit
* @param  nSpeed ������  nBits ֹͣλ nEvent ��żУ��λ nStop ֹͣλ
* @return  ����-1Ϊ��ʼ��ʧ��
*/
int uartInit(int nSpeed, int nBits, char nEvent, int nStop, int fd)
{
	struct termios newtio, oldtio;
	/*����������д��ڲ������ã�������������ںŵȳ���������صĳ�����Ϣ*/
	if (tcgetattr(fd, &oldtio) != 0) {
		perror("SetupSerial 1");
		printf("tcgetattr( fd,&oldtio) -> %d\n", tcgetattr(fd, &oldtio));
		return -1;
	}
	bzero(&newtio, sizeof(newtio));
	/*����һ�������ַ���С*/
	newtio.c_cflag |= CLOCAL | CREAD;
	newtio.c_cflag &= ~CSIZE;
	/*����ֹͣλ*/
	switch (nBits)
	{
	case 7:
		newtio.c_cflag |= CS7;
		break;
	case 8:
		newtio.c_cflag |= CS8;
		break;
	}
	/*������żУ��λ*/
	switch (nEvent)
	{
	case 'o':
	case 'O': //����
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
		break;
	case 'e':
	case 'E': //ż��
		newtio.c_iflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
		break;
	case 'n':
	case 'N':  //����żУ��λ
		newtio.c_cflag &= ~PARENB;
		break;
	default:
		break;
	}
	/*���ò�����*/
	switch (nSpeed)
	{
	case 2400:
		cfsetispeed(&newtio, B2400);
		cfsetospeed(&newtio, B2400);
		break;
	case 4800:
		cfsetispeed(&newtio, B4800);
		cfsetospeed(&newtio, B4800);
		break;
	case 9600:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	case 115200:
		cfsetispeed(&newtio, B115200);
		cfsetospeed(&newtio, B115200);
		break;
	case 460800:
		cfsetispeed(&newtio, B460800);
		cfsetospeed(&newtio, B460800);
		break;
	default:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	}
	/*����ֹͣλ*/
	if (nStop == 1)
		newtio.c_cflag &= ~CSTOPB;
	else if (nStop == 2)
		newtio.c_cflag |= CSTOPB;
	/*���õȴ�ʱ�����С�����ַ�*/
	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 0;
	/*����δ�����ַ�*/
	tcflush(fd, TCIFLUSH);
	/*����������*/
	if ((tcsetattr(fd, TCSANOW, &newtio)) != 0)
	{
		perror("com set error");
		return -1;
	}
	printf("set done!\n");
	return 0;
}

/**
*uartSend
*@param send_buf[] Ҫ���͵����� length ���͵����ݳ���
*/
void uartSend(char send_buf[], int length, int fd)
{
	int w;
	w = write(fd, send_buf, length);
	if (w == -1)
	{
		printf("Send failed!\n");
	}
	else
	{
		printf("Send success!\n");
	}
}
/**
*uartSend
*@param send_buf[] Ҫ���յ����� length ���յ����ݳ���
*/
void uartRead(char receive_buf[], int length, int fd)
{
	int r;
	r = read(fd, receive_buf, strlen(receive_buf));
	for (int i = 0; i < r; i++)
	{
		printf("%c", receive_buf[i]);
	}

}
