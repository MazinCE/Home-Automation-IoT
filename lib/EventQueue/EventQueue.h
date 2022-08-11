#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "Event.h"

#define EVENT_QUEUE_SIZE 11

typedef struct
{
    Event queue[EVENT_QUEUE_SIZE];
    int len;
    Event* head;
} EventQueue;

void EventQueue_init(void);
void EventQueue_Push(Event e);
int EventQueue_Poll(Event *e);

#endif