#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define GPIO_PATH "/sys/class/gpio"

int main() {
    int fd;
    char path[128];
    char *gpio_pin = "24"; 

    printf("Initializing GPIO %s...\n", gpio_pin);

    // 1. Export the GPIO pin to User Space
    fd = open(GPIO_PATH "/export", O_WRONLY);
    write(fd, gpio_pin, strlen(gpio_pin));
    close(fd);

    // 2. Configure as Output
    sprintf(path, GPIO_PATH "/gpio%s/direction", gpio_pin);
    fd = open(path, O_WRONLY);
    write(fd, "out", 3);
    close(fd);

    // 3. Toggle Logic (Blink)
    sprintf(path, GPIO_PATH "/gpio%s/value", gpio_pin);
    for(int i = 0; i < 10; i++) {
        fd = open(path, O_WRONLY);
        write(fd, (i % 2 == 0) ? "1" : "0", 1);
        printf("GPIO Value: %s\n", (i % 2 == 0) ? "HIGH" : "LOW");
        close(fd);
        usleep(500000); 
    }

    // 4. Clean up (Unexport)
    fd = open(GPIO_PATH "/unexport", O_WRONLY);
    write(fd, gpio_pin, strlen(gpio_pin));
    close(fd);

    return 0;
}
