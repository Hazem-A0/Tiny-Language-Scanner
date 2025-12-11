#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#include <QMainWindow>
#include <memory>
#include <vector>
#include "include/TinyCommon.h"
#include "include/TinyScanner.h"
#include "include/TinyParser.h"

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
    void on_pushButton_clicked();        // Browse button
    void on_pushButton_2_clicked();      // Show Syntax Tree button
    void on_pushButton_3_clicked();      // Save button
    void on_radioButton_toggled(bool checked);   // Code radio button
    void on_radioButton_2_toggled(bool checked); // Tokens radio button

private:
    Ui::InputWindow *ui;
    void setWallpaper(); // Function declaration for setting wallpaper
    
    // Backend integration variables
    std::vector<Token> tokens;
    std::shared_ptr<ASTNode> syntaxTree;
    bool isCodeMode;  // true = code mode, false = tokens mode
    QString currentImagePath; // Store path to generated image
    
    // Helper methods
    void processInput();
    void scanCode();
    void scanTokens();
    void parseTokens();
    void displayTokensInUI();
    void displaySyntaxTree();
    void saveOutput();
};
#endif // INPUTWINDOW_H
