#ifndef IPCONFIG_H
#define IPCONFIG_H

#include <QWidget>

namespace Ui {
class IPconfig;
}

class IPconfig : public QWidget
{
    Q_OBJECT

public:
    explicit IPconfig(const QString& ip, quint16 port, QWidget *parent = nullptr);
    ~IPconfig();

private slots:
    void on_pb_applyconfig_clicked();

private:
    Ui::IPconfig *ui;
    QString ip;
    quint16 port;
};

#endif // IPCONFIG_H
