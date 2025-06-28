#include "MainWindow.h"
#include "FileOpenDialog.h"
#include "JsonViewerWidget.h"
#include <QMenuBar>
#include <QAction>
#include <QFileInfo>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    setWindowTitle("JsonWikiBrowser");
    resize(800, 600);

    viewer = new JsonViewerWidget(this);
    setCentralWidget(viewer);

    auto* fileMenu = menuBar()->addMenu("File");
    auto* openAct = new QAction("Open", this);
    connect(openAct, &QAction::triggered, this, [this]() {
        FileOpenDialog dlg;
        if (dlg.exec() == QDialog::Accepted) {
            QFileInfo info(dlg.selectedFile());
            if (info.suffix() == "json" && info.size() <= 40 * 1024 * 1024) {
                loadJsonFile(info.absoluteFilePath());
            } else {
                QMessageBox::warning(this, "Invalid File", "Only JSON files up to 40MB are supported here.");
            }
        }
    });
    fileMenu->addAction(openAct);
}

void MainWindow::loadJsonFile(const QString& path) {
    viewer->loadJson(path);
}