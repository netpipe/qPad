#ifndef NUMPADTEXTEDIT_H
#define NUMPADTEXTEDIT_H

#include <QTextEdit>
#include <QObject>

class NumPadTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    NumPadTextEdit(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *e) override;

private:
    void eventLoopForComb();

private slots:
    void onCombKeysTimeout();

private:
    QTimer *m_combKeysTimer;
    int m_combKeysTimeout = 200; // ms
    Qt::Key m_last_key; // First fake modifier
    Qt::Key m_mod_key_one; // First fake modifier
    Qt::Key m_mod_key_second; // Second fake modifier
    QVector<int> m_key_queue;
    int m_three_zeros; // "000" comb
};

#endif // NUMPADTEXTEDIT_H
