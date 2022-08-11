#include "EventQueue.h"

EventQueue eventQueue;

void EventQueue_init(void)
{
    eventQueue.len = 0;
    Event e;
    e.type = EVNT_NULL;
    eventQueue.queue[0] = e;
    eventQueue.head = &eventQueue.queue[1];
}

void EventQueue_Push(Event e)
{
    if (eventQueue.len < EVENT_QUEUE_SIZE - 1)
    {
        eventQueue.queue[++eventQueue.len] = e;
    }
}

int EventQueue_Poll(Event *e)
{
    if (eventQueue.len == 0)
    {
        eventQueue.head = &eventQueue.queue[1];
        return 0;
    }

    *e = *eventQueue.head;

    eventQueue.head++;
    eventQueue.len--;

    return 1;
}