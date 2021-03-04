#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSystemTrayIcon>
#include <qt_windows.h>
#include <QSet>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void keyDown(DWORD key);
    void keyUp(DWORD key);

    static void pressKey(DWORD vkKeyCode);
    void createSystemTrayIcon();
    bool deleteSystemTrayIcon();

public slots:
    void onTimeOut();
    void onExitApplication();
private:
    Ui::MainWindow *ui;
    HHOOK hhkLowLevelKybd;

    QSet<DWORD>      m_pressed_keys;
    QTimer*          m_timer;
    QSystemTrayIcon* m_system_tray_icon;
    QMenu*           m_tray_menu;
};
#endif // MAINWINDOW_H
