#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QFont>
#include <QColor>
#include <QDebug>
#include <QFile>
#include <QPixmap>
#include <QSplashScreen>
#include <QDateTime>
#include <QElapsedTimer>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
     QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
     QFont font;
     font.setFamily("MS Shell Dlg 2"); // Tahoma 宋体
     qApp->setFont(font);
 //------------------------------------------------------------------------------------------
    QPixmap pixmap(":/image/screen");
    QPixmap pixmap2 = pixmap.scaled(450, 720, Qt::IgnoreAspectRatio);
     QSplashScreen *SplashScreen = new QSplashScreen;
     SplashScreen->setPixmap(pixmap2);

     Qt::Alignment align = Qt::AlignTop | Qt::AlignRight;
     SplashScreen->showMessage("Loading....",align,Qt::black);

     QProgressBar *p=new QProgressBar;
     p->setGeometry(0,pixmap.height()-50,pixmap.width(),5);
     p->setStyleSheet("QProgressBar {border:2px solid grey; border-radius:4px;text-align:center;}QProgressBar::chunk {background-color: rgb(202, 165, 14);width:16px;margin:0.5px}");
     p->setRange(0, 999000);
     p->setValue(0);

     QVBoxLayout *vlay=new QVBoxLayout;
     vlay->addStretch();
     vlay->addWidget(p);
     SplashScreen->setLayout(vlay);
     SplashScreen->move(450,3);
    SplashScreen->show();

     QDateTime curDateTime = QDateTime::currentDateTime();
     QDateTime nowDateTime;
     do
     {
         int v=p->value();
         for(int i=0;i<=5;i++) p->setValue(v+i);
         nowDateTime = QDateTime::currentDateTime();     
         app.processEvents();
     }while(curDateTime.secsTo(nowDateTime) <= 5);
//------------------------------------------------------------------------------------------------
    app.setStyleSheet("QPushButton{border: 2px solid grey; background:rgb(183,203,188); border-radius: 8px;}QPushButton:hover{border-color:rgb(183,203,188);}  QPushButton:pressed{border-color:gray}  QMainWindow,QDialog{background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 white, stop:0.5 rgb(89,222,255) , stop:1 rgb(3,56,182));}  QLineEdit{border: 1px solid gray;border-radius: 3px;background:rgb(200, 231, 232);} QLineEdit:hover{border-color:transparent; }");
    MainWindow win;
    win.setMinimumSize(450,650);

    app.setWindowIcon(QIcon("pika.ico"));

    win.move ( 450, 30 );
    win.show();
    SplashScreen->close();

    return app.exec();
}
