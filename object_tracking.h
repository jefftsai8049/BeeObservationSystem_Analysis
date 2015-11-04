#ifndef OBJECT_TRACKING_H
#define OBJECT_TRACKING_H

#include <QObject>
#include <QWidget>

class object_tracking : public QObject
{
    Q_OBJECT
public:
    explicit object_tracking(QObject *parent = 0);

signals:

public slots:
};

#endif // OBJECT_TRACKING_H
