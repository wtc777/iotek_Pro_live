#ifndef VIDEOSURFACE_H
#define VIDEOSURFACE_H

#include <QAbstractVideoSurface>
class VideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    explicit VideoSurface(QObject *parent = nullptr);


    QList<QVideoFrame::PixelFormat>
    supportedPixelFormats(QAbstractVideoBuffer::HandleType type =
            QAbstractVideoBuffer::NoHandle) const;

    bool present(const QVideoFrame& frame);
signals:
    void frameChanged(QVideoFrame frame);
public slots:
};

#endif // VIDEOSURFACE_H
