#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class InputWindow;
}
QT_END_NAMESPACE

class InputWindow : public QMainWindow
{
    Q_OBJECT

public:
    InputWindow(QWidget *parent = nullptr);
    ~InputWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::InputWindow *ui;
    void setWallpaper(); // Function declaration for setting wallpape
};
#endif // INPUTWINDOW_H
