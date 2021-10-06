#include "numpadtextedit.h"

#include <QEventLoop>
#include <QKeyEvent>
#include <QTimer>

#define SDL_SCANCODE_KP_000 0 // Define it! '000' button. It may differ from value defined in SDL

#define AutoDisconnect(l) \
    QSharedPointer<QMetaObject::Connection> l = QSharedPointer<QMetaObject::Connection>(\
        new QMetaObject::Connection(), \
        [](QMetaObject::Connection * conn) { /*QSharedPointer сам по себе не производит отключения при удалении*/ \
            QObject::disconnect(*conn);\
        }\
    ); *l //-- Use AutoDisconnect(conn1) = connect(....);


NumPadTextEdit::NumPadTextEdit(QWidget *parent) : QTextEdit(parent)
{
    m_combKeysTimer = new QTimer(this);
    //connect(m_combKeysTimer, &QTimer::timeout, this, &NumPadTextEdit::onCombKeysTimeout);
}

void
NumPadTextEdit::keyPressEvent(QKeyEvent *e)
{
    if (!((e->modifiers() & Qt::KeypadModifier)) || e->key() == Qt::Key_Enter)
        QTextEdit::keyPressEvent(e);

    m_key_queue.append(e->key());

    if (m_key_queue.count() == 1)
        eventLoopForComb();
}

void NumPadTextEdit::eventLoopForComb()
{
    QEventLoop _loop;

    AutoDisconnect(conn) = connect(m_combKeysTimer, &QTimer::timeout, [=,&_loop]() {
        onCombKeysTimeout();
        _loop.exit();
    });

    m_combKeysTimer->start(m_combKeysTimeout);

    _loop.exec();
}

void NumPadTextEdit::onCombKeysTimeout()
{
    if (m_key_queue.size() == 2) {
        if (m_key_queue.contains(SDL_SCANCODE_KP_000) && m_key_queue.contains(Qt::Key_Period)) {
            insertPlainText("w");
        } else if (m_key_queue.contains(SDL_SCANCODE_KP_000) && m_key_queue.contains(Qt::Key_0)) {
            insertPlainText("b");
        } else if (m_key_queue.contains(Qt::Key_1) && m_key_queue.contains(Qt::Key_2)) {
            insertPlainText("v");
        } else if (m_key_queue.contains(Qt::Key_1) && m_key_queue.contains(Qt::Key_4)) {
            insertPlainText("m");
        } else if (m_key_queue.contains(Qt::Key_2) && m_key_queue.contains(Qt::Key_3)) {
            insertPlainText("x");
        } else if (m_key_queue.contains(Qt::Key_2) && m_key_queue.contains(Qt::Key_5)) {
            insertPlainText("f");
        } else if (m_key_queue.contains(Qt::Key_3) && m_key_queue.contains(Qt::Key_6)) {
            insertPlainText("n");
        } else if (m_key_queue.contains(Qt::Key_3) && m_key_queue.contains(Qt::Key_Period)) {
            insertPlainText("c");
        } else if (m_key_queue.contains(Qt::Key_4) && m_key_queue.contains(Qt::Key_5)) {
            insertPlainText("p");
        } else if (m_key_queue.contains(Qt::Key_4) && m_key_queue.contains(Qt::Key_7)) {
            insertPlainText("l");
        } else if (m_key_queue.contains(Qt::Key_5) && m_key_queue.contains(Qt::Key_6)) {
            insertPlainText("k");
        } else if (m_key_queue.contains(Qt::Key_6) && m_key_queue.contains(Qt::Key_9)) {
            insertPlainText("y");
        } else if (m_key_queue.contains(Qt::Key_7) && m_key_queue.contains(Qt::Key_8)) {
            QString text = toPlainText();
            text.chop(1);
            clear();
            insertPlainText(text);
        } else if (m_key_queue.contains(Qt::Key_8) && m_key_queue.contains(Qt::Key_9)) {
            insertPlainText("r");
        } else if (m_key_queue.contains(Qt::Key_Asterisk) && m_key_queue.contains(Qt::Key_Minus)) {
            insertPlainText(".");
        } else if (m_key_queue.contains(Qt::Key_Asterisk) && m_key_queue.contains(Qt::Key_Slash)) {
            insertPlainText(",");
        }
    } else if (m_key_queue.size() == 1) {
        if (m_key_queue.back() == Qt::Key_9) {
            insertPlainText("j");
        } else if (m_key_queue.back() == Qt::Key_8) {
            insertPlainText("t");
        } else if (m_key_queue.back() == Qt::Key_7) {
            insertPlainText("q");
        } else if (m_key_queue.back() == Qt::Key_6) {
            insertPlainText("a");
        } else if (m_key_queue.back() == Qt::Key_5) {
            insertPlainText("e");
        } else if (m_key_queue.back() == Qt::Key_4) {
            insertPlainText("d");
        } else if (m_key_queue.back() == Qt::Key_3) {
            insertPlainText("s");
        } else if (m_key_queue.back() == Qt::Key_2) {
            insertPlainText("h");
        } else if (m_key_queue.back() == Qt::Key_1) {
            insertPlainText("o");
        } else if (m_key_queue.back() == Qt::Key_0) {
            insertPlainText("g");
        } else if (m_key_queue.back() == Qt::Key_Plus) {
            insertPlainText("z");
        } else if (m_key_queue.back() == Qt::Key_Period) {
            insertPlainText("u");
        } else if (m_key_queue.back() == Qt::Key_Asterisk) {
            insertPlainText("i");
        }
    }

    m_key_queue.clear();
}
