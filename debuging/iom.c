#include <common.h>
#include <poll.h>
 
int main(int argc, char *argv[]){

    int fd = 0;
    char buf[11];
    int ret;

    int timeout, pollret;
    struct pollfd fds[1];

    fds[0].fd = fd;
    fds[0].events = 0;
    fds[0].events |= POLLIN;

    timeout = 1000;

    while (1){
        printf("%c", getc(stdin));
    }
    

    // while (1){
    //     // pollret = poll(fds, 1, timeout);
    //     while (fgets(buf, sizeof(buf), stdin) != NULL){
    //         if (strlen(buf) > 0){
    //             // Check if we got the whole line
    //             if (buf[strlen(buf) - 1] == '\n'){
                    
    //             }else{
    //             }
    //                 printf("%s", buf);
    //         }
    //     }
    // }
    return 0;
}
