/********************************************************************************
** Form generated from reading UI file 'inputwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INPUTWINDOW_H
#define UI_INPUTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InputWindow
{
public:
    QWidget *centralwidget;
    QLabel *label_2;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPlainTextEdit *inputField;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QPushButton *pushButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *InputWindow)
    {
        if (InputWindow->objectName().isEmpty())
            InputWindow->setObjectName("InputWindow");
        InputWindow->resize(558, 600);
        centralwidget = new QWidget(InputWindow);
        centralwidget->setObjectName("centralwidget");
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(240, 80, 41, 20));
        QFont font;
        font.setBold(true);
        label_2->setFont(font);
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(200, 470, 131, 29));
        pushButton_3 = new QPushButton(centralwidget);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(450, 510, 83, 29));
        inputField = new QPlainTextEdit(centralwidget);
        inputField->setObjectName("inputField");
        inputField->setGeometry(QRect(90, 110, 341, 341));
        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(120, 30, 340, 41));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName("label");
        QFont font1;
        font1.setBold(true);
        font1.setItalic(true);
        label->setFont(font1);

        horizontalLayout->addWidget(label);

        pushButton = new QPushButton(horizontalLayoutWidget);
        pushButton->setObjectName("pushButton");

        horizontalLayout->addWidget(pushButton);

        InputWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(InputWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 558, 25));
        InputWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(InputWindow);
        statusbar->setObjectName("statusbar");
        InputWindow->setStatusBar(statusbar);

        retranslateUi(InputWindow);

        QMetaObject::connectSlotsByName(InputWindow);
    } // setupUi

    void retranslateUi(QMainWindow *InputWindow)
    {
        InputWindow->setWindowTitle(QCoreApplication::translate("InputWindow", "InputWindow", nullptr));
        label_2->setText(QCoreApplication::translate("InputWindow", "Input", nullptr));
        pushButton_2->setText(QCoreApplication::translate("InputWindow", "Show Sytax Tree", nullptr));
        pushButton_3->setText(QCoreApplication::translate("InputWindow", "Save", nullptr));
        label->setText(QCoreApplication::translate("InputWindow", "Type your file or browse your file  ", nullptr));
        pushButton->setText(QCoreApplication::translate("InputWindow", "Browse", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InputWindow: public Ui_InputWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INPUTWINDOW_H
