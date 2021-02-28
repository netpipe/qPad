#include "nativeeventfilter.h"
#include <QVector>
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <xcb/xcb.h>
#include <QSet>

namespace
{
    Display * m_display;        // X11 server connection
    Window m_win;               // Captured window - in this case the whole system will be
#if 0
    int keycode;                // key code
#endif
    QSet<int> m_key_codes;
    QSet<int> m_pressed_keys;
    unsigned int modifier;      // modifier code

    /* Vector of additional modifiers Num Lock, Caps lock
     * They are also taken into account in X11, so all possible combinations are needed
    **/
    QVector<quint32> maskModifiers(){
//        return QVector<quint32>() << 0 << Mod2Mask << LockMask << (Mod2Mask | LockMask);
        return QVector<quint32>() << 0 << Mod2Mask;
    }
}

NativeEventFilter::NativeEventFilter(QObject *parent) : QObject(parent)
{
    m_display = QX11Info::display();        // Let's create a connection to the server
    m_win = DefaultRootWindow(m_display);   // and pull out the captured window from it using a macro

    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
}

bool NativeEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType)
    Q_UNUSED(result)

    /* In this case, the event handling is already built on the XCB library instead of Xlib.
    *  Kind of like, receiving an event Qt knows it as an XCB event,
    *  but doesn't know it as an Xlib event, although use more
    *  easy Xlib syntax for installing hotkeys no one forbids us
    **/
    xcb_key_press_event_t *keyEvent = 0;

    // So we check that it was an xcb event
    if (eventType == "xcb_generic_event_t") {
        // We customize the message in the xcb event itself
        xcb_generic_event_t *event = static_cast<xcb_generic_event_t *>(message);

        // Check that the key was pressed
        if ((event->response_type & 127) == XCB_KEY_PRESS){

            // If so, then we cast the message into the keypress event
            keyEvent = static_cast<xcb_key_press_event_t *>(message);

            // Next, we check if this event is a desired hotkey
#if 0
            foreach (quint32 maskMods, maskModifiers()) {
                if((keyEvent->state == (modifier | maskMods ))
                        &&  keyEvent->detail == keycode){
                    emit activated();   // send a signal
                    return true;
#else
            foreach (int keycode, m_key_codes) {
                if((keyEvent->state == (modifier)) &&  keyEvent->detail == keycode){
                    m_pressed_keys += keycode;
                    if(m_pressed_keys.count() >= 3)
                    {
                        m_timer->start(300);
                    }
#endif
                }
            }
        }
        else if((event->response_type & 127) == XCB_KEY_RELEASE)
        {
            keyEvent = static_cast<xcb_key_press_event_t *>(message);
            foreach (int keycode, m_key_codes) {
                if((keyEvent->state == (modifier)) &&  keyEvent->detail == keycode){
                    m_pressed_keys -= keycode;
                    m_timer->stop();
                }
            }
        }
    }
    return false;
}

void NativeEventFilter::setShortcut()
{
/* First, to be on the safe side, disable the supposed hotkey,
 * even though there will be garbage for the first time in the hotkey parameters.
 * */
    unsetShortcut();
#if 0
    // Get the keycode from the KeySym definition and connection to the X11 server
    keycode = XKeysymToKeycode(m_display, XK_E);
    modifier = ControlMask; // Let's set a modifier
    /* And now let's go through all possible combinations, taking into account Num Lock and Caps Lock
     * installing hotkeys
     * */
    foreach (quint32 maskMods, maskModifiers())
    {
        XGrabKey(m_display,             // Specify the connection to X11
                 keycode ,              // key code
                 modifier | maskMods,   // modifier with all possible masks
                 m_win,                 // Capture window
                 True,                  // Whether the application is the owner of the event. in this example it is not important.
                 GrabModeAsync,         // Mandatory Asynchronous processing mode, otherwise, you run the risk of sticking
                 GrabModeAsync);        // with a frozen system that does not react to any influences if
                                        // do not write in advance the correct transmission of the event back to the system,
                                        // and most likely it will be
    }
#else

    m_key_codes << XKeysymToKeycode(m_display, XK_KP_0)
                   << XKeysymToKeycode(m_display, XK_KP_1)
                   << XKeysymToKeycode(m_display, XK_KP_2)
                      << XKeysymToKeycode(m_display, XK_KP_3)
                         << XKeysymToKeycode(m_display, XK_KP_4)
                            << XKeysymToKeycode(m_display, XK_KP_5)
                               << XKeysymToKeycode(m_display, XK_KP_6)
                                  << XKeysymToKeycode(m_display, XK_KP_7)
                                     << XKeysymToKeycode(m_display, XK_KP_8)
                                        << XKeysymToKeycode(m_display, XK_KP_9);
    modifier = Mod2Mask;
    foreach (int keycode, m_key_codes)
    {
        XGrabKey(m_display,
                 keycode ,
                 modifier,
                 m_win,
                 True,
                 GrabModeAsync,
                 GrabModeAsync);


    }
#endif
}

void NativeEventFilter::unsetShortcut()
{
#if 0
    // We go through all possible combinations and remove the hotkey
    foreach (quint32 maskMods, maskModifiers()) {
        XUngrabKey(m_display,
                   keycode,
                   modifier | maskMods,
                   m_win);
    }
#else
    foreach (int keycode, m_key_codes) {
        XUngrabKey(m_display,
                   keycode,
                   modifier,
                   m_win);
    }

#endif
}

void NativeEventFilter::onTimeOut()
{
    int count = m_pressed_keys.count();

    if(m_pressed_keys.contains(XKeysymToKeycode(m_display, XK_KP_2))
            && m_pressed_keys.contains(XKeysymToKeycode(m_display, XK_KP_4))
            && m_pressed_keys.contains(XKeysymToKeycode(m_display, XK_KP_8)))
    {
        QString out_str = "** KeyPressed ** 2 + 4 + 8 : A";
        emit pressedKeys(out_str);
    }
    else if(m_pressed_keys.contains(XKeysymToKeycode(m_display, XK_KP_8))
            && m_pressed_keys.contains(XKeysymToKeycode(m_display, XK_KP_6))
            && m_pressed_keys.contains(XKeysymToKeycode(m_display, XK_KP_2)))
    {
        QString out_str = "** KeyPressed ** 8 + 6 + 2 : A";
        emit pressedKeys(out_str);
    }
}
