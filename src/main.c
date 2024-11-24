#include "sysimp.h"
#include "tuntap.h"

#define BUFLEN 2048

int main(void) {

    tun_init();
    char* device = "/dev/net/tun0";  // Check and update as needed



    char buffer[BUFLEN];
    printf("Reading from device: %s\n", device);

    while (1) {
        // Read data from the TUN/TAP device
        int len = tun_read(buffer, sizeof(buffer));
        if (len < 0) {
            perror("Error reading from device");
            break;
        }

        if (len == 0) {
            printf("No data available. Exiting.\n");
            break;
        }

        // Print the received data in hexadecimal format
        printf("Received %d bytes:\n", len);
        for (int i = 0; i < len; i++) {
            printf("%02x ", (unsigned char)buffer[i]);
            if ((i + 1) % 16 == 0) printf("\n");  // Pretty-print 16 bytes per line
        }
        printf("\n");
    }


    printf("Device closed. Exiting...\n");
    return 0;
}

