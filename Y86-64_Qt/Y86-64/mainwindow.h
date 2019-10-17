#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextBrowser>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QProcess>
#include <QString>
#include <QProgressBar>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QSpinBox>
#include <QSlider>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    QMenu *file;
    void openFile();
    void saveFile();
    void test_begin();
    void choose_one();
    void Readme();
    void Author();
    void choose();
    void cleanall();
    void openfinal();
    void Login();
    void judgeID();
    void success();
    void setChinese();
    void unsetChinese();
    void runacode();
    void _setting();
    void got_val();
    void Logout();
    void get_qtcode();
    void set_cache();
    void opencache();
    QAction *openAction;
    QAction *saveAction;
    QAction *runall;
    QAction *runone;
    QAction *readme;
    QAction *author;
    QAction *login;
    QAction *logout;
    QAction *setting;
    QAction *translatetoC;
    QAction *translatetoE;
    QAction *submitAction;
    QAction *Qtcode;
    QToolBar *toolBar;
    QTextBrowser *textBrowser;
    QTextBrowser *rubbish;
    QTextEdit *textEdit;
    QVBoxLayout* vlayout;
    QHBoxLayout* hl;
    QWidget *w;
    QWidget *w2;
    QWidget *widget;
    QProcess *cmd;
    QProgressBar *progress;
    QPushButton *final;
    QPushButton *clean;
    QLineEdit *id;
    QLineEdit *password;
    QLabel *IDcard;
    QDialog *logindialog;
    QLabel *timelabel;
    QLabel *datelabel;
    QPushButton *submit;
    QSpinBox *spinBox_m;
    QSpinBox *spinBox_s;
    QSlider *slider_m;
    QSlider *slider_s;
    QSlider *cache;
    QLabel *cachelabel;
    QPushButton *cachebt;
public slots:
        void runalloutput();
        void runoneoutput();
        void readerror();
        void get_path(const QString &str);
        void get_string(const QString &str);
        void get_string2(const QString &str);
        void openone();
        void test_one();
        void sl_time();
        void printaprocess();
};

#endif // MAINWINDOW_H
