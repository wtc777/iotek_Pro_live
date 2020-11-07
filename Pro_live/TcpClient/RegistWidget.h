#ifndef REGISTWIDGET_H
#define REGISTWIDGET_H

#include <QWidget>
#include <QCloseEvent>
namespace Ui {
class RegistWidget;
}

class RegistWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RegistWidget(QWidget *parent = nullptr);
    ~RegistWidget();
signals:
    void gobackwidget();//返回上一界面
private slots:
    void on_pb_regist_clicked();

private:
    Ui::RegistWidget *ui;

    void closeEvent(QCloseEvent* ev);
};

#endif // REGISTWIDGET_H
