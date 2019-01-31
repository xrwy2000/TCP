#include "tcp_net_socket.h"
int tcp_init(const char *ip,int port){
    int sfd = socket(AF_INET,SOCK_STREAM,0);
    if(-1==sfd){
        perror("socket");
        exit(-1);
    }
    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(struct sockaddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(port);//转化为network byte order
    serveraddr.sin_addr.s_addr = inet_addr(ip);//得到网络字节序地址
    if(bind(sfd,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr))==-1){
        perror("bind");
        close(sfd);
        exit(-1);
    }

    if(listen(sfd,10)==-1){
        perror("listen");
        close(sfd);
        exit(-1);
    }
    return sfd;//返回sfd对象
}

//服务端的接收
int tcp_accept(int sfd){
    struct sockaddr_in clientaddr;//获取客户端信息
    memset(&clientaddr,0,sizeof(struct sockaddr));
    socklen_t addrlen = sizeof(struct sockaddr);
    int new_fd = accept(sfd,(struct sockaddr*)&clientaddr,&addrlen);
    //sfd接受客户端连接，创建新的new_fd，将客户端信息保存在clientaddr中
    if(-1==new_fd){
        perror("accept");
        close(sfd);
        exit(-1);
    }
    printf("%s %d success connect...\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
    return new_fd;
}
//客户端的连接
int tcp_connect(const char *ip,int port){
    int sfd = socket(AF_INET,SOCK_STREAM,0);//客户端注册新的socket
    if(-1==sfd){
        perror("socket");
        exit(-1);
    }
    struct sockaddr_in serveraddr;//连接服务器的sokcet
    memset(&serveraddr,0,sizeof(struct sockaddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(port);
    serveraddr.sin_addr.s_addr = inet_addr(ip);
    
    if(-1==connect(sfd,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr))){
        perror("connect");
        close(sfd);
        exit(-1);
    }
    return sfd;
}
//对int和quit信号屏蔽
void signalhandler(void){
    sigset_t sigSet;
    sigemptyset(&sigSet);
    sigaddset(&sigSet,SIGINT);
    sigaddset(&sigSet,SIGQUIT);
    sigprocmask(SIG_BLOCK,&sigSet,NULL);
}
