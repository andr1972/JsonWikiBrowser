#include <QMainWindow>
#include <QSplitter>
class JsonViewerWidget;
class JsonlNavigator;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private:
    JsonViewerWidget* viewer;
    JsonlNavigator* navigator;
    QSplitter* splitter;
    void loadJsonFile(const QString& path);
    void loadJsonlFile(const QString& path);
    void updateCentralWidget(QWidget* widget);
};