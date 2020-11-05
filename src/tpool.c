#include <stdlib.h>

#include "../includes/tpool.h"
#include <stdio.h>

// Созданеи пустого пулла
bool tpool_create(ThreadPool_t **pool) {
    // Выделяем память
    ThreadPool_t* tmp_tpool = malloc(sizeof(ThreadPool_t));
    if (tmp_tpool == NULL) {        
        return false;
    }
    tmp_tpool->count = 0;
    tmp_tpool->first = NULL;
    tmp_tpool->last = NULL;    

    // Инициируем внутренний семафор для потокобезопасного использования
    tmp_tpool->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
     
    // Возвращаем сформированный пулл
    *pool = tmp_tpool;
    
    return true;
}

// Уничтожение пулла и освобождение ресурсов
void tpool_destroy(ThreadPool_t **pool) {
    if (pool != NULL) {
        ThreadPool_t *tmp_tpool = (ThreadPool_t*)*pool;
        if (tmp_tpool != NULL) {
            while (tmp_tpool->count > 0) {
                tpool_pop(pool);
            }
            // Освобождаем семафор
            pthread_mutex_destroy(&(tmp_tpool->mutex));
            free(tmp_tpool);
        }
    }
}

// Актуальный размер пулла
uint32_t tpool_count(ThreadPool_t *pool) {
    return (pool != NULL) ? pool->count : 0;
}

// Добавления элемента в пулл
bool tpool_push(ThreadPool_t **pool, pthread_t tid) {
    // Проверяем что указатель на указатель на pool адекватный
    if (pool == NULL) {
        return false;
    }
    // Ресольвим указатель на пулл
    ThreadPool_t *tmp_pool = *pool;
    // Проверяем что он адекватный
    if (tmp_pool == NULL) {
        return false;
    }
    
    // Создаем новый элемент и убеждаемся, что удалось выделить под него память
    ThreadPoolItem_t *tmp_item = malloc(sizeof(ThreadPoolItem_t));
    if (tmp_item == NULL) {
        return false;
    }
        
    // Заполняем его данными
    tmp_item->tid = tid;
    tmp_item->prev = NULL;
    tmp_item->next = NULL;

    // Выставяелм семафор для атомарности операции
    pthread_mutex_lock(&(tmp_pool->mutex));
    
    // Добавляем его в конец пулла
    if ((tmp_pool->first == NULL) && (tmp_pool->last == NULL)) {
        // Первый элемент в пулле
        tmp_pool->first = tmp_item;
        tmp_pool->last = tmp_item;
    } else {
        // Добавляем в конец пулла
        ThreadPoolItem_t *old_last = tmp_pool->last;
        tmp_item->prev = old_last;
        old_last->next = tmp_item;
        tmp_pool->last = tmp_item;    
    }
    
    // Инкрементируем счетчик
    tmp_pool->count++;
    
    // Снимаем семафор 
    pthread_mutex_unlock(&(tmp_pool->mutex));
    
        
    return true;
}

bool tpool_remove(ThreadPool_t **pool, pthread_t tid) {
    // Проверяем что указатель на указатель на pool адекватный
    if (pool == NULL) {
        return false;
    }
    // Ресольвим указатель на пулл
    ThreadPool_t *tmp_pool = *pool;
    // Проверяем что он адекватный
    if (tmp_pool == NULL) {
        return false;
    }    
    // Проверяем что в пулле есть хотя бы 1 элемент
    if (tmp_pool->count < 1) {
        return false;
    }
    
    // Выставялем семафор для атомарности операции
    pthread_mutex_lock(&(tmp_pool->mutex));

    // Ищем нужный элемент начиная с начала
    ThreadPoolItem_t *item = tmp_pool->first;
    while ((item != NULL) && (item->tid != tid)) {
        item = item->next;
    }
    // Если элемент не найден
    if (item == NULL) {
        return false;
    }    
    // Если элемент найден, то проверяем есть ли элементы до и после него
    if ((item->prev == NULL) && (item->next == NULL)) {
        // Элемент единственный
        tmp_pool->count = 0;
        tmp_pool->first = NULL;
        tmp_pool->last = NULL;
    } else if ((item->prev == NULL) && (item->next != NULL)) {
        // Элемент первый в списке
        tmp_pool->count--;
        ThreadPoolItem_t *old_next = item->next;
        tmp_pool->first = old_next;
        old_next->prev = NULL;
    } else if ((item->prev != NULL) && (item->next == NULL)) {
        // Элемент последний в списке
        tmp_pool->count--;
        ThreadPoolItem_t *old_prev = item->prev;
        tmp_pool->last = old_prev;
        old_prev->next = NULL;
    } else {
        // Элемент в середине списка
        tmp_pool->count--;
        ThreadPoolItem_t *old_next = item->next;
        ThreadPoolItem_t *old_prev = item->prev;
        old_next->prev = old_prev;
        old_prev->next = old_next;
    }

    // Снимаем семафор 
    pthread_mutex_unlock(&(tmp_pool->mutex));
    free(item);
    
    return true;
}

// Получаем первый элемент из пулла
pthread_t tpool_pop(ThreadPool_t **pool) {
    // Проверяем что указатель на указатель на pool адекватный
    if (pool == NULL) {
        return false;
    }
    // Ресольвим указатель на пулл
    ThreadPool_t *tmp_pool = *pool;
    // Проверяем что он адекватный
    if (tmp_pool == NULL) {
        return false;
    }    
    // Проверяем что в пулле есть хотя бы 1 элемент
    if (tmp_pool->count < 1) {
        return false;
    }
    
    // Ищем нужный элемент начиная с начала
    ThreadPoolItem_t *item = tmp_pool->first;
    if (item != NULL) {
        pthread_t tmp_pthread_id = item->tid;
        tpool_remove(&tmp_pool, item->tid);
        return tmp_pthread_id;
    }
    
    return 0;
}
