#pragma once
#include <QMainWindow>
class JsonViewerWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private:
    JsonViewerWidget* viewer;
    void loadJsonFile(const QString& path);
};