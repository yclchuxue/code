#ifndef    __MY_RECV_H
#define    __MY_RECV_H
    #define   BUFSIZE 1024
    void my_err(const char *err_string,int line);
    int my_recv(int conn_fd, char *date_buf, int line);
#endif