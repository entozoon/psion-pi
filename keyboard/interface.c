/*
https://www.raspberrypi.org/forums/viewtopic.php?t=163497#p1062040

 Modified version of 'uinput-sample.c' to demonstrate injecting keypresses
 into linux input subsystem (so allowing keypresses to be 'faked' at the
 linux console). Could be used to, e.g., connect a serial keyboard to
 the system.

 Tested on Raspberry Pi, Oct 2016.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>

#define die(str, args...)   \
    do                      \
    {                       \
        perror(str);        \
        exit(EXIT_FAILURE); \
    } while (0)

int main(void)
{
    int fd;
    struct uinput_user_dev uidev;
    struct input_event ev;
    int dx, dy;
    int i;

    if ((fd = open("/dev/input/uinput", O_WRONLY | O_NONBLOCK)) < 0)
        fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    if (fd < 0)
        die("error: open");

    if (ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0)
        die("error: ioctl");

    if (ioctl(fd, UI_SET_KEYBIT, BTN_LEFT) < 0)
        die("error: ioctl");

    ioctl(fd, UI_SET_KEYBIT, KEY_M);
    ioctl(fd, UI_SET_KEYBIT, KEY_A);
    ioctl(fd, UI_SET_KEYBIT, KEY_D);

    if (ioctl(fd, UI_SET_EVBIT, EV_REL) < 0)
        die("error: ioctl");
    if (ioctl(fd, UI_SET_RELBIT, REL_X) < 0)
        die("error: ioctl");
    if (ioctl(fd, UI_SET_RELBIT, REL_Y) < 0)
        die("error: ioctl");

    memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-sample");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor = 0x1;
    uidev.id.product = 0x1;
    uidev.id.version = 1;

    if (write(fd, &uidev, sizeof(uidev)) < 0)
        die("error: write");

    if (ioctl(fd, UI_DEV_CREATE) < 0)
        die("error: ioctl");

    sleep(1);

    printf("Set up other terminal now.\nWaiting...");
    getchar();

    // **************************************************

    // Keydown
    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_KEY;
    ev.code = KEY_M;
    ev.value = 1;
    if (write(fd, &ev, sizeof(struct input_event)) < 0)
        die("error: write");
    else
        printf("Inject1 %d\n", KEY_M);

    usleep(10000); // 10000 microseconds = 0.01s

    // Keyup
    ev.type = EV_KEY;
    ev.code = KEY_M;
    ev.value = 0;
    if (write(fd, &ev, sizeof(struct input_event)) < 0)
        die("error: write");
    else
        printf("Inject1 %d\n", KEY_M);

    usleep(10000); // 10000 microseconds = 0.01s

    // Each time ??
    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_SYN;
    ev.code = SYN_REPORT;
    ev.value = 0;
    if (write(fd, &ev, sizeof(struct input_event)) < 0)
        die("error: write");

    // **************************************************

    // Keydown
    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_KEY;
    ev.code = KEY_A;
    ev.value = 1;
    if (write(fd, &ev, sizeof(struct input_event)) < 0)
        die("error: write");
    else
        printf("Inject1 %d\n", KEY_A);

    usleep(10000); // 10000 microseconds = 0.01s

    // Keyup
    ev.type = EV_KEY;
    ev.code = KEY_A;
    ev.value = 0;
    if (write(fd, &ev, sizeof(struct input_event)) < 0)
        die("error: write");
    else
        printf("Inject1 %d\n", KEY_A);

    usleep(10000); // 10000 microseconds = 0.01s

    // Each time ??
    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_SYN;
    ev.code = SYN_REPORT;
    ev.value = 0;
    if (write(fd, &ev, sizeof(struct input_event)) < 0)
        die("error: write");

    // **************************************************

    // Keydown
    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_KEY;
    ev.code = KEY_D;
    ev.value = 1;
    if (write(fd, &ev, sizeof(struct input_event)) < 0)
        die("error: write");
    else
        printf("Inject2 %d\n", KEY_D);

    usleep(10000); // 10000 microseconds = 0.01s

    // Keyup
    ev.type = EV_KEY;
    ev.code = KEY_D;
    ev.value = 0;
    if (write(fd, &ev, sizeof(struct input_event)) < 0)
        die("error: write");
    else
        printf("Inject2 %d\n", KEY_D);

    // Each time ??
    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_SYN;
    ev.code = SYN_REPORT;
    ev.value = 0;
    if (write(fd, &ev, sizeof(struct input_event)) < 0)
        die("error: write");

    printf("Waiting...");
    getchar();

    sleep(1);

    if (ioctl(fd, UI_DEV_DESTROY) < 0)
        die("error: ioctl");

    close(fd);

    return 0;
}