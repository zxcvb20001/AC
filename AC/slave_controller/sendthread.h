#ifndef SENDTHREAD_H
#define SENDTHREAD_H
#include "room.h"

class sendThread
{
private:
    Room *roomFor;//房间类变量，用于传参
public:
    sendThread();
    void sendMessage(QString s);
    void _SetRoom(Room *roomFor_temp);
};

extern sendThread *SendFromClient;


#endif // SENDTHREAD_H
