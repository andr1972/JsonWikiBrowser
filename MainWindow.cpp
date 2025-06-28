#include "MainWindow.h"
#include "FileOpenDialog.h"
#include "JsonlNavigator.h"
#include <QMenuBar>
#include <QAction>
#include <QFileInfo>
#include <QMessageBox>
#include <QFile>
#include <QStackedWidget>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    setWindowTitle("JsonWikiBrowser");
    resize(800, 600);

    navigator = new JsonlNavigator(this);
    setCentralWidget(navigator); // default

    auto* fileMenu = menuBar()->addMenu("File");
    auto* openAct = new QAction("Open", this);
    connect(openAct, &QAction::triggered, this, [this]() {
        FileOpenDialog dlg;
        if (dlg.exec() == QDialog::Accepted) {
            QString path = dlg.selectedFile();
            QFileInfo info(path);
            QString suffix = info.suffix().toLower();

            if (suffix == "json") {
                if (info.size() <= 40 * 1024 * 1024) {
                    loadJsonFile(path);
                } else {
                    QMessageBox::warning(this, "Too large", "JSON file exceeds 40MB limit.");
                }
            } else if (suffix == "jsonl") {
                loadJsonlFile(path);
            } else {
                QMessageBox::warning(this, "Unsupported", "Only .json and .jsonl files are currently supported.");
            }
        }
    });
    fileMenu->addAction(openAct);
}

void MainWindow::loadJsonFile(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QByteArray data = file.readAll();
    file.close();
    navigator->displayJson(QString::fromUtf8(data));
    setCentralWidget(navigator);
}

void MainWindow::loadJsonlFile(const QString& path) {
    navigator->setFocusPolicy(Qt::StrongFocus);
    connect(navigator, &JsonlNavigator::jsonSelected,
            navigator, &JsonlNavigator::displayJson);
    navigator->loadJsonlFile(path, true);
    navigator->setFocus();

}
