#pragma once
#include <QMainWindow>
class JsonlNavigator;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private:

    JsonlNavigator* navigator;

    void loadJsonFile(const QString& path);
    void loadJsonlFile(const QString& path);
};