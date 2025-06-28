#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QTreeView>
#include <QStandardItemModel>

class JsonlNavigator : public QWidget {
    Q_OBJECT
public:
    JsonlNavigator(QWidget* parent = nullptr);
    void loadJsonlFile(const QString& path, bool enableIndexing);

private slots:
    void onSearch();

signals:
    void jsonSelected(const QString& jsonText);

private:
    QLineEdit* inputEdit;
    QComboBox* searchTypeCombo;
    QPushButton* searchButton;
    QLabel* statusLabel;
    QTreeView* treeView;
    QStandardItemModel* model;

    QVector<qint64> lineOffsets;
    QMap<QString, int> idToLine;
    QMap<QString, int> labelToLine;
    QFile file;

    void buildIndex(bool includeMaps);
    QString readLineAt(int lineNumber);
    void displayJson(const QString& jsonLine);
    void buildTree(const QJsonValue& value, QStandardItem* parent);
};