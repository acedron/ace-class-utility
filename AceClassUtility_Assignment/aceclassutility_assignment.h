#ifndef ACECLASSUTILITY_ASSIGNMENT_H
#define ACECLASSUTILITY_ASSIGNMENT_H

#include <QDialog>

namespace Ui {
class AceClassUtility_Assignment;
}

class AceClassUtility_Assignment : public QDialog
{
    Q_OBJECT

public:
    explicit AceClassUtility_Assignment(QWidget *parent = nullptr);
    ~AceClassUtility_Assignment();

signals:
    void assignmentDeleted(QString filePath);

public slots:
    void opened(QString className, QString filePath);

private slots:
    void on_backButton_released();

    void on_pushButton_released();

private:
    Ui::AceClassUtility_Assignment *ui;
    QString className;
    QString filePath;
};

#endif // ACECLASSUTILITY_ASSIGNMENT_H
