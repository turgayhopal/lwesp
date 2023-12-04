// Header: LWESP Queue Source File
// File Name: lwesp_queue.c
// Author: Turgay Hopal
// Date: 29.11.2023

#include "lwesp_queue.h"

void lwesp_queue_init(lwesp_queue_t *queue) {
	queue->front = 0;
  queue->rear = -1;
  queue->size = 0;
}

void lwesp_queue_flush(lwesp_queue_t *queue) {
	memset(queue->data, 0x00, LWESP_QUEUE_SIZE);
	queue->front = 0;
  queue->rear = -1;
  queue->size = 0;
}

uint8_t lwesp_queue_is_full(lwesp_queue_t *queue) {
	return (queue->size == LWESP_QUEUE_SIZE);
}
uint8_t lwesp_queue_is_empty(lwesp_queue_t *queue) {
	return (queue->size == 0);
}

int16_t lwesp_queue_pop(lwesp_queue_t *queue) {
	if (lwesp_queue_is_empty(queue)) {
        // Queue is empty, handle accordingly
        return -1; // Or some other value indicating an error
    }
    uint8_t item = queue->data[queue->front];
    queue->front = (queue->front + 1) % LWESP_QUEUE_SIZE;
    queue->size--;
    return item;
}

void lwesp_queue_push(lwesp_queue_t *queue, uint8_t item) {
	if (lwesp_queue_is_full(queue)) {
			// Queue is full, handle accordingly
			return;
	}
	queue->rear = (queue->rear + 1) % LWESP_QUEUE_SIZE;
	queue->data[queue->rear] = item;
	queue->size++;
}

