#include <QApplication>
#include <QCoreApplication>
#ifndef QT_NO_SYSTEMTRAYICON
#include "headers/launcher.h"
#include <QMessageBox>
#include <QGuiApplication>
#include <QStyleHints>
#include <QDesktopWidget>
#include "headers/auto_start.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(openstreamapp);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication a(argc, argv);

    /*Auto startup logic*/
    if(!auto_start_exists()) {
        qDebug() << "Set auto start after login." << Qt::endl;
        QString app_dir = QDir::toNativeSeparators(QCoreApplication::applicationDirPath());
        QString app_name = QCoreApplication::applicationName() + ".exe";
        set_windows_auto_start(app_dir.toStdString(), app_name.toStdString());
    }
    else {
        qDebug() << "Auto start already set." << Qt::endl;
    }
    /*Auto startup logic*/

    if(!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, "Sunshine Launcher",
                              QObject::tr("I couldn't detect any system tray on the system."));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);

    Launcher w;

    if (!QGuiApplication::styleHints()->showIsFullScreen() && !QGuiApplication::styleHints()->showIsMaximized()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(&w);
        w.resize(availableGeometry.width() / 3, availableGeometry.height() * 2 / 3);
        w.setMaximumSize(availableGeometry.width() / 3, availableGeometry.height() * 1 / 3);
        w.setMinimumSize(availableGeometry.width() / 3, availableGeometry.height() * 1 / 3);
        w.move((availableGeometry.width() - w.width()) / 2,
                    (availableGeometry.height() - w.height()) / 2);
    }
    w.show();
    QTimer::singleShot(1000 / 2, &w, &Launcher::on_event_loop_started);
    return a.exec();
}

#else

#include <QLabel>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString text("QSystemTrayIcon is not supportd on this platform");

    QLabel *label = new QLabel(text);
    label->setWordWrap(true);
    label->show();
    qDebug() << text;
    app.exec();
}

#endif
