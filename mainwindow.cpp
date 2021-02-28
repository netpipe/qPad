#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#define kSystemTrayIconPath ":/qtkey.png"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createSystemTrayIcon();
    nativeEventFilter = new NativeEventFilter(this);
    qApp->installNativeEventFilter(nativeEventFilter);  // Install it on the application
    connect(nativeEventFilter, &NativeEventFilter::activated, this, &MainWindow::slotGlobalHotkey);
    connect(nativeEventFilter, SIGNAL(pressedKeys(QString)),  this, SLOT(onGlobalHotkey(QString)));
    nativeEventFilter->setShortcut();   // Install hotkey

}

MainWindow::~MainWindow()
{
    nativeEventFilter->unsetShortcut();
    delete ui;
}

void MainWindow::slotGlobalHotkey()
{
    QMessageBox::information(this,"Global Hotkey", "Global Hotkey is worked", QMessageBox::Ok);
}

void MainWindow::onGlobalHotkey(QString result)
{
    QMessageBox::information(NULL, "Global Hotkey", result, QMessageBox::Ok);
}


bool MainWindow::deleteSystemTrayIcon()
{
    m_system_tray_icon->setVisible(false);

    QList<QAction*> actions = m_system_tray_icon->contextMenu()->actions();

    while (m_system_tray_icon->contextMenu()->actions().size() > 0)
    {
        QAction* action = m_system_tray_icon->contextMenu()->actions().takeFirst();
        if(action)
        {
            delete action;
        }
    }

    if(m_system_tray_icon->contextMenu())
    {
        delete m_system_tray_icon->contextMenu();
    }

    if(m_system_tray_icon)
    {
        delete m_system_tray_icon;
    }

    return true;
}


void MainWindow::createSystemTrayIcon()
{
    QPixmap pixamp;
    pixamp.load(kSystemTrayIconPath);

    m_system_tray_icon = new QSystemTrayIcon(pixamp);

    QAction* show_action = new QAction(tr("Show window"), m_system_tray_icon);
//    show_action->setIcon(QIcon(":/Resources/update.png"));
    show_action->setObjectName("ShowAction");

    QAction* exit_action = new QAction(tr("Exit"), m_system_tray_icon);
    exit_action->setObjectName("ExitAction");

    connect(show_action,			SIGNAL(triggered()),   this, SLOT(onShowMainWindow()));
    connect(exit_action,			SIGNAL(triggered()),   this, SLOT(onExitApplication()));

    QMenu* m_tray_menu = new QMenu(this);
    m_tray_menu->addAction(show_action);
    m_tray_menu->addAction(exit_action);

    m_system_tray_icon->setContextMenu(m_tray_menu);
    m_system_tray_icon->setVisible(true);
    m_system_tray_icon->setToolTip("Global ShortCut");
    m_system_tray_icon->showMessage("Qt Global ShortCut Test", "NumPad Test", QSystemTrayIcon::Information, 1000);
}


void MainWindow::onShowMainWindow()
{
    show();
}

void MainWindow::onExitApplication()
{
    close();
    qApp->quit();
}
