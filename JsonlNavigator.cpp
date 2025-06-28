#include "JsonlNavigator.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>

JsonlNavigator::JsonlNavigator(QWidget* parent)
    : QWidget(parent), inputEdit(new QLineEdit(this)),
      searchTypeCombo(new QComboBox(this)), searchButton(new QPushButton("Goto", this)),
      statusLabel(new QLabel(this)), treeView(new QTreeView(this)),
      model(new QStandardItemModel(this)) {

    searchTypeCombo->addItems({"Line number", "ID", "Label"});

    auto* topLayout = new QHBoxLayout;
    topLayout->addWidget(searchTypeCombo);
    topLayout->addWidget(inputEdit);
    topLayout->addWidget(searchButton);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(treeView);

    treeView->setModel(model);
    model->setHorizontalHeaderLabels({"Key", "Value"});

    connect(searchButton, &QPushButton::clicked, this, &JsonlNavigator::onSearch);
}

void JsonlNavigator::loadJsonlFile(const QString& path, bool enableIndexing) {
    file.setFileName(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    lineOffsets.clear();
    idToLine.clear();
    labelToLine.clear();

    buildIndex(enableIndexing);
    file.seek(0);

    statusLabel->setText(QString("Indexed %1 lines").arg(lineOffsets.size()));
}

void JsonlNavigator::buildIndex(bool includeMaps) {
    file.seek(0); // reset to start
    lineOffsets.clear();
    idToLine.clear();
    labelToLine.clear();

    while (!file.atEnd()) {
        qint64 pos = file.pos();
        QByteArray line = file.readLine();
        lineOffsets.append(pos);

        if (includeMaps) {
            QJsonParseError err;
            QJsonDocument doc = QJsonDocument::fromJson(line, &err);
            if (!err.error && doc.isObject()) {
                QJsonObject obj = doc.object();
                if (obj.contains("id")) idToLine[obj["id"].toString()] = lineOffsets.size() - 1;
                if (obj.contains("labels") && obj["labels"].isObject()) {
                    QJsonObject labels = obj["labels"].toObject();
                    if (labels.contains("en")) {
                        labelToLine[labels["en"].toObject().value("value").toString()] = lineOffsets.size() - 1;
                    }
                }
            }
        }
    }
}

QString JsonlNavigator::readLineAt(int lineNumber) {
    if (lineNumber < 0 || lineNumber >= lineOffsets.size()) return {};
    qDebug() << "Seeking offset:" << lineOffsets[lineNumber];
    qDebug() << "Line read:" << file.readLine();
    file.seek(lineOffsets[lineNumber]);
    return file.readLine();
}

void JsonlNavigator::onSearch() {
    qDebug() << "onSearch triggered";
    int lineNumber = -1;
    QString key = inputEdit->text();
    QString type = searchTypeCombo->currentText();

    if (type == "Line number") {
        lineNumber = key.toInt();
    } else if (type == "ID") {
        lineNumber = idToLine.value(key, -1);
    } else if (type == "Label") {
        lineNumber = labelToLine.value(key, -1);
    }

    qDebug() << "Search type:" << searchTypeCombo->currentText();
    qDebug() << "Search key:" << inputEdit->text();
    qDebug() << "Line number resolved:" << lineNumber;
    if (lineNumber >= 0) {
        QString line = readLineAt(lineNumber);
        displayJson(line);
        emit jsonSelected(line);
    } else {
        QMessageBox::warning(this, "Not found", "No matching line found.");
    }
}

void JsonlNavigator::displayJson(const QString& jsonLine) {
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(jsonLine.toUtf8(), &err);
    if (err.error != QJsonParseError::NoError) return;

    model->clear();
    model->setHorizontalHeaderLabels({"Key", "Value"});
    QStandardItem* root = model->invisibleRootItem();
    buildTree(doc.isObject() ? QJsonValue(doc.object()) : QJsonValue(doc.array()), root);
}

void JsonlNavigator::buildTree(const QJsonValue& value, QStandardItem* parent) {
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
