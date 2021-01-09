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

#include "aceclassutility_studentlist.h"
#include "ui_aceclassutility_studentlist.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>

AceClassUtility_StudentList::AceClassUtility_StudentList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AceClassUtility_StudentList)
{
    ui->setupUi(this);
}

AceClassUtility_StudentList::~AceClassUtility_StudentList()
{
    delete ui;
}

void AceClassUtility_StudentList::opened(QString className)
{
    AceClassUtility_StudentList::className = className;
    ui->titleLabel->setText(className + " - Student List");
    setWindowTitle("Ace Class Utility - " + className + " - Student List");

    QFile f("AceClassUtility/" + className + "/studentList.json");
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString jsonString = f.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
        QJsonArray studentArray = doc.array();
        QString plainStudentList = "";
        for (int i = 0; i < studentArray.size(); i++) {
            plainStudentList.append(studentArray[i].toString() + "\n");
        }
        ui->studentListEdit->setPlainText(plainStudentList);
    }
}

void AceClassUtility_StudentList::on_cancelButton_released()
{
    QDialog::reject();
}

void AceClassUtility_StudentList::on_saveButton_released()
{
    QString plainStudentList = ui->studentListEdit->toPlainText();
    QStringList students = plainStudentList.split("\n", Qt::SkipEmptyParts);

    QJsonDocument doc;
    QJsonArray studentArray = QJsonArray::fromStringList(students);
    doc.setArray(studentArray);

    QFile f("AceClassUtility/" + AceClassUtility_StudentList::className + "/studentList.json");
    if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&f);
        out << doc.toJson();
        hide();
    } else {
        ui->statusLabel->setText("An error occured while saving student list!");
    }

    QDialog::accept();
}
