#include "question.h"

QDataStream& operator<<(QDataStream& dataStream,Question const& quest)
{
    dataStream<<quest.question<<quest.answer<<quest.choice1<<quest.choice2<<quest.choice3<<quest.choice4<<quest.timeQ;
    return dataStream;
}

QDataStream& operator>>(QDataStream &dataStream, Question& quest)
{
    dataStream>>quest.question>>quest.answer>>quest.choice1>>quest.choice2>>quest.choice3>>quest.choice4>>quest.timeQ;
    return dataStream;
}
