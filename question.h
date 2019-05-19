#ifndef QUESTION_H
#define QUESTION_H

#include <QString>
#include <QDataStream>

struct Question
{
   QString question;
   QString answer;
   QString choice1;
   QString choice2;
   QString choice3;
   QString choice4;
   quint16 timeQ;

};

QDataStream& operator<<(QDataStream&,Question const&);
QDataStream& operator>>(QDataStream&,Question&);


#endif // QUESTION_H
