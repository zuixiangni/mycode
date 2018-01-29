#include "mempool.h"
#include <iostream>
#include <errno.h>

char *MemPool::malloc_mem()
{
    char *buff = NULL;
    long free = 0;
    pthread_mutex_lock(&buff_lock_);
    
    if (!free_)
        goto FIN;

    buff = free_;
    free = *reinterpret_cast<long *>(free_);
    free_ = reinterpret_cast<char *>(free); 

FIN:
    pthread_mutex_unlock(&buff_lock_);
    return buff;
}

void MemPool::free_mem(char *buff)
{
    pthread_mutex_lock(&buff_lock_);
    if (!buff)
        goto FIN; 

    *reinterpret_cast<long *>(buff) = reinterpret_cast<long>(free_);
    free_ = buff; 

FIN:
    pthread_mutex_unlock(&buff_lock_);
    return;
}

void MemPool::Init_mem()
{
    char *tmp = NULL;
    int size = 0;

    pthread_mutex_lock(&buff_lock_);
    if (buff_)
        goto FIN;

    buff_ = (char *)valloc(buff_size_);
    if (!buff_){
        std::cout << "malloc buff [" << buff_size_ << "] failed, errno: " << errno << std::endl; 
        goto FIN;
    }

    size = buff_size_/chuck_size_;
    if (size <=0)
        goto FIN;
        
    free_ = buff_;
    tmp = buff_; 
    for (int i=0; i<size-1; i++)
    {
        *reinterpret_cast<long *>(tmp) = reinterpret_cast<long>(tmp + chuck_size_);
        tmp += chuck_size_;
    }
    
    *reinterpret_cast<long *>(tmp) = 0;

FIN:
    pthread_mutex_unlock(&buff_lock_);
    return;
}
