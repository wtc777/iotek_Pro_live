#include "VideoSurface.h"
#include <QList>
#include <QVideoFrame>
VideoSurface::VideoSurface(QObject *parent) : QAbstractVideoSurface(parent)
{

}

QList<QVideoFrame::PixelFormat> VideoSurface::
supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const
{
    return QList<QVideoFrame::PixelFormat>()<<QVideoFrame::Format_RGB32;
}

bool VideoSurface::present(const QVideoFrame &frame)
{
    emit frameChanged(frame);
    return true;
}



