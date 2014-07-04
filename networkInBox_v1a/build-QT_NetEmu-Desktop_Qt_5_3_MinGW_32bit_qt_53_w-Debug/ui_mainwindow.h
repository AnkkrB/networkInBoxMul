/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QComboBox *comboBox_adapter1;
    QFrame *line_2;
    QLabel *label_sent1;
    QLabel *label_4;
    QLineEdit *lineEdit_macFilter1;
    QLabel *label_dropped1;
    QLabel *label_3;
    QSpinBox *spinBox_packetLoss1;
    QLabel *label_bandwidth1;
    QLabel *label_17;
    QLabel *label_buffer1;
    QDoubleSpinBox *doubleSpinBox_bandwith1;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QComboBox *comboBox_adapter2;
    QFrame *line;
    QLabel *label_sent2;
    QLabel *label_5;
    QLineEdit *lineEdit_macFilter2;
    QLabel *label_dropped2;
    QLabel *label_10;
    QSpinBox *spinBox_packetLoss2;
    QLabel *label_bandwidth2;
    QLabel *label_12;
    QLabel *label_buffer2;
    QDoubleSpinBox *doubleSpinBox_bandwith2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_refreshInterfaceList;
    QPushButton *pushButton_start;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuFile;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(581, 376);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMinimumSize(QSize(0, 0));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        comboBox_adapter1 = new QComboBox(groupBox);
        comboBox_adapter1->setObjectName(QStringLiteral("comboBox_adapter1"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboBox_adapter1->sizePolicy().hasHeightForWidth());
        comboBox_adapter1->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(comboBox_adapter1, 0, 1, 1, 1);

        line_2 = new QFrame(groupBox);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line_2, 0, 2, 5, 1);

        label_sent1 = new QLabel(groupBox);
        label_sent1->setObjectName(QStringLiteral("label_sent1"));

        gridLayout_2->addWidget(label_sent1, 0, 3, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_2->addWidget(label_4, 1, 0, 1, 1);

        lineEdit_macFilter1 = new QLineEdit(groupBox);
        lineEdit_macFilter1->setObjectName(QStringLiteral("lineEdit_macFilter1"));

        gridLayout_2->addWidget(lineEdit_macFilter1, 1, 1, 1, 1);

        label_dropped1 = new QLabel(groupBox);
        label_dropped1->setObjectName(QStringLiteral("label_dropped1"));

        gridLayout_2->addWidget(label_dropped1, 1, 3, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_2->addWidget(label_3, 2, 0, 1, 1);

        spinBox_packetLoss1 = new QSpinBox(groupBox);
        spinBox_packetLoss1->setObjectName(QStringLiteral("spinBox_packetLoss1"));
        spinBox_packetLoss1->setMaximum(100);

        gridLayout_2->addWidget(spinBox_packetLoss1, 2, 1, 1, 1);

        label_bandwidth1 = new QLabel(groupBox);
        label_bandwidth1->setObjectName(QStringLiteral("label_bandwidth1"));
        label_bandwidth1->setMinimumSize(QSize(160, 0));

        gridLayout_2->addWidget(label_bandwidth1, 2, 3, 1, 1);

        label_17 = new QLabel(groupBox);
        label_17->setObjectName(QStringLiteral("label_17"));

        gridLayout_2->addWidget(label_17, 4, 0, 1, 1);

        label_buffer1 = new QLabel(groupBox);
        label_buffer1->setObjectName(QStringLiteral("label_buffer1"));

        gridLayout_2->addWidget(label_buffer1, 4, 3, 1, 1);

        doubleSpinBox_bandwith1 = new QDoubleSpinBox(groupBox);
        doubleSpinBox_bandwith1->setObjectName(QStringLiteral("doubleSpinBox_bandwith1"));
        doubleSpinBox_bandwith1->setMaximum(1000);
        doubleSpinBox_bandwith1->setValue(100);

        gridLayout_2->addWidget(doubleSpinBox_bandwith1, 3, 1, 2, 1);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(0, 0));
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        comboBox_adapter2 = new QComboBox(groupBox_2);
        comboBox_adapter2->setObjectName(QStringLiteral("comboBox_adapter2"));
        sizePolicy1.setHeightForWidth(comboBox_adapter2->sizePolicy().hasHeightForWidth());
        comboBox_adapter2->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(comboBox_adapter2, 0, 1, 1, 1);

        line = new QFrame(groupBox_2);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 0, 2, 7, 1);

        label_sent2 = new QLabel(groupBox_2);
        label_sent2->setObjectName(QStringLiteral("label_sent2"));

        gridLayout->addWidget(label_sent2, 0, 3, 1, 1);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 2, 0, 1, 1);

        lineEdit_macFilter2 = new QLineEdit(groupBox_2);
        lineEdit_macFilter2->setObjectName(QStringLiteral("lineEdit_macFilter2"));

        gridLayout->addWidget(lineEdit_macFilter2, 1, 1, 2, 1);

        label_dropped2 = new QLabel(groupBox_2);
        label_dropped2->setObjectName(QStringLiteral("label_dropped2"));

        gridLayout->addWidget(label_dropped2, 2, 3, 1, 1);

        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout->addWidget(label_10, 4, 0, 1, 1);

        spinBox_packetLoss2 = new QSpinBox(groupBox_2);
        spinBox_packetLoss2->setObjectName(QStringLiteral("spinBox_packetLoss2"));

        gridLayout->addWidget(spinBox_packetLoss2, 3, 1, 2, 1);

        label_bandwidth2 = new QLabel(groupBox_2);
        label_bandwidth2->setObjectName(QStringLiteral("label_bandwidth2"));
        label_bandwidth2->setMinimumSize(QSize(160, 0));

        gridLayout->addWidget(label_bandwidth2, 4, 3, 1, 1);

        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout->addWidget(label_12, 6, 0, 1, 1);

        label_buffer2 = new QLabel(groupBox_2);
        label_buffer2->setObjectName(QStringLiteral("label_buffer2"));

        gridLayout->addWidget(label_buffer2, 6, 3, 1, 1);

        doubleSpinBox_bandwith2 = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_bandwith2->setObjectName(QStringLiteral("doubleSpinBox_bandwith2"));
        doubleSpinBox_bandwith2->setMaximum(1000);
        doubleSpinBox_bandwith2->setValue(100);

        gridLayout->addWidget(doubleSpinBox_bandwith2, 5, 1, 2, 1);


        verticalLayout->addWidget(groupBox_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        pushButton_refreshInterfaceList = new QPushButton(centralWidget);
        pushButton_refreshInterfaceList->setObjectName(QStringLiteral("pushButton_refreshInterfaceList"));
        pushButton_refreshInterfaceList->setEnabled(true);

        horizontalLayout_3->addWidget(pushButton_refreshInterfaceList);

        pushButton_start = new QPushButton(centralWidget);
        pushButton_start->setObjectName(QStringLiteral("pushButton_start"));

        horizontalLayout_3->addWidget(pushButton_start);


        verticalLayout->addLayout(horizontalLayout_3);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 581, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionExit);

        retranslateUi(MainWindow);
        QObject::connect(actionExit, SIGNAL(activated()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Interface 1", 0));
        label->setText(QApplication::translate("MainWindow", "Interface Name", 0));
        label_sent1->setText(QApplication::translate("MainWindow", "Packets Read: 0", 0));
        label_4->setText(QApplication::translate("MainWindow", "Filter Command", 0));
        lineEdit_macFilter1->setText(QString());
        label_dropped1->setText(QApplication::translate("MainWindow", "Packets Dropped: 0", 0));
        label_3->setText(QApplication::translate("MainWindow", "Packet Loss %", 0));
        label_bandwidth1->setText(QApplication::translate("MainWindow", "Actual Bandwith: 0 MB/S", 0));
        label_17->setText(QApplication::translate("MainWindow", "Bandwidth Limit (Mb/s)", 0));
        label_buffer1->setText(QApplication::translate("MainWindow", "Buffer Size: 0", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Interface 2", 0));
        label_2->setText(QApplication::translate("MainWindow", "Interface Name", 0));
        label_sent2->setText(QApplication::translate("MainWindow", "Packets Read: 0", 0));
        label_5->setText(QApplication::translate("MainWindow", "Filter Command", 0));
        lineEdit_macFilter2->setText(QString());
        label_dropped2->setText(QApplication::translate("MainWindow", "Packets Dropped: 0", 0));
        label_10->setText(QApplication::translate("MainWindow", "Packet Loss %", 0));
        label_bandwidth2->setText(QApplication::translate("MainWindow", "Actual Bandwith: 0 MB/S", 0));
        label_12->setText(QApplication::translate("MainWindow", "Bandwidth Limit (Mb/s)", 0));
        label_buffer2->setText(QApplication::translate("MainWindow", "Buffer Size: 0", 0));
        pushButton_refreshInterfaceList->setText(QApplication::translate("MainWindow", "Reload Interface List", 0));
        pushButton_start->setText(QApplication::translate("MainWindow", "Start", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
