#ifndef ROOMOPEWIDGET_H
#define ROOMOPEWIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include "Protocol.h"
#include <QMovie>
class QTimer;

namespace Ui {
class RoomOpeWidget;
}

class RoomOpeWidget : public QWidget
{
    Q_OBJECT

public:
    static RoomOpeWidget* getInstance();

    void RoomListFlush(const PDU &package);

    void displayLabelMovie();

    void flush();
    ~RoomOpeWidget();
public slots:
    void doubleClickedLW(QListWidgetItem *item);
private slots:
    void on_pbCreateMyroom_clicked();

    void on_pbFlushList_clicked();

private:
    explicit RoomOpeWidget(QWidget *parent = nullptr);
    RoomOpeWidget(const RoomOpeWidget& rhs) = delete ;
    const RoomOpeWidget& operator =(const RoomOpeWidget& rhs) = delete;
    static RoomOpeWidget* instance;

    Ui::RoomOpeWidget *ui;

    QTimer* cycFlushRoomList;
    QMovie* waitmovie;
};

#endif // ROOMOPEWIDGET_H
