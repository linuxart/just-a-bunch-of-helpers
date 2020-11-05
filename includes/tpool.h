/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tpool.h
 * Author: art
 *
 * Created on 10 июля 2018 г., 21:56
 */

#ifndef TPOOL_H
#define TPOOL_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct ThreadPool_t {
    uint32_t count;
    pthread_mutex_t mutex;
    struct ThreadPoolItem_t *first;
    struct ThreadPoolItem_t *last;    
} ThreadPool_t;
    
typedef struct ThreadPoolItem_t {
    pthread_t tid;
    struct ThreadPoolItem_t *prev;
    struct ThreadPoolItem_t *next;       
} ThreadPoolItem_t;

bool tpool_create(ThreadPool_t **pool);
void tpool_destroy(ThreadPool_t **pool);
uint32_t tpool_count(ThreadPool_t *pool);
bool tpool_push(ThreadPool_t **pool, pthread_t tid);
bool tpool_remove(ThreadPool_t **pool, pthread_t tid);
pthread_t tpool_pop(ThreadPool_t **pool);

// Пулл потоков
ThreadPool_t *tpool;

#ifdef __cplusplus
}
#endif

#endif /* TPOOL_H */

