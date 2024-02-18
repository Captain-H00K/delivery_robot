#include <iostream>
#include <stdio.h>
#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */ 
#include <errno.h>   /* ERROR Number Definitions           */
#include <stdlib.h>

using namespace std;

uint8_t msg[6];

enum comands {
    SEND_MSG,
    PING,
    UnknownErr,
    DataErr,
    ConnectErr,
    GyroCalibrate,
    NONE
};

comands currentSTATE = NONE;


struct incomingDate {           //структура для чтения данных
  uint8_t comandCode;
  uint16_t value;
  uint8_t direct;
  uint16_t second_value;
};

/*Сейчас стоит задача ожидать данные в буфере ядра, и если их нет то выходить по какому-то таймауту, 
возвращая управления программе. Для этого напишем следующую функцию.
На вход функция принимает файловый дескриптор открытого порта, 
длину запрашиваемых данных, таймаут в миллисекундах, после которого возвращается 
управление системе и указатель на буфер, куда будут считаны данные*/

/*СКОРЕЕ ВСЕГО НАМ ЭТО НАХУЙ НЕ НАДО БУДЕТ

int read_com(int fd, int len , int timeout, uint8_t * buff){ //НАХУЙ БЛЯТЬ НЕ НАДО ааааааааааааааааааааааааааааааааааа
    int ret = 0;
        
    struct pollfd fds;          //Управление осуществляется с помощью структуры
    fds.fd=fd;                  //записываем файловый дескриптор, который хотим мониторить
    fds.events = POLLIN;        //Указываем, то что мы хотим мониторить событие получение данных
    poll(&fds, 1, timeout);     //взводим наш «сторожевой таймер»
    if(fds.revents & POLLIN) {
        ret = read(fd, buff, len);
    }
    if(ret<0){
        ret = 0;
    }
    return ret; 
}*/

void readTermCommand(uint8_t& _recived) {                         //просто для тестового запуска
    uint8_t command;
    std::cin >> "Введите номер команды" >> command;
    _recived[0] = command;
}

void readIncomingMessage(int fd, uint8_t& buff) {
    read(fd, buff, 6);
    printf("\n  принятые данные", buff)
}

void main(void) {

    int fd; /*File Descriptor*/
    fd = open(comport, O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd < 0){														/* Could not open the port */
		printf("\n  Error! in Opening ttyUSB0  ");
		exit(0);
	} 

    printf("\n  ttyUSB0 Opened Successfully "); //порт открыт удачно

    struct termios SerialPortSettings; //создаём структуру порта для дальнейшей её настройки
    tcgetattr(fd, &SerialPortSettings);	/* Get the current attributes of the Serial port */

    cfsetispeed(&SerialPortSettings,B115200); /* Set Read  Speed as 9600                       */
	cfsetospeed(&SerialPortSettings,B115200); /* Set Write Speed as 9600                       */

    SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
	SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
	SerialPortSettings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
	SerialPortSettings.c_cflag |=  CS8;      /* Set the data bits = 8                                 */
    SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
	SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */ 
    SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
	SerialPortSettings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode */      
    SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/

    SerialPortSettings.c_cc[VMIN] = 0; 
	SerialPortSettings.c_cc[VTIME] = 0; 

    if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
		printf("\n  ERROR ! in Setting attributes");
	else
        printf("\n  BaudRate = 9600 \n  StopBits = 1 \n  Parity   = none");

    

    while(1) {

        readTermCommand((byte*)&incomingDate);

        readIncomingMessage((byte*)msg);    //пока неактивно, в рамках проверки пересылки данных

        currentSTATE = incomingDate.comandCode;

        switch (currentSTATE)               //машина состояний для отправки данных к контроллеру от рос
        {
        case SEND_MSG:
            write(fd, (uint8_t*)incomingDate, sizeof(incomingDate));
            break;
        
        case PING:
            incomingDate.comandCode = 255;
            write(fd, (uint8_t*)incomingDate, sizeof(incomingDate));
            break;

        case GyroCalibrate:
            incomingDate.comandCode = 203;
            write(fd, (uint8_t*)incomingDate, sizeof(incomingDate));
            break;

        case NONE:
            break;

        default:
            break;
        }

        switch ()                       //машина состояний для ответа контроллеру 
        {

        }
    }
}