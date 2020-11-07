#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void loadconfig();

    ~MainWindow();

private slots:
    void on_btnListen_clicked();

    void newconnectSlot();

    void on_pb_showLog_clicked();

    void on_pb_changeIP_clicked();

protected:

private:
    Ui::MainWindow *ui;
    QTcpServer* m_tcpserver;
    QString IP;
    quint16 port;

};
#endif // MAINWINDOW_H
