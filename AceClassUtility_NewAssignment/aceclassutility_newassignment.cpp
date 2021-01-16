/****************************************************************************
** Ace Class Utility - A Basic Utility App for Classes.
** Copyright (C) 2021  acedron <acedrons@yahoo.co.jp>
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <https://www.gnu.org/licenses/>.
****************************************************************************/

#include "aceclassutility_newassignment.h"
#include "ui_aceclassutility_newassignment.h"

#include <QFile>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>

AceClassUtility_NewAssignment::AceClassUtility_NewAssignment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AceClassUtility_NewAssignment)
{
    ui->setupUi(this);
}

AceClassUtility_NewAssignment::~AceClassUtility_NewAssignment()
{
    delete ui;
}

void AceClassUtility_NewAssignment::opened(QString className)
{
    AceClassUtility_NewAssignment::className = className;
    ui->titleLabel->setText(AceClassUtility_NewAssignment::className + " - New Assignment");
    setWindowTitle("Ace Class Utility - " + AceClassUtility_NewAssignment::className + " - New Assignment");

    QLocale locale = QLocale::system();
    ui->assignmentDueEdit->setDisplayFormat(locale.dateTimeFormat(QLocale::LongFormat));
    ui->assignmentDueEdit->setDateTime(QDateTime::currentDateTime().addDays(1));
}

void AceClassUtility_NewAssignment::on_cancelButton_released()
{
    QDialog::reject();
}

void AceClassUtility_NewAssignment::on_createButton_released()
{
    QDialog::accept();

    QString assignmentName = ui->assignmentNameEdit->text();
    QDateTime assignmentDueDate = ui->assignmentDueEdit->dateTime();
    QString assignmentDesc = ui->assignmentDescEdit->toPlainText();

    if (assignmentName.isEmpty())
        ui->statusLabel->setText("Please type assignment name first!");
    else {
        QFile f(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" +
                AceClassUtility_NewAssignment::className + "/assignments/" + assignmentName + ".json");
        if (f.exists())
            ui->statusLabel->setText("Assignment already exists!");
        else if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QJsonObject obj;

            obj.insert("desc", QJsonValue(assignmentDesc));

            QJsonDocument doc(obj);
            QTextStream out(&f);
            out << doc.toJson();
            f.close();
            emit assignmentCreated(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" +
                                   AceClassUtility_NewAssignment::className + "/assignments/" + assignmentName + ".json");
            QDialog::accept();
        } else
            ui->statusLabel->setText("An error occured while creating assignment!");
    }
}
