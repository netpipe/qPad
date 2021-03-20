#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include "nativeeventfilter.h"





namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

//    void enumerateXWindows(Display *display, Window rootWindow);

    void createSystemTrayIcon();
    bool deleteSystemTrayIcon();
private slots:
    void onGlobalHotkey(QString result);
    void slotGlobalHotkey();

    void onShowMainWindow();
    void onExitApplication();
private:
    Ui::MainWindow *ui;
    NativeEventFilter *nativeEventFilter;
    QSystemTrayIcon* m_system_tray_icon;
//    QMenu*           m_tray_menu;




};

#endif // MAINWINDOW_H
