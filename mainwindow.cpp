#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>

#include <X11/Xlib.h>           // `apt-get install libx11-dev`
#include <X11/Xmu/WinUtil.h>    // `apt-get install libxmu-dev`

Display* d;

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

    d = XOpenDisplay(NULL);



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





  //  QString test="xdotool key ";
  //          test.append('"');
  //          test.append("Shift+Insert");
 //           test.append( '"');
           //qDebug() << test;



 //   QStringList arguments;
 //   arguments << "./paste.sh";
    //result=$(wmctrl -l | grep 'TODO - gvim' | grep -Eo '0x[0-9a-f]+')
    //arguments << "window=$(xdotool getactivewindow);xdotool windowactivate $window key Shift+Insert;";
 //   QProcess::execute( "/bin/sh", arguments );


         //     Display* d;
              Window w;
              int revert_to;
              printf("getting input focus window ... ");
              XGetInputFocus(d, &w, &revert_to); // see man



}



//https://stackoverflow.com/questions/1201179/how-to-identify-top-level-x11-windows-using-xlib
void enumerateXWindows(Display *display, Window rootWindow)
{
    Window parent;
    Window *children;
    Window *child;
    quint32 nNumChildren;
    XTextProperty wmName;
    XTextProperty wmCommand;
    int status = XGetWMName(display, rootWindow, &wmName);
    if (status && wmName.value && wmName.nitems)
    {
        int i;
        char **list;
        status = XmbTextPropertyToTextList(display, &wmName, &list, &i);
        if (status >= Success && i && *list)
        {
            qDebug() << "Found window with name:" << (char*) *list;
        }
        status = XGetCommand(display, rootWindow, &list, &i);
        if (status >= Success && i && *list)
        {
            qDebug() << "... and Command:" << i << (char*) *list;
        }
        Window tf;
        status = XGetTransientForHint(display, rootWindow, &tf);
        if (status >= Success && tf)
        {
            qDebug() << "TF set!";
        }
        XWMHints *pHints = XGetWMHints(display, rootWindow);
        if (pHints)
        {
            qDebug() << "Flags:" << pHints->flags
                    << "Window group:" << pHints->window_group;
        }
    }
    status = XQueryTree(display, rootWindow, &rootWindow, &parent, &children, &nNumChildren);
    if (status == 0)
    {
        // Could not query window tree further, aborting
        return;
    }
    if (nNumChildren == 0)
    {
        // No more children found. Aborting
        return;
    }
    for (int i = 0; i < nNumChildren; i++)
    {
        enumerateXWindows(display, children[i]);
    }
    XFree((char*) children);


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
