#include "display.h"

Display::Display(QWidget *parent)
    : QMainWindow(parent), m_serverCnx(this)
{
    srand(time(0));
    m_numQuestion=0;
    m_questionsSucceded=0;
    m_questionsFailed=0;
    m_questionsUnanswered=0;
    m_time=0;
    m_numberOfQuestions=0;
    m_remainQuestion=0;
    m_notSeenQuestion=0;
    m_timer=new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(updateTime()));

    m_centralWidget=new QWidget();
    m_centralWidget->setObjectName("centralWidget");   //  Central Widget
    this->setCentralWidget(m_centralWidget);
    QVBoxLayout *layoutPrincipal=new QVBoxLayout();
    m_centralWidget->setLayout(layoutPrincipal);

    m_ipLabel=new QLabel("Enter the ip adress");
    m_ipLabel->setObjectName("ipLabel");
    m_ipEdit=new QLineEdit();
    m_ipEdit->setObjectName("ipEdit");
    m_ipEdit->setText("localhost");
    m_portLabel=new QLabel("Enter the port");
    m_portLabel->setObjectName("portLabel");
    m_portEdit=new QLineEdit();
    m_portEdit->setText("65432");
    m_portEdit->setObjectName("portEdit");

    QHBoxLayout *portIpLayout=new QHBoxLayout();
    portIpLayout->addWidget(m_ipLabel);
    portIpLayout->addWidget(m_ipEdit);
    portIpLayout->addWidget(m_portLabel);
    portIpLayout->addWidget(m_portEdit);
    layoutPrincipal->addLayout(portIpLayout);

    m_startConnexion=new QPushButton("Start connection");
    m_startConnexion->setObjectName("startConnexion");
    connect(m_startConnexion,SIGNAL(clicked(bool)),this,SLOT(startConnection()));
    layoutPrincipal->addWidget(m_startConnexion);

    m_labelCentral=new QLabel();
    m_labelCentral->setVisible(false);
    m_labelCentral->setObjectName("labelCentral");
    layoutPrincipal->addWidget(m_labelCentral);

    m_rollNumberEdit=new QLineEdit();
    m_rollNumberEdit->setVisible(false);
    m_rollNumberEdit->setObjectName("rollNumberEdit");

    m_sendRollNumber=new QPushButton("Send Roll Number");
    m_sendRollNumber->setVisible(false);
    m_sendRollNumber->setObjectName("sendRollNumber");
    connect(m_sendRollNumber,SIGNAL(clicked(bool)),this,SLOT(sendRollNumber()));

    QHBoxLayout *rollNumbLayout= new QHBoxLayout();
    rollNumbLayout->addWidget(m_rollNumberEdit);
    rollNumbLayout->addWidget(m_sendRollNumber);
    layoutPrincipal->addLayout(rollNumbLayout);


    m_Exam=new QWidget();
    m_Exam->setVisible(false);
    m_Exam->setObjectName("Exam");
    layoutPrincipal->addWidget(m_Exam);

    QVBoxLayout *examLayout=new QVBoxLayout();
    m_Exam->setLayout(examLayout);

    m_question=new QLabel("Question");
    m_question->setObjectName("question");
    examLayout->addWidget(m_question);

    m_answers=new QButtonGroup();
    m_rep1=new QCheckBox();
    m_rep2=new QCheckBox();
    m_rep3=new QCheckBox();
    m_rep4=new QCheckBox();
    m_rep5=new QCheckBox();
    m_answers->addButton(m_rep1,0);
    m_answers->addButton(m_rep2,1);
    m_answers->addButton(m_rep3,2);
    m_answers->addButton(m_rep4,3);
    m_answers->addButton(m_rep5,4);

    m_checksBoxWidget=new QGroupBox();
    m_checksBoxWidget->setObjectName("checksBoxWidget");
    QVBoxLayout *checkBoxLayout=new QVBoxLayout();

    checkBoxLayout->addWidget(m_rep1);
    checkBoxLayout->addWidget(m_rep2);
    checkBoxLayout->addWidget(m_rep3);
    checkBoxLayout->addWidget(m_rep4);
    checkBoxLayout->addWidget(m_rep5);
    m_checksBoxWidget->setLayout(checkBoxLayout);

    examLayout->addWidget(m_checksBoxWidget);
    m_timeLabel=new QLabel();
    m_timeLabel->setObjectName("timeLabel");
    examLayout->addWidget(m_timeLabel);

    m_repondre=new QPushButton("Answer");
    m_repondre->setObjectName("repondre");
    connect(m_repondre,SIGNAL(clicked(bool)),this,SLOT(checkAnswer()));

    m_passQuestion=new QPushButton("Pass And Answer Later");
    m_passQuestion->setObjectName("passQuestion");
    connect(m_passQuestion,SIGNAL(clicked(bool)),this,SLOT(passQuestion()));

    QHBoxLayout *optionLayout=new QHBoxLayout();
    optionLayout->addWidget(m_passQuestion);
    optionLayout->addWidget(m_repondre);
    examLayout->addLayout(optionLayout);


}


Display::~Display()
{

}


void Display::checkAnswer()
{
    if(!m_answers->checkedButton())
        return;

    m_timer->stop();
    --m_remainQuestion;
    --m_notSeenQuestion;
    m_repondre->setEnabled(false);
    m_passQuestion->setEnabled(false);
    if(m_answers->checkedButton()->text()==m_questionnaire[m_numQuestion].answer)
    {
        ++m_questionsSucceded;
    }
    else
    {
        ++m_questionsFailed;
    }
    static int number=0;
    if(m_remainQuestion==0)
    {
        m_serverCnx.sendMarks(m_questionsSucceded);
        this->endOfExam();
    }
    else
    {
        if(m_notSeenQuestion<=0)
        {
            ++number;
            if(number>1)
            {
                m_leftedQuestion.remove(m_numQuestion);
                m_numQuestion=m_leftedQuestion.first();
            }
            else
            {
                m_numQuestion=m_leftedQuestion.first();
            }
        }
        else
        {
            ++m_numQuestion;
        }
        m_answers->setExclusive(false);
        m_answers->checkedButton()->setChecked(false);
        m_answers->setExclusive(true);
        this->examLoop();
    }
}


void Display::endOfExam()
{
    m_Exam->setVisible(false);
    m_labelCentral->setVisible(true);
    m_labelCentral->setText("You finished your exam, here is your perfomance: <BR /> "+
                            QString::number(m_questionsSucceded)+" Succeded <BR />"+
                            QString::number(m_questionsFailed)+" Failed");
}


void Display::enterRollNumber()
{
    m_labelCentral->setText("Enter your rollNumber");
    m_rollNumberEdit->setVisible(true);
    m_sendRollNumber->setVisible(true);
}


void Display::examLoop()
{
    m_repondre->setEnabled(false);
    m_passQuestion->setEnabled(false);
    m_question->setText("Question "+QString::number(m_numQuestion+1)+" / "+
                        QString::number(m_numberOfQuestions)+"<H2>"+m_questionnaire[m_numQuestion].question+"</H2>");
    m_time=m_questionnaire[m_numQuestion].timeQ;
    m_timeLabel->setText(QString::number(m_time)+" Seconds Remains");
    QVector<int> idTab={0,1,2,3,4};
    int sizeTab=5;

    int index=rand() %sizeTab;
    int id=idTab[index];
    auto pointer=m_answers->button(id);
    pointer->setText(m_questionnaire[m_numQuestion].answer);
    idTab.remove(index);
    --sizeTab;

    index=rand()%sizeTab;
    id=idTab[index];
    pointer=m_answers->button(id);
    pointer->setText(m_questionnaire[m_numQuestion].choice1);
    idTab.remove(index);
    --sizeTab;

    index=rand()%sizeTab;
    id=idTab[index];
    pointer=m_answers->button(id);
    pointer->setText(m_questionnaire[m_numQuestion].choice2);
    idTab.remove(index);
    --sizeTab;

    index=rand()%sizeTab;
    id=idTab[index];
    pointer=m_answers->button(id);
    pointer->setText(m_questionnaire[m_numQuestion].choice3);
    idTab.remove(index);
    --sizeTab;

    index=rand()%sizeTab;
    id=idTab[index];
    pointer=m_answers->button(id);
    pointer->setText(m_questionnaire[m_numQuestion].choice4);
    idTab.remove(index);
    --sizeTab;
    m_repondre->setEnabled(true);
    if(m_notSeenQuestion>0)
    {
        m_passQuestion->setEnabled(true);
    }
    m_timer->start(1000);
}


void Display::passQuestion()
{
    m_timer->stop();
    m_repondre->setEnabled(false);
    --m_notSeenQuestion;
    m_passQuestion->setEnabled(false);
    if(!m_leftedQuestion.contains(m_numQuestion))
    {
        m_leftedQuestion[m_numQuestion]=m_numQuestion;
        ++m_numQuestion;
    }
    else
    {

    }
    m_questionnaire[m_numQuestion].timeQ=m_time;
    this->examLoop();
}


void Display::sendRollNumber()
{
    m_rollNumberEdit->setVisible(false);
    m_sendRollNumber->setVisible(false);

    m_serverCnx.sendRollNumber(m_rollNumberEdit->text().toInt());
    m_labelCentral->setText("Please wait the checking of your roll number");
}


void Display::startExam()
{
    m_labelCentral->setVisible(false);
    m_Exam->setVisible(true);
    examLoop();
}


void Display::startConnection()
{
    m_startConnexion->setEnabled(false);
    m_labelCentral->setVisible(true);
    if(m_serverCnx.startConnexion(m_ipEdit->text(),m_portEdit->text().toInt()))
    {
        m_ipLabel->setVisible(false);
        m_ipEdit->setVisible(false);
        m_portLabel->setVisible(false);
        m_portEdit->setVisible(false);
        m_startConnexion->setVisible(false);
        m_labelCentral->setText("<H3> Connexion established, wait a moment for entering your roll number</H3>");

    }
    else
    {
        m_labelCentral->setText(m_serverCnx.socketError());
        m_startConnexion->setText("Try again");
        m_startConnexion->setEnabled(true);
    }
}


void Display::questionnaireReceived(bool canPassEXam,QVector<Question> questions)
{
    if(canPassEXam)
    {
        m_labelCentral->setText("Roll number accepted<BR /> your are allowed to pass exam <BR />you have already receive your exam."
                                "<BR />Waiting for the signal to start the exam");
        m_questionnaire=questions;
        m_numberOfQuestions=questions.size();
        m_remainQuestion=m_numberOfQuestions;
        m_notSeenQuestion=m_numberOfQuestions;

    }
    else
    {
        m_labelCentral->setText("You are not alowed to pass the exam");
    }
}


void Display::updateTime()
{
    --m_time;
    if(m_time>0)
    {
        m_timeLabel->setText(QString::number(m_time)+" Seconds Remain");
    }
    else
    {
        if(m_answers->checkedButton())
        {
            checkAnswer();
        }
        else
        {
            m_answers->button(0)->setChecked(true);
            checkAnswer();
        }
    }
}
