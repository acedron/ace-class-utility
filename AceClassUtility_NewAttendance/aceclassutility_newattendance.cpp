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

#include "aceclassutility_newattendance.h"
#include "ui_aceclassutility_newattendance.h"

#include <QStandardPaths>
#include <QCheckBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

AceClassUtility_NewAttendance::AceClassUtility_NewAttendance(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AceClassUtility_NewAttendance)
{
    ui->setupUi(this);
}

AceClassUtility_NewAttendance::~AceClassUtility_NewAttendance()
{
    delete ui;
}

void AceClassUtility_NewAttendance::opened(QString className)
{
    AceClassUtility_NewAttendance::className = className;
    AceClassUtility_NewAttendance::attendanceDateTime = QDateTime::currentDateTime();
    ui->titleLabel->setText(AceClassUtility_NewAttendance::className + " - New Attendance ");
    setWindowTitle("Ace Class Utility - " + AceClassUtility_NewAttendance::className + " - New Attendance");

    QLocale locale = QLocale::system();
    ui->dateTimeLabel->setText(AceClassUtility_NewAttendance::attendanceDateTime.toString(locale.dateTimeFormat(QLocale::ShortFormat)));
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    ui->studentList->widget()->setLayout(layout);

    QFile f(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" +
            AceClassUtility_NewAttendance::className + "/studentList.json");
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString jsonString = f.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
        AceClassUtility_NewAttendance::students = doc.array();
        for (int i = 0; i < AceClassUtility_NewAttendance::students.size(); i++) {
            AceClassUtility_NewAttendance::attendanceMap[AceClassUtility_NewAttendance::students[i].toString()] = Qt::Unchecked;
            QCheckBox *checkBox = new QCheckBox();
            checkBox->setText(AceClassUtility_NewAttendance::students[i].toString());
            QObject::connect(checkBox, SIGNAL(stateChanged(int)),
                             this, SLOT(studentStateChanged(int)));
            ui->studentList->widget()->layout()->addWidget(checkBox);
        }
        f.close();
    } else
        AceClassUtility_NewAttendance::students = QJsonArray();
}

void AceClassUtility_NewAttendance::studentStateChanged(int state)
{
    QCheckBox *checkBoxSender = qobject_cast<QCheckBox *>(sender());
    QString studentName = checkBoxSender->text();

    AceClassUtility_NewAttendance::attendanceMap[studentName] = state;
}

void AceClassUtility_NewAttendance::on_cancelButton_released()
{
    QDialog::reject();
}

void AceClassUtility_NewAttendance::on_confirmButton_released()
{
    if (AceClassUtility_NewAttendance::students.isEmpty()) {
        ui->statusLabel->setText("Please create a student list first!");
    } else {
        QFile f(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" +
                AceClassUtility_NewAttendance::className + "/attendance/" +
                AceClassUtility_NewAttendance::attendanceDateTime.toString(Qt::ISODate) + ".json");
        if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QJsonObject attendance;

            QJsonObject students;
            int attendeeCount = 0;
            int absentCount = 0;
            QList<QString> keys = AceClassUtility_NewAttendance::attendanceMap.keys();
            for (int i = 0; i < AceClassUtility_NewAttendance::attendanceMap.size(); i++) {
                int val = AceClassUtility_NewAttendance::attendanceMap[keys[i]];
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
            emit attendanceTaken(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" +
                                 AceClassUtility_NewAttendance::className + "/attendances/" +
                                 AceClassUtility_NewAttendance::attendanceDateTime.toString(Qt::ISODate) + ".json");
            QDialog::accept();
        } else
            ui->statusLabel->setText("An error occured while saving attendance!");
    }
}
