#ifndef STUDENT_H
#define STUDENT_H

#include <QTcpSocket>
#include <QString>

struct Student
{
   QTcpSocket* tcpSocket;
   quint32 rollNumber;
   quint16 stepCnx;
   quint16 sizePack;
};
#endif // STUDENT_H
