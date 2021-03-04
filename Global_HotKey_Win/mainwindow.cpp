#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#define kSystemTrayIconPath ":/qtkey.png"

MainWindow * g_main_windows = nullptr;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        if(PKBDLLHOOKSTRUCT(lParam)->vkCode == VK_NUMLOCK)
        {
            return CallNextHookEx(NULL, nCode, wParam, lParam);
        }

        switch (wParam)
        {
            // Pass KeyDown/KeyUp messages for Qt class to logicize
            case WM_KEYDOWN:
                g_main_windows->keyDown(PKBDLLHOOKSTRUCT(lParam)->vkCode);
            break;
            case WM_KEYUP:
                g_main_windows->keyUp(PKBDLLHOOKSTRUCT(lParam)->vkCode);
            break;
        }
    }
    return true; // block all keyboard input
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createSystemTrayIcon();
    g_main_windows = this;
    hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);

    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
}

MainWindow::~MainWindow()
{
    if(m_timer)
    {
        m_timer->stop();
        delete m_timer;
    }
    deleteSystemTrayIcon();
    UnhookWindowsHookEx(hhkLowLevelKybd);
    delete ui;
}

void MainWindow::keyDown(DWORD vkKeyCode)
{
    bool num_lock_pressed = ((((ushort)GetKeyState(VK_NUMLOCK)) & 0xffff) != 0);
    if(!num_lock_pressed)
    {
        qDebug() << "Num lock is not pressed;";
        m_pressed_keys.clear();
        m_timer->stop();
        return;
    }

    m_pressed_keys += vkKeyCode;

    if((m_pressed_keys.size() == 3) &&
        ( (m_pressed_keys.contains(VK_NUMPAD2)
          && m_pressed_keys.contains(VK_NUMPAD4)
          && m_pressed_keys.contains(VK_NUMPAD8))
          ||
          (m_pressed_keys.contains(VK_NUMPAD2)
          && m_pressed_keys.contains(VK_NUMPAD6)
          && m_pressed_keys.contains(VK_NUMPAD8))))
    {
        m_timer->start(300);
    }
}

void MainWindow::keyUp(DWORD vkKeyCode)
{
    bool num_lock_pressed = ((((ushort)GetKeyState(VK_NUMLOCK)) & 0xffff) != 0);
    if(!num_lock_pressed)
    {
        qDebug() << "Num lock is not pressed;";
        m_pressed_keys.clear();
        m_timer->stop();
        return;
    }

    m_pressed_keys -= vkKeyCode;
    m_timer->stop();
}

void MainWindow::onTimeOut()
{
    QString out_str;
    if(m_pressed_keys.contains(VK_NUMPAD2) && m_pressed_keys.contains(VK_NUMPAD4) && m_pressed_keys.contains(VK_NUMPAD8))
    {
        out_str = "** KeyPressed ** 2 + 4 + 8 : A";
    }
    else if(m_pressed_keys.contains(VK_NUMPAD8) && m_pressed_keys.contains(VK_NUMPAD6) && m_pressed_keys.contains(VK_NUMPAD2))
    {
        out_str = "** KeyPressed ** 8 + 6 + 2 : A";
    }
    qDebug() << out_str;
    QMessageBox::information(nullptr, tr("GlobalHotKey"), out_str);
}


bool MainWindow::deleteSystemTrayIcon()
{
    m_system_tray_icon->setVisible(false);

    QList<QAction*> actions = m_tray_menu->actions();

    while (m_tray_menu->actions().size() > 0)
    {
        QAction* action = m_tray_menu->actions().takeFirst();
        if(action)
        {
            delete action;
        }
    }

    if(m_system_tray_icon)
    {
        delete m_system_tray_icon;
    }
    if(m_tray_menu)
    {
        delete m_tray_menu;
    }

    return true;
}


void MainWindow::createSystemTrayIcon()
{
    QPixmap pixamp;
    pixamp.load(kSystemTrayIconPath);

    m_system_tray_icon = new QSystemTrayIcon(pixamp);

    QAction* exit_action = new QAction(tr("Exit"), m_system_tray_icon);
    exit_action->setObjectName("ExitAction");

    connect(exit_action,			SIGNAL(triggered()),   this, SLOT(onExitApplication()));

    m_tray_menu = new QMenu();
    m_tray_menu->addAction(exit_action);

    m_system_tray_icon->setContextMenu(m_tray_menu);
    m_system_tray_icon->setVisible(true);
    m_system_tray_icon->setToolTip("Global ShortCut");
    m_system_tray_icon->showMessage("Qt Global ShortCut Test", "NumPad Test", QSystemTrayIcon::Information, 1000);
}

void MainWindow::onExitApplication()
{
    close();
    qApp->quit();
}


//void MainWindow::pressKey(DWORD vkKeyCode)
//{
//    INPUT Input;
//    // Set up a generic keyboard event.
//    Input.type = INPUT_KEYBOARD;
//    Input.ki.wScan = 0;
//    Input.ki.time = 0;
//    Input.ki.dwExtraInfo = 0;
//    Input.ki.dwFlags = 0;

//    Input.ki.wVk = vkKeyCode;
//    SendInput(1, &Input, sizeof(INPUT));
//}
