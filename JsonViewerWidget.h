#pragma once
#include <QWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include <QJsonDocument>
#include <QStandardItemModel>

class JsonViewerWidget : public QWidget {
    Q_OBJECT
public:
    JsonViewerWidget(QWidget* parent = nullptr);
    void loadJson(const QString& path);
    void loadJsonFromText(const QString &text);

private:
    QTreeView* treeView;
    QStandardItemModel* model;
    void buildTree(const QJsonValue& value, QStandardItem* parent);
};