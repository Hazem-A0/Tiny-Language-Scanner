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
#include <QScrollArea>
#include <QDir>
#include <QDateTime>
#include <algorithm>
#include <sstream>

InputWindow::InputWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::InputWindow)
{
    ui->setupUi(this);
    setWallpaper();
    
    // Configure image label for proper scaling
    ui->imageLabel->setAlignment(Qt::AlignCenter);
    ui->imageLabel->setScaledContents(false);
}

InputWindow::~InputWindow()
{
    delete ui;
}
void InputWindow::setWallpaper() {
    // Try to load background from data folder first
    QString dataBackground = QDir::currentPath() + "/../../data/background.png";
    QPixmap pixmap;
    
    if (QFile::exists(dataBackground)) {
        pixmap.load(dataBackground);
    } else {
        // Fallback to icons folder
        pixmap.load("icons/i.jpg");
    }
    
    if (!pixmap.isNull()) {
        QPalette palette = this->palette();
        palette.setBrush(QPalette::Window, pixmap.scaled(size(), Qt::IgnoreAspectRatio));
        setAutoFillBackground(true);
        setPalette(palette);
    }
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

void InputWindow::on_pushButton_2_clicked()
{
    // Show Syntax Tree button clicked
    processInput();
}

void InputWindow::on_pushButton_3_clicked()
{
    // Save button clicked
    saveOutput();
}

void InputWindow::processInput()
{
    // Process as code
    scanCode();
    
    // Parse the tokens and display syntax tree
    if (!tokens.empty()) {
        parseTokens();
        displaySyntaxTree();
    }
}

void InputWindow::scanCode()
{
    // Get input text from the plain text edit
    QString inputText = ui->inputField->toPlainText();
    std::string code = inputText.toStdString();
    
    // Normalize newlines
    std::replace(code.begin(), code.end(), '\r', '\n');
    
    // Create scanner and scan the code
    Scanner scanner(code);
    tokens = scanner.scanAll();
    
    if (tokens.empty()) {
        QMessageBox::warning(this, "Scan Error", "No tokens found in the input code.");
    } else {
        // Display tokens in the UI
        displayTokensInUI();
        ui->statusbar->showMessage(QString("Successfully scanned %1 tokens.").arg(tokens.size()), 3000);
    }
}

void InputWindow::scanTokens()
{
    // Parse tokens from input field (format: token_value,token_type)
    QString inputText = ui->inputField->toPlainText();
    tokens.clear();
    
    QStringList lines = inputText.split('\n', Qt::SkipEmptyParts);
    
    for (const QString& line : lines) {
        QString trimmed = line.trimmed();
        if (trimmed.isEmpty()) continue;
        
        QStringList parts = trimmed.split(',');
        if (parts.size() >= 2) {
            std::string value = parts[0].trimmed().toStdString();
            std::string typeStr = parts[1].trimmed().toStdString();
            
            TokenType type = stringToTokenType(typeStr);
            tokens.push_back(Token(value, type));
        }
    }
    
    // Add EOF token if not present
    if (tokens.empty() || tokens.back().type != TokenType::END_OF_FILE) {
        tokens.push_back(Token("", TokenType::END_OF_FILE));
    }
    
    if (tokens.size() <= 1) {
        QMessageBox::warning(this, "Parse Error", "No valid tokens found. Format: value,TYPE");
    } else {
        // Display tokens in the UI
        displayTokensInUI();
        ui->statusbar->showMessage(QString("Successfully loaded %1 tokens.").arg(tokens.size() - 1), 3000);
    }
}

void InputWindow::displayTokensInUI()
{
    // Display tokens in the tokens tab
    QString tokenText;
    int count = 0;
    
    for (const auto& token : tokens) {
        if (token.type == TokenType::END_OF_FILE) continue;
        count++;
        tokenText += QString("%1. %2, %3\n")
            .arg(count)
            .arg(QString::fromStdString(token.value).isEmpty() ? "<empty>" : QString::fromStdString(token.value))
            .arg(QString::fromStdString(tokenTypeToString(token.type)));
    }
    
    ui->tokensOutput->setPlainText(tokenText);
    ui->tabWidget->setCurrentIndex(0); // Switch to tokens tab
}

void InputWindow::parseTokens()
{
    if (tokens.empty()) {
        QMessageBox::warning(this, "Parse Error", "No tokens to parse.");
        return;
    }
    
    TinyParser parser;
    TinyParser::ParseResult result = parser.parse(tokens);
    
    if (result.success) {
        syntaxTree = result.ast;
        ui->statusbar->showMessage("Successfully parsed the input!", 3000);
    } else {
        QString errorMsg = "Parse errors occurred:\n";
        for (const auto& error : result.errors) {
            errorMsg += QString::fromStdString(error) + "\n";
        }
        QMessageBox::critical(this, "Parse Error", errorMsg);
    }
}

void InputWindow::displaySyntaxTree()
{
    if (!syntaxTree) {
        QMessageBox::warning(this, "Display Error", "No syntax tree to display.");
        return;
    }
    
    // Generate DOT format and save to temporary file
    std::string dotContent = syntaxTree->toGraphViz();
    
    QString dotPath = QDir::temp().filePath("syntax_tree.dot");
    QFile dotFile(dotPath);
    if (!dotFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not create temporary DOT file.");
        return;
    }
    
    QTextStream out(&dotFile);
    out << QString::fromStdString(dotContent);
    dotFile.close();
    
    // Generate PNG to temporary location
    QString pngPath = QDir::temp().filePath("syntax_tree.png");
    
    QString command = QString("dot -Tpng \"%1\" -o \"%2\"").arg(dotPath).arg(pngPath);
    int result = system(command.toStdString().c_str());
    
    if (result == 0 && QFile::exists(pngPath)) {
        // Load and display the image at full size
        QPixmap pixmap(pngPath);
        if (!pixmap.isNull()) {
            // Display image at full size - scroll area will handle scrolling
            ui->imageLabel->setPixmap(pixmap);
            ui->imageLabel->adjustSize(); // Resize label to fit the full image
            ui->imageLabel->setScaledContents(false);
            currentImagePath = pngPath;
            ui->tabWidget->setCurrentIndex(1); // Switch to syntax tree tab
            ui->statusbar->showMessage("Syntax tree generated and displayed successfully! Use scroll to view full image.", 3000);
        } else {
            QMessageBox::warning(this, "Error", "Could not load the generated image.");
        }
    } else {
        QMessageBox::warning(this, "GraphViz Error", 
            "Could not generate PNG. Make sure GraphViz is installed.\n"
            "You can install it from: https://graphviz.org/download/\n\n"
            "DOT file saved at: " + dotPath);
        
        // Show DOT content as fallback
        ui->imageLabel->setText("GraphViz not found. Install GraphViz to view the syntax tree image.\n\nDOT file location: " + dotPath);
        ui->tabWidget->setCurrentIndex(1);
    }
}

void InputWindow::saveOutput()
{
    // Let user choose directory
    QString defaultPath = QDir::currentPath() + "/../../data";
    QString saveDirectory = QFileDialog::getExistingDirectory(
        this,
        "Choose Directory to Save Outputs",
        defaultPath,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    
    if (saveDirectory.isEmpty()) {
        return; // User cancelled
    }
    
    // Generate timestamp for unique filenames
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    
    // Save tokens to text file
    QString tokensFilePath = saveDirectory + "/tokens_" + timestamp + ".txt";
    QFile tokensFile(tokensFilePath);
    if (tokensFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&tokensFile);
        
        // Save tokens
        out << "=== TOKENS ===\n";
        for (const auto& token : tokens) {
            out << QString::fromStdString(token.value) << ", " 
                << QString::fromStdString(tokenTypeToString(token.type)) << "\n";
        }
        
        // Save syntax tree text representation
        if (syntaxTree) {
            out << "\n=== SYNTAX TREE ===\n";
            out << QString::fromStdString(syntaxTree->toString());
        }
        
        tokensFile.close();
    } else {
        QMessageBox::warning(this, "Error", "Could not save tokens file.");
        return;
    }
    
    // Save PNG image if it exists
    QString savedFiles = "Tokens saved to: " + tokensFilePath;
    
    if (!currentImagePath.isEmpty() && QFile::exists(currentImagePath)) {
        QString pngFilePath = saveDirectory + "/syntax_tree_" + timestamp + ".png";
        
        if (QFile::copy(currentImagePath, pngFilePath)) {
            savedFiles += "\n\nSyntax tree image saved to: " + pngFilePath;
        } else {
            // If copy fails, try to regenerate the PNG
            if (syntaxTree) {
                std::string dotContent = syntaxTree->toGraphViz();
                QString dotPath = saveDirectory + "/syntax_tree_" + timestamp + ".dot";
                
                QFile dotFile(dotPath);
                if (dotFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream out(&dotFile);
                    out << QString::fromStdString(dotContent);
                    dotFile.close();
                    
                    QString command = QString("dot -Tpng \"%1\" -o \"%2\"").arg(dotPath).arg(pngFilePath);
                    int result = system(command.toStdString().c_str());
                    
                    if (result == 0) {
                        savedFiles += "\n\nSyntax tree image saved to: " + pngFilePath;
                    }
                }
            }
        }
    }
    
    QMessageBox::information(this, "Success", savedFiles);
    ui->statusbar->showMessage("Files saved successfully!", 3000);
}

