#ifndef NATIVEEVENTFILTER_H
#define NATIVEEVENTFILTER_H

#include <QObject>
#include <QAbstractNativeEventFilter>
#include <QTimer>

class NativeEventFilter : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    explicit NativeEventFilter(QObject *parent = nullptr);

    // Overriding the nativeEventFilter method
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
    void setShortcut();     // Adding a hotkey installation method
    void unsetShortcut();   // and hotkey removal method for completeness

signals:
    void activated();       // Hotkey activation signal
    void pressedKeys(QString);
public slots:

    void onTimeOut();
public:
    QTimer* m_timer;
};

#endif // NATIVEEVENTFILTER_H
