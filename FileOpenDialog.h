#pragma once
#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

class FileOpenDialog : public QDialog {
    Q_OBJECT
public:
    FileOpenDialog(QWidget* parent = nullptr);

private:
    QListWidget* typeList;
    QListWidget* formatList;
    QPushButton* chooseFileBtn;
    QLabel* selectedFileLabel;
};
