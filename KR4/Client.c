//
//  Client.c
//  KR4
//
//  Created by Nikita-Mac on 07.05.17.
//  Copyright © 2017 Nikita-Mac. All rights reserved.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MSGSZ     128
#define MSGKEY    665

typedef struct msgform {
    long    mtype;
    char    mtext[MSGSZ];
} message_buf;

int main(int argc, const char * argv[]) {
    
    int msqid;
    message_buf sbuf;
    message_buf  rbuf;
    size_t buf_length;
    
    if ((msqid = msgget(MSGKEY, 0777|IPC_CREAT)) < 0) {
        perror("msgget");
        exit(1);
    }
    printf("\nClient - start");
    int number = 0;
    
    while (1) {
        
        if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }
        printf("%s", rbuf.mtext);
        number = atoi(rbuf.mtext);
        
        if (number == -1)
            break;
        
        for ( int i = 2; i*i <= number; i ++ ){
            if ( number % i == 0 ){
                strcpy(sbuf.mtext, "Not Simple");
                break;
            }
            strcpy(sbuf.mtext, "Simple");
        }
        
        buf_length = strlen(sbuf.mtext) + 1;
        
        if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
            perror("msgsnd");
            exit(1);
        }
        
        printf("\nAnswer - send");
    }
    
    return 0;
}

