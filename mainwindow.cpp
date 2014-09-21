#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <cstdlib>

#include <unistd.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    int i;
    thumbnails[20] = {0};

    ui->setupUi(this);
    QApplication::setQuitOnLastWindowClosed(true);

    setWindowFlags(Qt::Popup);
    setStyleSheet("background-color: black; color: white;");

    layout = new QGridLayout;

    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);

    long unsigned workspaces = getWorkspaceCount();
    setWorkspace(0);

    for (i = 0; i < workspaces; i++) {
        setWorkspace(i);
        setThumbnail(i);
    }

    setWorkspace(0);

    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Popup);

    XClientMessageEvent xev;
    Display *display = XOpenDisplay(0);

    xev.type = ClientMessage;
    xev.window = this->winId();
    xev.send_event = True;
    xev.display = display;
    xev.message_type = XInternAtom(display, "_NET_WM_DESKTOP", True);
    xev.format = 32;
    xev.data.l[0] = -1;

    XSendEvent(display, RootWindow(display, 0), False,
         (SubstructureNotifyMask | SubstructureRedirectMask),
         (XEvent *) & xev);

    adjustSize();
    move(QApplication::desktop()->screen()->rect().center() - rect().center());
}

MainWindow::~MainWindow()
{
    delete ui;
}

long unsigned MainWindow::getWorkspaceCount()
{
    Display  *display;
    Atom     actual_type;
    int      actual_format;
    long unsigned    nitems;
    long unsigned    bytes;
    long unsigned    *data;
    int      status;
    long unsigned count;

    display = XOpenDisplay(0);
    if (!display) {
            printf("can't open display");
            exit(1);
    }

    status = XGetWindowProperty(
            display,
            RootWindow(display, 0),
            XInternAtom(display, "_NET_NUMBER_OF_DESKTOPS", True), //replace this with your property
            0,
            (~0L),
            False,
            AnyPropertyType,
            &actual_type,
            &actual_format,
            &nitems,
            &bytes,
            (unsigned char**)&data);
    if (status != Success) {
            fprintf(stderr, "status = %d\n", status);
            exit(1);
    }

    count = *data;

    return count;
}

int MainWindow::getWorkspace()
{
    Display  *display;
    Atom     actual_type;
    int      actual_format;
    long unsigned    nitems;
    long unsigned    bytes;
    unsigned char   *data;
    int      status;
    long unsigned count;

    display = XOpenDisplay(0);
    if (!display) {
            printf("can't open display");
            exit(1);
    }

    status = XGetWindowProperty(
        display,
        RootWindow(display, 0),
        XInternAtom(display, "_NET_CURRENT_DESKTOP", True), //replace this with your property
        0,
        32,
        False,
        XA_CARDINAL,
        &actual_type,
        &actual_format,
        &nitems,
        &bytes,
        &data
    );

    if (status != Success) {
            fprintf(stderr, "status = %d\n", status);
            exit(1);
    }

        int r = std::atoi( reinterpret_cast<char( & )[sizeof(data)]>( data ) );


    XCloseDisplay(display);

    return r;
}

void MainWindow::setWorkspace(int i) {
    Display *display = XOpenDisplay(0);

    if (!display) {
        printf("can't open display");
        exit(1);
    }

    XClientMessageEvent xev;

    xev.type = ClientMessage;
    xev.window = RootWindow(display, 0);
    xev.send_event = True;
    xev.display = display;
    xev.message_type = XInternAtom(display, "_NET_CURRENT_DESKTOP", True);
    xev.format = 32;
    xev.data.l[0] = i;

    XSendEvent(
        display,
        RootWindow(display, 0),
        False,
        (SubstructureNotifyMask | SubstructureRedirectMask),
        (XEvent *) &xev
    );

    XFlush(display);

    while (True) {
        int workspace = getWorkspace();

        if(workspace == i) {
            break;
        }

        usleep(5);
    }

    XCloseDisplay(display);
}

void MainWindow::setThumbnail(int i) {
    QLabel *screenshotLabel;

    if (thumbnails[i] == NULL) {
        screenshotLabel = new QLabel;
        screenshotLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        screenshotLabel->setAlignment(Qt::AlignCenter);
        screenshotLabel->setMinimumSize(240, 160);

        layout->addWidget(screenshotLabel, i / 3, i % 3);
        thumbnails[i] = screenshotLabel;
    }
    else {
        screenshotLabel = thumbnails[i];
    }

    QScreen *srn = QApplication::screens().at(0);
    QPixmap workspacePixmap = srn->grabWindow(QApplication::desktop()->winId());

    screenshotLabel->setPixmap(
        workspacePixmap.scaled(
            screenshotLabel->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        )
    );
}
