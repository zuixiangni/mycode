#ifndef _MEM_POOL_H_
#define _MEM_POOL_H_

#include <stdlib.h>
#include <pthread.h>

class MemPool
{
public:
    MemPool(int buff_size, int chuck_size):buff_(NULL), buff_size_(buff_size), chuck_size_(chuck_size), free_(NULL){
        pthread_mutex_init(&buff_lock_, NULL);
    }

    ~MemPool(){
        if (buff_)
            free(buff_);
        pthread_mutex_destroy(&buff_lock_);
    }

    char *malloc_mem();
    void free_mem(char *buff);
    void Init_mem(); 
    
private:
    MemPool(MemPool &pool);
    MemPool &operator=(MemPool &pool);

private:
    char    *buff_;
    long    buff_size_;
    long    chuck_size_;
    char    *free_;
    pthread_mutex_t buff_lock_;
};

#endif 
