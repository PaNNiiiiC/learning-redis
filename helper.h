#pragma once
#include<iostream>
#include <unistd.h>
inline void die(const char* s) {
    std::cerr<<s<<'\n';
    abort();
}

inline int read_all(int fd,char* rbuf,size_t sz) {
    while (sz) {
        ssize_t rv=read(fd,rbuf,sz);
        if (rv<=0) {
            return -1;
        }
        sz-=static_cast<size_t>(rv);
        rbuf+=rv;
    }
    return 0;
}

inline int write_all(int fd, const char* wbuf,size_t sz) {
    while (sz) {
        ssize_t rv=write(fd,wbuf,sz);
        if (rv<=0) {
            return -1;
        }
        sz-=static_cast<size_t>(rv);
        wbuf+=rv;
    }
    return 0;
}