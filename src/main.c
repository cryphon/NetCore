#include "sysimp.h"
#include "tuntap.h"
#include "vn_device.h"
#include <pthread.h>

#define BUFLEN 2048
static pthread_t threads[4];


static void create_thread(pthread_t id, void *(*func) (void *))
{
    if (pthread_create(&threads[id], NULL,
                       func, NULL) != 0) {
        perror("Could not create core thread\n");
    }
}

int main(void) {

    tun_init();
    vn_device_init();

    create_thread(0, vn_device_recqueue_loop);


    for (int i = 0; i < 3; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Error when joining threads\n");
            exit(1);
        }
    }

    return 0;
}

