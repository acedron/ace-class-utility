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

#include "aceclassutility_takeattendance.h"
#include "ui_aceclassutility_takeattendance.h"

#include <QCheckBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>

AceClassUtility_TakeAttendance::AceClassUtility_TakeAttendance(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AceClassUtility_TakeAttendance)
{
    ui->setupUi(this);
}

AceClassUtility_TakeAttendance::~AceClassUtility_TakeAttendance()
{
    delete ui;
}

void AceClassUtility_TakeAttendance::opened(QString className)
{
    AceClassUtility_TakeAttendance::className = className;
    AceClassUtility_TakeAttendance::attendanceTime = QTime::currentTime();
    ui->titleLabel->setText(className + " - Take Attendance ");
    setWindowTitle("Ace Class Utility - " + className + " - Take Attendance");


    QVBoxLayout *layout = new QVBoxLayout();
    ui->studentList->widget()->setLayout(layout);

    QFile f("AceClassUtility/" + className + "/studentList.json");
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString jsonString = f.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
        QJsonArray studentArray = doc.array();
        for (int i = 0; i < studentArray.size(); i++) {
            QCheckBox *checkBox = new QCheckBox();
            checkBox->setText(studentArray[i].toString());
            ui->studentList->widget()->layout()->addWidget(checkBox);
        }
    }
}

void AceClassUtility_TakeAttendance::on_cancelButton_released()
{
    QDialog::reject();
}

void AceClassUtility_TakeAttendance::on_confirmButton_released()
{
    QDialog::accept();
}
