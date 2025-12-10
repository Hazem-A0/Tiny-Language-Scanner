#include "inputwindow.h"
#include "./ui_inputwindow.h"

#include <QFileDialog> // for browse button
#include <QString>
#include <string>
#include <QFile> // to open file in the Qplain texet
#include <QTextStream>
#include <QPlainTextEdit>
#include <QMessageBox> // for massge box // msh m7taga 7aga y3ny (bsot 3adel emam)
#include <QTextCursor> //for highlight errors lines
#include <QTextCharFormat>
#include <QVector>
#include <QTextDocument>
#include <QTextBlock>
#include <QPixmap>   // For setting custom icon

InputWindow::InputWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::InputWindow)
{

    ui->setupUi(this);
    setWallpaper();
}

InputWindow::~InputWindow()
{
    delete ui;
}
void InputWindow::setWallpaper() {
    QPixmap pixmap("icons/i.jpg"); // Better use resource system
    QPalette palette = this->palette();

    palette.setBrush(QPalette::Window, pixmap.scaled(size(), Qt::IgnoreAspectRatio));
    setAutoFillBackground(true);
    setPalette(palette);
    qDebug() << QFile::exists(":/icons/i.jpg");
}

void browseAndLoadFile(QPlainTextEdit* plainTextEdit) {
    /*
     -function usage:  to open the explorer when BROWSE button is clicked
     -paramenters : only pass to it the textfield(GUI area) it will display file in
    */

    QString filter = "XML Files (*.xml);;Text Files (*.txt)"; // only can see XML and text files
    QString fileName = QFileDialog::getOpenFileName(
        nullptr,
        "Open File",
        QDir::homePath(),
        filter
        );

    if (fileName.isEmpty()) {
        return; // User cancelled
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Error", "Could not open the file.");
        return;
    }

    QTextStream in(&file);
    plainTextEdit->setPlainText(in.readAll());
    file.close();
}


void InputWindow::on_pushButton_clicked()
{
    browseAndLoadFile(ui->inputField);
}

