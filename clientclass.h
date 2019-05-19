#ifndef CLIENTCLASS_H
#define CLIENTCLASS_H


class Display;
#include "student.h"
#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QVector>
#include <QDebug>
#include <QString>

class ClientClass: public QObject
{

    Q_OBJECT

    public:
        ClientClass(Display*);
        bool startConnexion(QString,quint16=65432);
        void sendRollNumber(quint32);
        void sendMarks(quint16);
        QString socketError();
    private:
        Student m_student;
        quint16 m_sizePack;
        qint16 m_stepIncommingCnx;
        Display *m_displayClass;
        QString m_error;
        int m_numError;
        bool m_canPassExam;
    public slots:
        void dataReceived();
        //void connectionSuccess();
        //void deconnecte();
};
#include "display.h"
#endif // CLIENTCLASS_H
