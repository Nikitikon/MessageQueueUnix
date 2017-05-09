//
//  Serv.c
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
    int number = 0;
    
    while (1) {
        
        scanf("%d", &number);
        sprintf(sbuf.mtext,"%120.0d", number);
        buf_length = strlen(sbuf.mtext) + 1 ;
        
        if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
            perror("msgsnd");
            exit(1);
        }
        
        if (number == -1)
            break;
        
        if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }
        
        printf("\n%d - %s", number, rbuf.mtext);
    }
    
    return 0;
}

