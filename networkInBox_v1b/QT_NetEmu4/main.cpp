#include <QtWidgets/QApplication>
#include "mainwindow.h"
#include <cmath>
#include <QCommandLineParser>
#include <QDebug>

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QCommandLineParser parser;
    // Usage: image-editor file
    //
    // Arguments:
    // file                  The file to open.
    parser.addPositionalArgument("file", QCoreApplication::translate("main", "The file to open."));
    // Process the actual command line arguments given by the user
    parser.process(a);
    const QStringList args = parser.positionalArguments();
    if (args.size() != 1) {
           fprintf(stderr, "%s\n", qPrintable(QCoreApplication::translate("main", "Error: Must specify one filename argument.")));
           parser.showHelp(1);
    }
    qDebug() << "filename:  " << args.at(0);
    MainWindow w(a.arguments());
    //MainWindow w;
    w.show();
    return a.exec();
}
