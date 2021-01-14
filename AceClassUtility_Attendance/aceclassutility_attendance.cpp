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

#include "aceclassutility_attendance.h"
#include "ui_aceclassutility_attendance.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>

AceClassUtility_Attendance::AceClassUtility_Attendance(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AceClassUtility_Attendance)
{
    ui->setupUi(this);
}

AceClassUtility_Attendance::~AceClassUtility_Attendance()
{
    delete ui;
}

void AceClassUtility_Attendance::opened(QString className, QString filePath)
{
    AceClassUtility_Attendance::className = className;
    AceClassUtility_Attendance::filePath = filePath;
    ui->titleLabel->setText(AceClassUtility_Attendance::className + " - Attendance");
    setWindowTitle("Ace Class Utility - " + AceClassUtility_Attendance::className + " - Attendance");

    QVBoxLayout *attendeeLayout = new QVBoxLayout();
    attendeeLayout->setAlignment(Qt::AlignTop);
    ui->attendees->widget()->setLayout(attendeeLayout);

    QVBoxLayout *absentLayout = new QVBoxLayout();
    absentLayout->setAlignment(Qt::AlignTop);
    ui->absents->widget()->setLayout(absentLayout);

    QFile f(AceClassUtility_Attendance::filePath);
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QFileInfo fileInfo(f.fileName());
        QString fname(fileInfo.baseName());
        QDateTime attendanceDateTime = QDateTime::fromString(fname, Qt::ISODate);
        QLocale locale = QLocale::system();
        ui->dateTimeLabel->setText(attendanceDateTime.toString(locale.dateTimeFormat(QLocale::ShortFormat)));

        int absentCount = 0;
        int attendeeCount = 0;

        QString jsonString = f.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
        QJsonObject obj = doc.object();
        QJsonObject attendance = obj.value("attendance").toObject();
        QList<QString> students = attendance.keys();
        for (int i = 0; i < students.size(); i++) {
            if (attendance.value(students[i]).toBool()) {
                attendeeCount += 1;
                QLabel *label = new QLabel();
                label->setText(students[i]);
                ui->attendees->widget()->layout()->addWidget(label);
            } else {
                absentCount += 1;
                QLabel *label = new QLabel();
                label->setText(students[i]);
                ui->absents->widget()->layout()->addWidget(label);
            }
        }

        ui->attendeeInfoButton->setText("Attendees: " + QString::number(attendeeCount));
        ui->absentInfoButton->setText("Absents: " + QString::number(absentCount));
        f.close();
    } else {
        ui->statusLabel->setText("An error occured while reading attendance!");
        ui->dateTimeLabel->setText("");
    }
}

void AceClassUtility_Attendance::on_backButton_released()
{
    QDialog::reject();
}
