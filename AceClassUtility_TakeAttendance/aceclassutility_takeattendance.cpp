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
#include <QJsonObject>
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
    AceClassUtility_TakeAttendance::attendanceDateTime = QDateTime::currentDateTime();
    ui->titleLabel->setText(AceClassUtility_TakeAttendance::className + " - Take Attendance ");
    setWindowTitle("Ace Class Utility - " + AceClassUtility_TakeAttendance::className + " - Take Attendance");

    QLocale locale = QLocale::system();
    ui->dateTimeLabel->setText(AceClassUtility_TakeAttendance::attendanceDateTime.toString(locale.dateTimeFormat(QLocale::ShortFormat)));
    QVBoxLayout *layout = new QVBoxLayout();
    ui->studentList->widget()->setLayout(layout);

    QFile f("AceClassUtility/" + AceClassUtility_TakeAttendance::className + "/studentList.json");
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString jsonString = f.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
        AceClassUtility_TakeAttendance::students = doc.array();
        for (int i = 0; i < AceClassUtility_TakeAttendance::students.size(); i++) {
            AceClassUtility_TakeAttendance::attendanceMap[AceClassUtility_TakeAttendance::students[i].toString()] = Qt::Unchecked;
            QCheckBox *checkBox = new QCheckBox();
            checkBox->setText(AceClassUtility_TakeAttendance::students[i].toString());
            QObject::connect(checkBox, SIGNAL(stateChanged(int)),
                             this, SLOT(studentStateChanged(int)));
            ui->studentList->widget()->layout()->addWidget(checkBox);
        }
        f.close();
    } else
        AceClassUtility_TakeAttendance::students = QJsonArray();
}

void AceClassUtility_TakeAttendance::studentStateChanged(int state)
{
    QCheckBox *checkBoxSender = qobject_cast<QCheckBox *>(sender());
    QString studentName = checkBoxSender->text();

    AceClassUtility_TakeAttendance::attendanceMap[studentName] = state;
}

void AceClassUtility_TakeAttendance::on_cancelButton_released()
{
    QDialog::reject();
}

void AceClassUtility_TakeAttendance::on_confirmButton_released()
{
    if (AceClassUtility_TakeAttendance::students.isEmpty()) {
        ui->statusLabel->setText("Please create a student list first!");
    } else {
        QFile f("AceClassUtility/" + AceClassUtility_TakeAttendance::className + "/attendance/" +
                AceClassUtility_TakeAttendance::attendanceDateTime.toString(Qt::ISODate) + ".json");
        if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QJsonObject attendance;

            QJsonObject students;
            int attendeeCount = 0;
            int absentCount = 0;
            QList<QString> keys = AceClassUtility_TakeAttendance::attendanceMap.keys();
            for (int i = 0; i < AceClassUtility_TakeAttendance::attendanceMap.size(); i++) {
                int val = AceClassUtility_TakeAttendance::attendanceMap[keys[i]];
                if (val >= 1) {
                    attendeeCount += 1;
                    students.insert(keys[i], QJsonValue(true));
                }
                else {
                    absentCount += 1;
                    students.insert(keys[i], QJsonValue(false));
                }
            }
            attendance.insert("attendance", QJsonValue(students));
            attendance.insert("attendeeCount", QJsonValue(attendeeCount));
            attendance.insert("absentCount", QJsonValue(absentCount));

            QJsonDocument doc(attendance);
            QTextStream out(&f);
            out << doc.toJson();
            f.close();
            emit attendanceTaken("AceClassUtility/" + AceClassUtility_TakeAttendance::className + "/attendance/" +
                                 AceClassUtility_TakeAttendance::attendanceDateTime.toString(Qt::ISODate) + ".json");
            QDialog::accept();
        } else
            ui->statusLabel->setText("An error occured while saving attendance!");
    }
}
