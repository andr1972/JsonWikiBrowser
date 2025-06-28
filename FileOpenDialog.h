#pragma once
#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QString>

class FileOpenDialog : public QDialog {
    Q_OBJECT
public:
    FileOpenDialog(QWidget* parent = nullptr);
    QString selectedFile() const { return selectedFilePath; }

private:
    QListWidget* typeList;
    QListWidget* formatList;
    QPushButton* chooseFileBtn;
    QLabel* selectedFileLabel;
    QString selectedFilePath;
};
