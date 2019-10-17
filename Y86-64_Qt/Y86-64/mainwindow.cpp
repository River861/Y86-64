#include "mainwindow.h"

#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <Qdebug>
#include <QTextBrowser>
#include <QTextEdit>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QProcess>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFont>
#include <QProgressBar>
#include <QTime>
#include <QDate>
#include <QTimer>
#include <QApplication>
#include <QDebug>
#include <QSpinBox>
#include <QSlider>
#include <QPixmap>
#include <QPalette>
#include <QDateTime>

 QString copyPath,Path;
 bool cache_state=true;
 char  _string[200],_string2[1000];
 int count=0;
 int max_memory=1000;
 int run_speed=5;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("River's Y86-64");
    file = menuBar()->addMenu(tr("&File"));
    QMenu *run = menuBar()->addMenu(tr("&Run"));
    QMenu *help=menuBar()->addMenu(tr("&Help"));
    QMenu *Log_in=menuBar()->addMenu(tr("&Log_in"));
    QMenu *Setting=menuBar()->addMenu(tr("&Setting"));
    toolBar = addToolBar(tr("&File"));

    statusBar();
    textBrowser = new QTextBrowser(this);
    rubbish=new QTextBrowser;
    textEdit =new QTextEdit(this);

    textBrowser->setObjectName("myframe");
    textBrowser->resize(400,700);
    textBrowser->setStyleSheet("QFrame#myframe{border-image:url(:image/sky3)}  color: rgb(65,48,216)" );
    QPalette *pal=new QPalette;
    pal->setColor(QPalette::Text,QColor(30,21,111));
    textBrowser->setPalette(*pal);

    textEdit->setObjectName("myframe2");
    textEdit->resize(400,700);
    textEdit->setStyleSheet("QFrame#myframe2{border-image:url(:image/sea3)}" );
    textEdit->setPalette(*pal);

    cmd = new QProcess(this);    cmd->start("cmd");   cmd->waitForStarted();
    connect(cmd , SIGNAL(readyReadStandardError()) ,this, SLOT(readerror()));
//-------------------------------------------------------------------------------------------------------
    QFont font("",10, 87);
    timelabel=new QLabel;
    timelabel->setStyleSheet("color: rgb(138,211,255)");
    timelabel->setFont(font);
    datelabel=new QLabel;
    datelabel->setStyleSheet("color: rgb(138,211,255)");
    datelabel->setFont(font);
    statusBar()->setStyleSheet("color:rgb(138,211,255)");
    submit=new QPushButton;
    submit->setMinimumSize(50,25);
//---------------------------------------------------------------------------------
    cachelabel=new QLabel;
    cachelabel->setStyleSheet("color: rgb(138,211,255)");
    cachelabel->setFont(font);
    cachelabel->setText("Use Cache");
    cache = new QSlider(Qt::Horizontal);
    cache->setRange(0,1);
    cache->setValue(1);
    cache->setStyleSheet("QSlider:groove:horizontal {border: 1px solid #00688B;  height:5px;  border-radius: 3px;}  QSlider:handle:horizontal{background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,stop:0.6 #45ADED, stop:0.778409 rgba(255, 255, 255, 255));  width:10px;  margin-top:-3px;  margin-bottom:-3px;border-radius:5px;}  QSlider:handle:horizontal:hover {background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0.6 #2A8BDA,stop:0.778409 rgba(255, 255, 255, 255));  width:10px;  margin-top: -3px;  margin-bottom: -3px;  border-radius:5px;}  QSlider:sub-page{border-radius:3px;  background:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #009ACD, stop:1 #008B8B);   margin: 2px 0;}  ");
    cache->setMaximumSize(35,15);
    QObject::connect(cache,&QSlider::valueChanged,this,&MainWindow::set_cache) ;
  //-------------------------------------------------------------------------------------
    progress=new QProgressBar;
    progress->setOrientation(Qt::Horizontal);
    progress->setValue(0);  
    progress->setStyleSheet("QProgressBar{border:2px solid grey; border-radius:4px;text-align:center;}QProgressBar::chunk{background-color:skyblue;width:8px;margin:0.5px;}");

    cachebt=new QPushButton;
    cachebt->setMinimumSize(50,25);
    cachebt->setText(tr("cache"));
    cachebt->setStatusTip(tr("Check the cache state of the case last run."));
    connect(cachebt,&QPushButton::clicked,this,&MainWindow::opencache);

    final=new QPushButton;
    final->setMinimumSize(50,25);
    final->setText(tr("result"));
    final->setStatusTip(tr("Check the final result of the case last run."));
    connect(final,&QPushButton::clicked,this,&MainWindow::openfinal);
    clean=new QPushButton;
    clean->setMinimumSize(50,25);
    clean->setText(tr("clear"));
    clean->setStatusTip(tr("clear the table."));
    connect(clean,&QPushButton::clicked,this,&MainWindow::cleanall);

    QHBoxLayout* hl=new QHBoxLayout;
    hl->addWidget(progress);
    hl->addWidget(cachebt);
    hl->addWidget(final);
    hl->addWidget(clean);

    w=new QWidget;
    w->setLayout(hl);

    QHBoxLayout* hl2=new QHBoxLayout;
    hl2->addWidget(datelabel);
    hl2->addWidget(timelabel);
    hl2->addStretch();
    hl2->addWidget(cache);
    hl2->addWidget(cachelabel);
    hl2->addStretch();
    hl2->addWidget(submit);

   w2=new QWidget;
   w2->setLayout(hl2);
//---------------------------------------------------------------------------------
    widget = new QWidget();
    vlayout=new QVBoxLayout;
    vlayout->addWidget(textBrowser);
    vlayout->addWidget(w);
    vlayout->addWidget(textEdit);
    vlayout->addWidget(w2);
    widget->setLayout(vlayout);
    setCentralWidget(widget);
//---------------------------------------------------------------------------------
    openAction=new QAction(QIcon(":/image/openfile"),tr("&Open a file"),this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open a test or result file."));
    saveAction = new QAction(QIcon(":/image/save"), tr("&Save the file"), this);
    saveAction->setShortcuts(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save the file you changed."));
    connect(openAction,&QAction::triggered,this,&MainWindow::openFile);
    connect(saveAction,&QAction::triggered,this,&MainWindow::saveFile);
    file->addAction(openAction);
    file->addAction(saveAction);
    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);
//-----------------------------------------------------------------------------------
    runall=new QAction(QIcon(":/image/run"),tr("&Run all"),this);
    runall->setShortcuts(QKeySequence::New);
    runall->setStatusTip(tr("Run all the test cases."));
    runone = new QAction(QIcon(":/image/runone"), tr("Run one test..."), this);
    runone->setStatusTip(tr("Choose a case to run."));
    connect(runall,&QAction::triggered,this,&MainWindow::test_begin);
    connect(runone,&QAction::triggered,this,&MainWindow::choose_one);
    run->addAction(runall);
    run->addAction(runone);
    toolBar->addAction(runall);
    toolBar->addAction(runone);
//-------------------------------------------------------------------------------------------
       submit->setText(tr("submit"));
       submit->setStatusTip(tr("send an instant instructions with break points to Y86-64."));
       connect(submit,&QPushButton::clicked,this,&MainWindow::runacode);

       submitAction=new QAction(QIcon(":/image/submit"),tr("Submit a code..."),this);
       submitAction->setStatusTip(tr("Submit the code you write on the texteditor."));
       connect(submitAction,&QAction::triggered,this,&MainWindow::runacode);
       run->addAction(submitAction);
       toolBar->addAction(submitAction);
//-------------------------------------------------------------------------------------
    readme=new QAction(QIcon(":/image/help"),tr("&Ask for help"),this);
    readme->setShortcuts(QKeySequence::HelpContents);
    readme->setStatusTip(tr("Learn more imformations about River's Y86-64."));
    Qtcode=new QAction(QIcon(":/image/qt"),tr("Get a Qr_code..."),this);
    Qtcode->setStatusTip(tr("Get quick help with your mobile phone."));
    author=new QAction(QIcon(":/image/author"),tr("&About the author..."),this);
    author->setStatusTip(tr("get to know River."));
    connect(readme,&QAction::triggered,this,&MainWindow::Readme);
    connect(author,&QAction::triggered,this,&MainWindow::Author);
    connect(Qtcode,&QAction::triggered,this,&MainWindow::get_qtcode);
    help->addAction(readme);
    help->addAction(Qtcode);
    help->addAction(author);
    toolBar->addAction(readme);
    toolBar->addAction(Qtcode);
//--------------------------------------------------------------------------------------
   login=new QAction(QIcon(":/image/key"),tr("&Log in"),this);
   login->setShortcuts(QKeySequence::AddTab);
   login->setStatusTip(tr("Log in to get more authority."));
   connect(login,&QAction::triggered,this,&MainWindow::Login);
   Log_in->addAction(login);
   toolBar->addAction(login);

   logout=new QAction(QIcon(":/image/logout"),tr("&Log out"),this);
   logout->setStatusTip(tr("Log out."));
   IDcard=new QLabel;
   connect(logout,&QAction::triggered,this,&MainWindow::Logout);
   Log_in->addAction(logout);
   toolBar->addAction(logout);
//------------------------------------------------------------------------------------------
   QTimer *timer=new QTimer;
   timer->start(500);
   connect(timer,SIGNAL(timeout()),this,SLOT(sl_time()));
//-------------------------------------------------------------------------------------------
   setting=new QAction(QIcon(":/image/cog"),tr("&Setting..."),this);
   setting->setShortcuts(QKeySequence::Replace);
   setting->setStatusTip(tr("Set value of the Memory Size and the Running Speed."));
   connect(setting,&QAction::triggered,this,&MainWindow::_setting);
   Setting->addAction(setting);
   toolBar->addAction(setting);
//------------------------------------------------------------------------------------------

}

void MainWindow::get_qtcode()
{
    QDialog *dialog = new QDialog(this);
    dialog->setMinimumSize(200,200);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowTitle(tr("Qr Code"));
    dialog->setStatusTip(tr("Scan the qr_code to get a quick help."));

    QPixmap pixmap(":/image/Qt");
    QPixmap pixmap2 = pixmap.scaled(200, 200);
    QLabel *ww=new QLabel;
    ww->setPixmap(pixmap2);

    QHBoxLayout *ll=new QHBoxLayout;
    ll->addWidget(ww);
    dialog->setLayout(ll);

    dialog->show();
}

void MainWindow::Logout()
{
    IDcard->setText(" ");
}

void MainWindow::_setting()
{
    QDialog *dialog = new QDialog(this);
    dialog->setMinimumSize(400,150);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowTitle(tr("Setting"));
    dialog->setStatusTip(tr("Set value of the Memory Size and the Running Speed."));

    spinBox_m = new QSpinBox;
    spinBox_s = new QSpinBox;
    slider_m = new QSlider(Qt::Horizontal);
    slider_s = new QSlider(Qt::Horizontal);
    slider_m->setStyleSheet("QSlider:groove:horizontal {border: 1px solid #00688B;  height:5px;  border-radius: 3px;}  QSlider:handle:horizontal{background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,stop:0.6 #45ADED, stop:0.778409 rgba(255, 255, 255, 255));  width:10px;  margin-top:-3px;  margin-bottom:-3px;border-radius:5px;}  QSlider:handle:horizontal:hover {background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0.6 #2A8BDA,stop:0.778409 rgba(255, 255, 255, 255));  width:10px;  margin-top: -3px;  margin-bottom: -3px;  border-radius:5px;}  QSlider:sub-page{border-radius:3px;  background:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #009ACD, stop:1 #008B8B);   margin: 2px 0;}  ");
    slider_s->setStyleSheet("QSlider:groove:horizontal {border: 1px solid #00688B;  height:5px;  border-radius: 3px;}  QSlider:handle:horizontal{background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,stop:0.6 #45ADED, stop:0.778409 rgba(255, 255, 255, 255));  width:10px;  margin-top:-3px;  margin-bottom:-3px;border-radius:5px;}  QSlider:handle:horizontal:hover {background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0.6 #2A8BDA,stop:0.778409 rgba(255, 255, 255, 255));  width:10px;  margin-top: -3px;  margin-bottom: -3px;  border-radius:5px;}  QSlider:sub-page{border-radius:3px;  background:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #009ACD, stop:1 #008B8B);   margin: 2px 0;}  ");
    spinBox_m->setRange(1, 1000);
    spinBox_s->setRange(1, 5);
    spinBox_m->setValue(max_memory);
    spinBox_s->setValue(run_speed);
    slider_s->setRange(1, 5);
    slider_s->setValue(run_speed);
    slider_m->setRange(1, 1000);
    slider_m->setValue(max_memory);
    QObject::connect(slider_m, &QSlider::valueChanged, spinBox_m, &QSpinBox::setValue);
    void (QSpinBox:: *spinBoxSignal)(int) = &QSpinBox::valueChanged;
    QObject::connect(spinBox_m, spinBoxSignal, slider_m, &QSlider::setValue);
    QObject::connect(slider_s, &QSlider::valueChanged, spinBox_s, &QSpinBox::setValue);
    QObject::connect(spinBox_s, spinBoxSignal, slider_s, &QSlider::setValue);

    QLabel *memory=new QLabel;
    memory->setText(tr("Memory Size (MB): "));
    QHBoxLayout *layout_M = new QHBoxLayout;
    layout_M->addWidget(memory);
    layout_M->addWidget(spinBox_m);
    layout_M->addWidget(slider_m);
    QWidget *wm=new QWidget;
    wm->setLayout(layout_M);

    QLabel *speed=new QLabel;
    speed->setText(tr("Running Speed level: "));
    QHBoxLayout *layout_S = new QHBoxLayout;
    layout_S->addWidget(speed);
    layout_S->addWidget(spinBox_s);
    layout_S->addWidget(slider_s);
    QWidget *ws=new QWidget;
    ws->setLayout(layout_S);

    QPushButton *b=new QPushButton;
    b->setMinimumSize(50,25);
    b->setText(tr("OK"));
    connect(b,&QPushButton::clicked,this,&MainWindow::got_val);
    connect(b,&QPushButton::clicked,dialog,&QDialog::close);

    QHBoxLayout *lb=new QHBoxLayout;
    lb->addStretch();
    lb->addWidget(b);
    lb->addStretch();
    QWidget *x=new QWidget;
    x->setLayout(lb);

    QVBoxLayout *lms = new QVBoxLayout;
    lms->addWidget(wm);
    lms->addWidget(ws);
    lms->addWidget(x);
    dialog->setLayout(lms);

    dialog->show();
}

void MainWindow::got_val()
{ 
   max_memory=spinBox_m->value();
   run_speed=spinBox_s->value();
}

void MainWindow::runacode()
{  
    connect(cmd , SIGNAL(readyReadStandardOutput()) , this,SLOT(printaprocess()));

    get_string2(QDir::currentPath());
    cmd->write("cd ");
    cmd->write(_string2);cmd->write("\n");
    cmd->write("cd ../../../Y86-64_VS/River's_Y86-64/Release\n");
    cmd->write(" .\\River's_Y86-64.exe\n");

    get_string2(QString::number(max_memory));
    cmd->write(_string2);cmd->write("\n");

    get_string2(QString::number(run_speed));
    cmd->write(_string2);cmd->write("\n");

    if(cache_state==false) cmd->write("no_cache.\n");
    else cmd->write("use.\n");

    cmd->write("order.\n");

    QString code=textEdit->document()->toPlainText();
    cmd->write(code.toLocal8Bit() + '\n');
    cmd->write("EOF\n");
    progress->setRange(0,999);
    int val=progress->value();
    for(int i=1;i<=1000;i++)
    {
        double dProgress = (progress->value() - progress->minimum()) * 100.0
                        / (progress->maximum() - progress->minimum());
        progress->setFormat(tr("Current progress : %1%").arg(QString::number(dProgress, 'f', 1)));
        progress->setValue(val+i);
    }
    progress->setValue(0);
}

void MainWindow::printaprocess()
{
    count++;
    if(count<7)
    {
        rubbish->append(cmd->readAllStandardOutput().data());
        return;
    }
    QString str=QString::fromLocal8Bit(cmd->readAllStandardOutput());
    textBrowser->append(str);
    return;
}

void MainWindow::sl_time()
{
    QTime time=QTime::currentTime();
    QString txtTime=time.toString("hh:mm:ss");

    QDate date=QDate::currentDate();
    QString txtDate=date.toString("yy-MM-dd");
    txtDate ="20"+txtDate;
    timelabel->setText(txtTime);
    datelabel->setText(txtDate);
}

void MainWindow::Login()
{
    logindialog = new QDialog(this);
    logindialog->setMinimumSize(200,200);
    logindialog->setAttribute(Qt::WA_DeleteOnClose);
    logindialog->setWindowTitle(tr("Log in"));
    logindialog->setStatusTip(tr("Log in to get more power."));

    QLabel *input_id = new QLabel(QWidget::tr("Your ID :"));
    id = new QLineEdit();
    id->setPlaceholderText(tr("Please input your ID"));
    QHBoxLayout *h1 = new QHBoxLayout;
    h1->addWidget(input_id);
    h1->addWidget(id);
    QWidget *w1=new QWidget;
    w1->setLayout(h1);

    QLabel *input_password = new QLabel(QWidget::tr("Password :"));
    password = new QLineEdit();
    password->setEchoMode(QLineEdit::Password);
    password->setPlaceholderText(tr("The password."));
    QHBoxLayout *h2 = new QHBoxLayout;
    h2->addWidget(input_password);
    h2->addWidget(password);
    QWidget *w2=new QWidget;
    w2->setLayout(h2);

    QPushButton *b=new QPushButton;
    b->setMinimumSize(50,25);
    b->setText(tr("Log in"));
    connect(b,&QPushButton::clicked,this,&MainWindow::judgeID);

    QHBoxLayout *lb=new QHBoxLayout;
    lb->addStretch();
    lb->addWidget(b);
    lb->addStretch();
    QWidget *x=new QWidget;
    x->setLayout(lb);

    QVBoxLayout *v=new QVBoxLayout;
    v->addWidget(w1);
    v->addWidget(w2);
    v->addWidget(x);

    logindialog->setLayout(v);
    logindialog->show();
}

void MainWindow::judgeID()
{
    if(id->text() == "River" &&
           password->text() == "17307130162")
    {
       success();
    }
    else {
       QMessageBox::warning(this, tr("Warning !!"),
                   tr("Wrong ID or password."),
                   QMessageBox::Yes);
    }
}

void MainWindow::success()
{
      IDcard->setText(tr("Welcome back, River."));
      toolBar->addWidget(IDcard);
      logindialog->close();
}

void MainWindow::Author()
{
    QDialog *dialog = new QDialog(this);
    dialog->setMinimumSize(300,300);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowTitle(tr("Nice to meet you~~~"));
    dialog->setStatusTip(tr("River is happy to see you."));

    QLabel *name=new QLabel;
    name->setText(tr("Name:  River"));
    QLabel *note= new QLabel;
    note->setText(tr("ID:  17307130162"));
    QLabel *picture=new QLabel;
    picture->setBaseSize(30,30);
    picture->setPixmap(QPixmap(":/image/me"));
    QLabel *words=new QLabel;
    words->setText(tr("Enjoy yourself~~ >_<"));


    QFont font2("",10,75);
    name->setFont(font2); note->setFont(font2);  words->setFont(font2);

    QVBoxLayout *vlay=new QVBoxLayout;
    vlay->addWidget(name);
    vlay->addWidget(note);
    vlay->addWidget(words);
    QHBoxLayout *hlay=new QHBoxLayout;
    QWidget *wid=new QWidget;
    wid->setLayout(vlay);
    hlay->addWidget(picture);
    hlay->addWidget(wid);

    dialog->setLayout(hlay);
    dialog->show();
}

void MainWindow::Readme()
{
    QFile file("../../../README.txt");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Read File"),
                             tr("Cannot open file:\n%1").arg(Path));
        return;
    }
    QTextStream in(&file);
    textBrowser->setText(in.readAll());
    file.close();
}

void MainWindow::test_begin()
{
    count=0;
    connect(cmd , SIGNAL(readyReadStandardOutput()) , this,SLOT(runalloutput()));

    get_string2(QDir::currentPath());
    cmd->write("cd ");
    cmd->write(_string2);cmd->write("\n");
    cmd->write("cd ../../../Y86-64_VS/River's_Y86-64/Release\n");
    cmd->write(" .\\River's_Y86-64.exe\n");

    get_string2(QString::number(max_memory));
    cmd->write(_string2);cmd->write("\n");

    get_string2(QString::number(run_speed));
    cmd->write(_string2);cmd->write("\n");

    if(cache_state==false) cmd->write("no_cache.\n");
    else cmd->write("use.\n");

    cmd->write("test_begin.\n");
}

void MainWindow::runalloutput()
{
    count++;
    progress->setRange(0,13488);
    int val=progress->value();
    for(int i=1;i<=500;i++)
    {
        double dProgress = (progress->value() - progress->minimum()) * 100.0
                        / (progress->maximum() - progress->minimum());
        progress->setFormat(tr("Current progress : %1%").arg(QString::number(dProgress, 'f', 1)));
        progress->setValue(val+i);
    }

    if(count<4)
    {
         rubbish->append(cmd->readAllStandardOutput().data());
         return;
    }
    else if(count==27)
    {
        rubbish->append(cmd->readAllStandardOutput().data());
        count=0;
        disconnect(cmd , SIGNAL(readyReadStandardOutput()) , this,SLOT(runalloutput()));
        progress->setValue(0);
        choose();
        return;
    }
     textBrowser->append(QString::fromLocal8Bit(cmd->readAllStandardOutput()));
}

void MainWindow::readerror()
{
    QMessageBox::information(0, tr("Error"), cmd->readAllStandardError().data());
}

void MainWindow::choose()
{
    QDialog *dialog = new QDialog(this);
    dialog->setMinimumSize(250,100);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowTitle(tr("choose a file"));
    dialog->setStatusTip(tr("Choose a file to check the detailed process."));
//----------------------------------------------------------------------------------
    QGridLayout *glayout = new QGridLayout;
    glayout->setColumnStretch(0, 1);
    glayout->setColumnStretch(1, 3);
    glayout->setMargin(10);

    QLabel *choose_result = new QLabel(QWidget::tr("Check the process of:"));
    QComboBox *result_file = new QComboBox();
    result_file->addItem(QWidget::tr("select one"));
    result_file->addItem(QWidget::tr("prog1.yo.txt"));result_file->addItem(QWidget::tr("prog2.yo.txt"));result_file->addItem(QWidget::tr("prog3.yo.txt"));
    result_file->addItem(QWidget::tr("prog4.yo.txt"));result_file->addItem(QWidget::tr("prog5.yo.txt"));result_file->addItem(QWidget::tr("prog6.yo.txt"));
    result_file->addItem(QWidget::tr("prog7.yo.txt"));result_file->addItem(QWidget::tr("prog8.yo.txt"));result_file->addItem(QWidget::tr("prog9.yo.txt"));
    result_file->addItem(QWidget::tr("prog10.yo.txt"));result_file->addItem(QWidget::tr("abs-asum-cmov.yo.txt"));result_file->addItem(QWidget::tr("abs-asum-jmp.yo.txt"));
    result_file->addItem(QWidget::tr("asum.yo.txt"));result_file->addItem(QWidget::tr("asumi.yo.txt"));result_file->addItem(QWidget::tr("asumr.yo.txt"));
    result_file->addItem(QWidget::tr("cjr.yo.txt"));result_file->addItem(QWidget::tr("j-cc.yo.txt"));result_file->addItem(QWidget::tr("poptest.yo.txt"));
    result_file->addItem(QWidget::tr("pushquestion.yo.txt"));result_file->addItem(QWidget::tr("pushtest.yo.txt"));result_file->addItem(QWidget::tr("ret-hazard.yo.txt"));
    result_file->addItem(QWidget::tr("other..."));
    result_file->insertSeparator(22);

    glayout->addWidget(choose_result, 0, 0);
    glayout->addWidget(result_file, 0, 1);

    QHBoxLayout *bomLayout = new QHBoxLayout;
    QPushButton *select_btn = new QPushButton(QWidget::tr("Select"));
    select_btn->setMinimumSize(50,25);
    bomLayout->addStretch();
    bomLayout->addWidget(select_btn);
    bomLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(glayout);
    mainLayout->addLayout(bomLayout);

    dialog->setLayout(mainLayout);

    connect(result_file, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(get_path(const QString &)));
    connect(select_btn, SIGNAL(clicked()), this, SLOT(openone()));
//-----------------------------------------------------------------------------------
    dialog->show();
}

void MainWindow::get_path(const QString &str)
{
    if(str=="select one") return;
    else if(str=="other...")
    {
        Path=str; return;
    }
    copyPath=Path="../../../Y86-64_VS/file/test process/"+str;
}

void MainWindow::openone()
{
    if(Path=="other...")
    {
        openFile(); Path.clear(); return;
    }
    if(!Path.isEmpty()) {
        QFile file(Path);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, tr("Read File"),
                                 tr("Cannot open file:\n%1").arg(Path));
            return;
        }
        progress->setMaximum(799);
        for(int i=700;i<=800;i++)
        {
            double dProgress = (progress->value() - progress->minimum()) * 100.0
                            / (progress->maximum() - progress->minimum());
            progress->setFormat(tr("Current progress : %1%").arg(QString::number(dProgress, 'f', 1)));
            progress->setValue(i);
        }
        QTextStream in(&file);
        textBrowser->setText(in.readAll());
        progress->setValue(0);
        file.close();
    } else {
        QMessageBox::warning(this, tr("Path"),
                             tr("You did not select any file."));
    }
    Path.clear();
}

void MainWindow::choose_one()
{
    QDialog *dialog = new QDialog(this);
    dialog->setMinimumSize(250,100);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowTitle(tr("Input the test case name"));
    dialog->setStatusTip(tr("Input the name of the test case you want to run."));
//-------------------------------------------------------------------------------------------------
    QGridLayout *glayout = new QGridLayout;
    glayout->setColumnStretch(0, 1);
    glayout->setColumnStretch(1, 3);
    glayout->setMargin(10);

    QLabel *choose_test = new QLabel(QWidget::tr("I want to run:"));
    QLineEdit *test_file = new QLineEdit();

    glayout->addWidget(choose_test, 0, 0);
    glayout->addWidget(test_file, 0, 1);

    QHBoxLayout *bomLayout = new QHBoxLayout;
    QPushButton *run_btn = new QPushButton(QWidget::tr("Run"));
    run_btn->setMinimumSize(50,25);
    bomLayout->addStretch();
    bomLayout->addWidget(run_btn);
    bomLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(glayout);
    mainLayout->addLayout(bomLayout);

    dialog->setLayout(mainLayout);

    connect(test_file, SIGNAL(textChanged(QString)), this, SLOT(get_string(const QString &)));
    connect(test_file, SIGNAL(textChanged(QString)), this, SLOT(get_path(const QString &)));
    connect(run_btn, SIGNAL(clicked()), this, SLOT(test_one()));
    connect(run_btn,&QPushButton::clicked, dialog, &QDialog::close);
//-----------------------------------------------------------------------------------
    dialog->show();
}

void MainWindow::get_string(const QString &str)
{
    QByteArray dome = str.toLocal8Bit();
    strcpy_s(_string, dome.data());
    _string[str.size()]='\0';
}

void MainWindow::get_string2(const QString &str)
{
    QByteArray dome = str.toLocal8Bit();
    strcpy_s(_string2, dome.data());
    _string2[str.size()]='\0';
}

void MainWindow::test_one()
{
    count++;
    connect(cmd , SIGNAL(readyReadStandardOutput()) , this,SLOT(runoneoutput()));

    get_string2(QDir::currentPath());
    cmd->write("cd ");
    cmd->write(_string2);cmd->write("\n");
    cmd->write("cd ../../../Y86-64_VS/River's_Y86-64/Release\n");
    cmd->write(" .\\River's_Y86-64.exe\n");

    get_string2(QString::number(max_memory));
    cmd->write(_string2);cmd->write("\n");

    get_string2(QString::number(run_speed));
    cmd->write(_string2);cmd->write("\n");

    if(cache_state==false) cmd->write("no_cache.\n");
    else cmd->write("use.\n");

    cmd->write("test_one.\n");
    cmd->write(_string);cmd->write("\n");
}

void MainWindow::runoneoutput()
{
    count++;
    if(count<4)
    {
         rubbish->append(cmd->readAllStandardOutput().data());
         return;
    }
    else if(count==9)
    {
        rubbish->append(cmd->readAllStandardOutput().data());
        count=0;
        Path+=".txt";
        copyPath+=".txt";
        disconnect(cmd , SIGNAL(readyReadStandardOutput()) , this,SLOT(runoneoutput()));

        progress->setRange(0,800);
        for(int i=1;i<=700;i++)
            progress->setValue(i);

       QDateTime curDateTime = QDateTime::currentDateTime();//延时1秒
        QDateTime nowDateTime;
        do
        {
            nowDateTime = QDateTime::currentDateTime();
            qApp->processEvents();
        }while(curDateTime.secsTo(nowDateTime) <= 1);

        openone();
        return;
    }
    textBrowser->append(cmd->readAllStandardOutput().data());
}

void MainWindow::openFile()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                tr("Open File"),
                                                "../../../Y86-64_VS/file",
                                                tr("Text Files(*.txt *.yo)"),
                                                0,QFileDialog::ShowDirsOnly);
    if(!path.isEmpty()) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, tr("Read File"),
                                 tr("Cannot open file:\n%1").arg(path));
            return;
        }
        QTextStream in(&file);
        if(Path=="other...") textBrowser->setText(in.readAll());
        else textEdit->setText(in.readAll());
        file.close();
    } else {
        QMessageBox::warning(this, tr("Path"),
                             tr("You did not select any file."));
    }
}

void MainWindow::saveFile()
{
    QString path = QFileDialog::getSaveFileName(this,
                                                tr("Open File"),
                                                "../../../Y86-64_VS/file",
                                                tr("Text Files(*.txt *.yo)"),
                                                0,QFileDialog::ShowDirsOnly);
    if(!path.isEmpty()) {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(this, tr("Write File"),
                                       tr("Cannot save file:\n%1").arg(path));
            return;
        }
        QTextStream out(&file);
        out << textEdit->toPlainText();
        file.close();
     } else {
        QMessageBox::warning(this, tr("Path"),
                             tr("You did not select any file."));
    }
}

void MainWindow::cleanall()
{
    count=0;
    textBrowser->clear();
    textEdit->clear();
    progress->setValue(0);
    progress->setFormat(tr("Current progress : %1%").arg(QString::number(0, 'f', 0)));
    cmd->close();
    cmd = new QProcess(this);    cmd->start("cmd");   cmd->waitForStarted();
    connect(cmd , SIGNAL(readyReadStandardError()) ,this, SLOT(readerror()));
}

void MainWindow::openfinal()
{
    Path="../../../Y86-64_VS/file/test result/";
    for(int i=36;i<copyPath.size();i++)
        Path+=copyPath[i];
    openone();
}

void MainWindow::opencache()
{
    Path="../../../Y86-64_VS/file/cache state/";
    for(int i=36;i<copyPath.size();i++)
        Path+=copyPath[i];
    openone();
}

void MainWindow::set_cache()
{
  //  qDebug()<<cache_state<<endl;
    cache_state=!cache_state;
}

MainWindow::~MainWindow()
{
    if(cmd)
    {
          cmd->close();
          cmd->waitForFinished();
    }
}
