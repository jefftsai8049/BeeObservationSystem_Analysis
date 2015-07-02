#ifndef TRAJECTORY_TRACKING_H
#define TRAJECTORY_TRACKING_H

#include <QObject>

class trajectory_tracking : public QObject
{
    Q_OBJECT
public:
    explicit trajectory_tracking(QObject *parent = 0);
    ~trajectory_tracking();

signals:

public slots:
};

#endif // TRAJECTORY_TRACKING_H
