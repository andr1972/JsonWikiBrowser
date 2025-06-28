#include "MainWindow.h"
#include "FileOpenDialog.h"
#include <QMenuBar>
#include <QAction>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    setWindowTitle("JsonWikiBrowser");
    resize(800, 600);

    auto* fileMenu = menuBar()->addMenu("File");
    auto* openAct = new QAction("Open", this);
    connect(openAct, &QAction::triggered, this, []() {
        FileOpenDialog dlg;
        dlg.exec();
    });
    fileMenu->addAction(openAct);
}