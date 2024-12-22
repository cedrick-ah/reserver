#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <pthread.h>
#include <unistd.h>
#include "inotify.h"
#include "libserver.h"

#define EVENT_SIZE  (sizeof(struct inotify_event))
#define BUF_LEN     (1024 * (EVENT_SIZE + 16))

void inotify() {
    char buffer[BUF_LEN];
    int length, i = 0;
    int fd, wd;

    fd = inotify_init();

    if (fd < 0) {
        perror("Error at inotify_init");
    }

    wd = inotify_add_watch(fd, "./static",
    IN_MODIFY | IN_CREATE | IN_DELETE);
    length = read(fd, buffer, BUF_LEN);

    if (length < 0) {
        perror("Error at read");
    }
    
    while (i < length) {
        struct inotify_event *event = (struct inotify_event *) &buffer[i];
        if (event->len) {
            printf("Detected changes...\n");
            update();
        }
        i += EVENT_SIZE + event->len;
    }

    (void) inotify_rm_watch(fd, wd);
    (void) close(fd);
} 

void* inotifyThread(void* vargp)
{
    while (1) {
        inotify();
    }
}

int main(){
    pthread_t inotify_tid;
    pthread_create(&inotify_tid, NULL, inotifyThread, NULL);
    serve();
    exit(0);
}
