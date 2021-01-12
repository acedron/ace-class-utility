#include "aceclassutility_assignment.h"
#include "ui_aceclassutility_assignment.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>

AceClassUtility_Assignment::AceClassUtility_Assignment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AceClassUtility_Assignment)
{
    ui->setupUi(this);
}

AceClassUtility_Assignment::~AceClassUtility_Assignment()
{
    delete ui;
}

void AceClassUtility_Assignment::opened(QString className, QString filePath)
{
    AceClassUtility_Assignment::className = className;
    AceClassUtility_Assignment::filePath = filePath;
    ui->titleLabel->setText(AceClassUtility_Assignment::className + " - Assignment");
    setWindowTitle("Ace Class Utility - " + AceClassUtility_Assignment::className + " - Assignment");

    QFile f(filePath);
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QFileInfo fileInfo(f.fileName());
        QString assignmentName(fileInfo.baseName());
        ui->titleLabel->setText(AceClassUtility_Assignment::className + " - " + assignmentName);
        setWindowTitle("Ace Class Utility - " + AceClassUtility_Assignment::className + " - " + assignmentName);
        ui->assignmentName->setText(assignmentName);

        QString jsonString = f.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
        QJsonObject assignment = doc.object();
        ui->assignmentDesc->setText(assignment.value("desc").toString());

        QLocale locale = QLocale::system();
        QDateTime assignmentDateTime = QDateTime::fromString(assignment.value("dueDate").toString(), Qt::ISODate);
        ui->assignmentDateTime->setText("Due: " + assignmentDateTime.toString(locale.dateTimeFormat(QLocale::ShortFormat)));

        f.close();
    } else
        ui->statusLabel->setText("An error occured while reading assignment!");
}

void AceClassUtility_Assignment::on_backButton_released()
{
    QDialog::reject();
}

void AceClassUtility_Assignment::on_pushButton_released()
{
    QFile f(AceClassUtility_Assignment::filePath);
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        f.remove();
    }
    emit assignmentDeleted(AceClassUtility_Assignment::filePath);
    QDialog::reject();
}
