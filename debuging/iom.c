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
        pollret = poll(fds, 1, timeout);

    
        char smallBuffer[10];
        while (fgets(smallBuffer, sizeof(smallBuffer), stdin) != NULL)
        {
                if (strlen(smallBuffer) > 0)
                {
                            // Check if we got the whole line
                            //         if (smallBuffer[strlen(smallBuffer) - 1] == '\n')
                            //                 {
                            //                             // Yes, we got the whole line
                            //                                     }
                            //                                             else
                            //                                                     {
                            //                                                                 // Whole line not read, there is still "excess" input
                            //                                                                         }
                            //                                                                             }
                            //                                                                             }
                            //                                                     }
                            //                 }
                }
        }
    }
    return 0;
}
