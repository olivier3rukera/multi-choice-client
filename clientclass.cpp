#include "clientclass.h"

ClientClass::ClientClass(Display* displayClass)
{
    m_displayClass=displayClass;
    m_student.tcpSocket=new QTcpSocket(this);
    connect(m_student.tcpSocket,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    connect(m_student.tcpSocket,SIGNAL(connected()),this,SLOT(connectionSuccess()));
    connect(m_student.tcpSocket,SIGNAL(disconnected()),this,SLOT(deconnecte()));
    m_stepIncommingCnx=0;
    m_sizePack=0;
    m_numError=100;
    m_canPassExam=false;

}


void ClientClass::dataReceived()
{
    QDataStream in(m_student.tcpSocket);
    qDebug()<<"DataReceived";

    if(m_sizePack==0||m_stepIncommingCnx==0)
    {qDebug()<<m_sizePack<<m_stepIncommingCnx;
        if(m_student.tcpSocket->bytesAvailable()<(int)2*sizeof(quint16))
        {
            return;
        }
        in>>m_sizePack;
        in>>m_stepIncommingCnx;
        qDebug()<<m_sizePack<<m_stepIncommingCnx;
    }  else{}

    if(m_stepIncommingCnx==1) // signal to show rollnumber
    {
        m_displayClass->enterRollNumber();
    }

    else if(m_stepIncommingCnx==2)// We receive questions
    {
        if(m_student.tcpSocket->bytesAvailable()<m_sizePack)
        {
            return;
        } else{}

        in>>m_canPassExam;
        QVector<Question> questions;
        if(m_canPassExam)
        {
            in>>questions;
        }
        else{}
        m_displayClass->questionnaireReceived(m_canPassExam,questions);
    }
    else if(m_stepIncommingCnx==3 && m_canPassExam==true) //signal to start the exam
    {
        m_displayClass->startExam();
    }
    else{}

    m_sizePack=0;
    m_stepIncommingCnx=0;
}


void ClientClass::sendMarks(quint16 marks)
{
    QByteArray packet;
    QDataStream out(&packet,QIODevice::WriteOnly);
    out<<(quint16)0;
    out<<(quint16)2;
    out<<marks;
    out.device()->seek(0);
    out<<(quint16)(packet.size()-sizeof(quint16));
    m_student.tcpSocket->write(packet);
}


void ClientClass::sendRollNumber(quint32 rollNumber)
{
    QByteArray packet;
    QDataStream out(&packet,QIODevice::WriteOnly);
    out<<(quint16)0;
    out<<(quint16)1;
    out<<rollNumber;
    out.device()->seek(0);
    out<<(quint16)(packet.size()-sizeof(quint16));
    m_student.tcpSocket->write(packet);
}


bool ClientClass::startConnexion(QString ip, quint16 port)
{
    m_student.tcpSocket->abort();
    m_student.tcpSocket->connectToHost(ip,port);
    m_numError=m_student.tcpSocket->error();
    return m_student.tcpSocket->waitForConnected(60000);
}


QString ClientClass::socketError()
{
    switch (m_numError)
    {
        case 0:
            m_error="The Connection Was refused by the server";
        case 2:
            m_error="Host Not Found";
            break;
        default:
            m_error="The id of error is :"+QString::number(m_numError)+"<BR /> Try again";
            break;
    }
    return m_error;
}
