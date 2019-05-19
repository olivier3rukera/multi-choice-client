#ifndef DISPLAY_H
#define DISPLAY_H


#include "clientclass.h"
#include "question.h"
#include <QByteArray>
#include <QButtonGroup>
#include <QCheckBox>
#include <QDataStream>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>
#include <QDebug>
#include <cstdlib>
#include <ctime>
#include <QAbstractButton>
#include <QList>
#include <QTimer>
#include <QMap>

class Display: public QMainWindow
{
    Q_OBJECT

    public:
        Display(QWidget *parent=0);
        void enterRollNumber();
        void questionnaireReceived(bool,QVector<Question>);
        void startExam();
        void endOfExam();
        ~Display();
    private:
        QPushButton *m_startConnexion;
        QPushButton *m_sendRollNumber;
        QLabel *m_labelCentral;
        QLabel *m_portLabel, *m_ipLabel;
        QLineEdit *m_ipEdit, *m_portEdit, *m_rollNumberEdit;
        QWidget *m_centralWidget;
        ClientClass m_serverCnx;

        QWidget *m_Exam;
        QLabel *m_question;
        QButtonGroup *m_answers;
        QCheckBox *m_rep1, *m_rep2, *m_rep3, *m_rep4, *m_rep5;
        QGroupBox *m_checksBoxWidget;
        QPushButton *m_passQuestion, *m_repondre;
        QLabel *m_timeLabel;
        QTimer *m_timer;
        QVector<Question> m_questionnaire;
        int m_numQuestion;
        int m_questionsSucceded;
        int m_questionsFailed;
        int m_questionsUnanswered;
        int m_time;
        int m_numberOfQuestions;
        int m_remainQuestion;
        int m_notSeenQuestion;
        QMap<int,int> m_leftedQuestion;


    public slots:
        void startConnection();
        void sendRollNumber();
        void examLoop();
        void checkAnswer();
        void updateTime();
        void passQuestion();

};

#endif // DISPLAY_H
