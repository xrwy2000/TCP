#include <tcp_net_socket.h>

int main(int argc,char **argv)
{
    if(argc<3){
        printf("usage:./servertcp ip port\n");
        exit(-1);
    }
    signalhandler();
    int sfd = tcp_init(argv[1],atoi(argv[2]));//char[] 转 int
    while(1){
        int cfd = tcp_accept(sfd);
        char buf[512]={0};
        if(recv(cfd,buf,sizeof(buf),0)==-1){
            perror("recv");
            close(cfd);
            close(sfd);
            exit(-1);
        }
        puts(buf);
        if(-1==send(cfd,"hello world",12,0)){
            perror("send");
            close(cfd);
            close(sfd);
            exit(-1);
        }    
        close(cfd);
    }
    close(sfd);
    return 0;
}

