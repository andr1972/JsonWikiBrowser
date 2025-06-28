#include "FileOpenDialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QFileInfo>

FileOpenDialog::FileOpenDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("Select Data Type and Format");
    resize(500, 300);

    typeList = new QListWidget(this);
    formatList = new QListWidget(this);
    typeList->addItems({"Plain JSON", "Wikidata"});
    formatList->addItems({".json", ".jsonl", ".xml.bz2"});

    chooseFileBtn = new QPushButton("Choose File", this);
    selectedFileLabel = new QLabel("No file selected.", this);

    auto* listsLayout = new QHBoxLayout;
    listsLayout->addWidget(typeList);
    listsLayout->addWidget(formatList);

    auto* bottomLayout = new QVBoxLayout;
    bottomLayout->addLayout(listsLayout);
    bottomLayout->addWidget(chooseFileBtn);
    bottomLayout->addWidget(selectedFileLabel);

    setLayout(bottomLayout);

    connect(chooseFileBtn, &QPushButton::clicked, this, [this]() {
        QString fileName = QFileDialog::getOpenFileName(this, "Select File");
        if (!fileName.isEmpty()) {
            QFileInfo info(fileName);
            if (info.suffix() == "json" && info.size() > 40 * 1024 * 1024) {
                selectedFileLabel->setText("File too large (limit 40MB for JSON).");
            } else {
                selectedFileLabel->setText(fileName);
            }
        }
    });
}