#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QPalette>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setFont(QFont("Microsoft Yahei", 9));
    QFile qss(":/lightblue.css");
    if(qss.open(QFile::ReadOnly))
    {
        qDebug() << "stylesheet open success!";
        QString style = QLatin1String(qss.readAll());
        QString paletteColor = style.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        a.setStyleSheet(style);
        qss.close();
    }
    else
    {
        qDebug() << "stylesheet open fail!";
    }

    MainWindow w;
    w.show();

    return a.exec();
}
