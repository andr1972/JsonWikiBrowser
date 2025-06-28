#include "JsonViewerWidget.h"
#include <QVBoxLayout>
#include <QFile>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>

JsonViewerWidget::JsonViewerWidget(QWidget* parent)
    : QWidget(parent), treeView(new QTreeView(this)), model(new QStandardItemModel(this)) {
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(treeView);
    treeView->setModel(model);
    model->setHorizontalHeaderLabels({"Key", "Value"});
}

void JsonViewerWidget::loadJson(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &err);
    file.close();
    if (err.error != QJsonParseError::NoError) return;

    model->clear();
    model->setHorizontalHeaderLabels({"Key", "Value"});

    QStandardItem* root = model->invisibleRootItem();
    buildTree(doc.isObject() ? QJsonValue(doc.object()) : QJsonValue(doc.array()), root);
}

void JsonViewerWidget::buildTree(const QJsonValue& value, QStandardItem* parent) {
    if (value.isObject()) {
        auto obj = value.toObject();
        for (auto it = obj.begin(); it != obj.end(); ++it) {
            QStandardItem* keyItem = new QStandardItem(it.key());
            QStandardItem* valItem = new QStandardItem(it.value().isObject() || it.value().isArray() ? "" : it.value().toVariant().toString());
            parent->appendRow({keyItem, valItem});
            buildTree(it.value(), keyItem);
        }
    } else if (value.isArray()) {
        auto arr = value.toArray();
        for (int i = 0; i < arr.size(); ++i) {
            QString idx = QString("[%1]").arg(i);
            QStandardItem* keyItem = new QStandardItem(idx);
            QStandardItem* valItem = new QStandardItem(arr[i].isObject() || arr[i].isArray() ? "" : arr[i].toVariant().toString());
            parent->appendRow({keyItem, valItem});
            buildTree(arr[i], keyItem);
        }
    }
}

void JsonViewerWidget::loadJsonFromText(const QString& text) {
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8(), &err);
    if (err.error == QJsonParseError::NoError) {
        model->clear();
        model->setHorizontalHeaderLabels({ "Key", "Value" });
        QStandardItem* root = model->invisibleRootItem();
        buildTree(doc.isObject() ? QJsonValue(doc.object()) : QJsonValue(doc.array()), root);
    }
}