// GPIO15 RXD0 = /dev/serial0

// OUTPUT WOOOOOOOOOP (pressing A once)
// Read 1: 0xe0
// Read 1: 0xe0
// Read 1: 0xf
// Read 1: 0xfe
// Read 1: 0xe0
// Read 1: 0xf8
// Read 1: 0xf
// Read 1: 0xfe

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include <errno.h>

int main(void)
{
    int sfd = open("/dev/serial0", O_RDWR | O_NOCTTY);
    struct termios tty;

    // Baud
    cfsetospeed(&tty, B4800);
    cfsetispeed(&tty, B4800);

    //

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK; // disable break processing
    tty.c_lflag = 0;        // no signaling chars, no echo,
                            // no canonical processing
    tty.c_oflag = 0;        // no remapping, no delays
    tty.c_cc[VMIN] = 0;     // read doesn't block
    tty.c_cc[VTIME] = 5;    // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);   // ignore modem controls,
                                       // enable reading
    tty.c_cflag &= ~(PARENB | PARODD); // shut off parity
    tty.c_cflag |= 0;                  // no parity
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    //

    if (sfd == -1)
    {
        printf("Error no is : %d\n", errno);
        printf("Error description is : %s\n", strerror(errno));
        return (-1);
    };

    // char buf[100];
    // int n = read(sfd, buf, sizeof buf); // read up to 100 characters if ready to read

    /* simple noncanonical input */
    printf("Read stuff to the point buffer is filled");
    do
    {
        unsigned char buf[800];
        int rdlen;

        rdlen = read(sfd, buf, sizeof(buf) - 1);
        if (rdlen > 0)
        {
#ifdef DISPLAY_STRING
            buf[rdlen] = 0;
            printf("Read %d: \"%s\"\n", rdlen, buf);
#else /* display hex */
            unsigned char *p;
            printf("Read %d:", rdlen);
            for (p = buf; rdlen-- > 0; p++)
                printf(" 0x%x", *p);
            printf("\n");
#endif
        }
        else if (rdlen < 0)
        {
            printf("Error from read: %d: %s\n", rdlen, strerror(errno));
        }
        /* repeat read to get full message */
    } while (1);
}

//https://stackoverflow.com/a/6947758/3098773
// #include <errno.h>
// #include <fcntl.h>
// #include <string.h>
// #include <termios.h>
// #include <unistd.h>

// int set_interface_attribs(int fd, int speed, int parity)
// {
//     struct termios tty;
//     memset(&tty, 0, sizeof tty);
//     if (tcgetattr(fd, &tty) != 0)
//     {
//         error_message("error %d from tcgetattr", errno);
//         return -1;
//     }

//     cfsetospeed(&tty, speed);
//     cfsetispeed(&tty, speed);

//     tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit chars
//     // disable IGNBRK for mismatched speed tests; otherwise receive break
//     // as \000 chars
//     tty.c_iflag &= ~IGNBRK; // disable break processing
//     tty.c_lflag = 0;        // no signaling chars, no echo,
//                             // no canonical processing
//     tty.c_oflag = 0;        // no remapping, no delays
//     tty.c_cc[VMIN] = 0;     // read doesn't block
//     tty.c_cc[VTIME] = 5;    // 0.5 seconds read timeout

//     tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

//     tty.c_cflag |= (CLOCAL | CREAD);   // ignore modem controls,
//                                        // enable reading
//     tty.c_cflag &= ~(PARENB | PARODD); // shut off parity
//     tty.c_cflag |= parity;
//     tty.c_cflag &= ~CSTOPB;
//     tty.c_cflag &= ~CRTSCTS;

//     if (tcsetattr(fd, TCSANOW, &tty) != 0)
//     {
//         error_message("error %d from tcsetattr", errno);
//         return -1;
//     }
//     return 0;
// }

// void set_blocking(int fd, int should_block)
// {
//     struct termios tty;
//     memset(&tty, 0, sizeof tty);
//     if (tcgetattr(fd, &tty) != 0)
//     {
//         error_message("error %d from tggetattr", errno);
//         return;
//     }

//     tty.c_cc[VMIN] = should_block ? 1 : 0;
//     tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout

//     if (tcsetattr(fd, TCSANOW, &tty) != 0)
//         error_message("error %d setting term attributes", errno);
// }

// ... char *portname = "/dev/ttyUSB1" ... int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
// if (fd < 0)
// {
//     error_message("error %d opening %s: %s", errno, portname, strerror(errno));
//     return;
// }

// set_interface_attribs(fd, B115200, 0); // set speed to 115,200 bps, 8n1 (no parity)
// set_blocking(fd, 0);                   // set no blocking

// write(fd, "hello!\n", 7); // send 7 character greeting

// usleep((7 + 25) * 100); // sleep enough to transmit the 7 plus
//                         // receive 25:  approx 100 uS per char transmit
// char buf[100];
// int n = read(fd, buf, sizeof buf); // read up to 100 characters if ready to read