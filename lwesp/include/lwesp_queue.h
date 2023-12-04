// Header: LWESP Queue Header File
// File Name: lwesp_queue.h
// Author: Turgay Hopal
// Date: 28.11.2023


#ifndef INC_LWESP_QUEUE_H_
#define INC_LWESP_QUEUE_H_

#define LWESP_QUEUE_SIZE 1024

#include <stdint.h>
#include <string.h>

typedef struct lwesp_queue_s{
    uint8_t data[LWESP_QUEUE_SIZE];
    uint32_t front;
    uint32_t rear;
    uint32_t size;
} lwesp_queue_t;

void lwesp_queue_init(lwesp_queue_t *queue);
void lwesp_queue_flush(lwesp_queue_t *queue);

uint8_t lwesp_queue_is_full(lwesp_queue_t *queue);
uint8_t lwesp_queue_is_empty(lwesp_queue_t *queue);

int16_t lwesp_queue_pop(lwesp_queue_t *queue);
void lwesp_queue_push(lwesp_queue_t *queue, uint8_t item);


#endif /* INC_LWESP_QUEUE_H_ */