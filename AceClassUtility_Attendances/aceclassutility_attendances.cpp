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

#include "aceclassutility_attendances.h"
#include "ui_aceclassutility_attendances.h"

#include "../AceClassUtility_Attendance/aceclassutility_attendance.h"
#include "../AceClassUtility_NewAttendance/aceclassutility_newattendance.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>

AceClassUtility_Attendances::AceClassUtility_Attendances(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AceClassUtility_Attendances)
{
    ui->setupUi(this);
}

AceClassUtility_Attendances::~AceClassUtility_Attendances()
{
    delete ui;
}

void AceClassUtility_Attendances::opened(QString className)
{
    AceClassUtility_Attendances::className = className;
    ui->classNameLabel->setText(AceClassUtility_Attendances::className + " - Attendances");
    setWindowTitle("Ace Class Utility - " + AceClassUtility_Attendances::className + " - Attendances");

    QGridLayout *layout = new QGridLayout();
    layout->setAlignment(Qt::AlignTop);
    ui->attendances->widget()->setLayout(layout);

    QDir d(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" +
           AceClassUtility_Attendances::className + "/attendances/");
    QStringList attendances = d.entryList(QDir::Files | QDir::NoDotAndDotDot, QDir::Time);
    for (int i = 0; i < attendances.size(); i++) {
        QFile f(d.filePath(attendances[i]));
        if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QFileInfo fileInfo(f.fileName());
            QString fname(fileInfo.baseName());

            QString jsonString = f.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
            QJsonObject obj = doc.object();
            QDateTime attendanceDateTime = QDateTime::fromString(fname, Qt::ISODate);
            QLocale locale = QLocale::system();

            QPushButton *button = new QPushButton();
            button->setText(attendanceDateTime.toString(locale.dateTimeFormat(QLocale::LongFormat)));
            button->setObjectName(d.filePath(attendances[i]));
            button->setFlat(true);
            button->setIcon(QIcon::fromTheme("contact"));
            button->setIconSize(QSize(48, 48));
            QFont font = button->font();
            font.setBold(true);
            button->setFont(font);
            ui->attendances->widget()->layout()->addWidget(button);
            QObject::connect(button, SIGNAL(released()),
                             this, SLOT(attendance_opened()));
            button->show();
            f.close();
        }
    }
}

void AceClassUtility_Attendances::dialog_closed()
{
    show();
}

void AceClassUtility_Attendances::attendance_opened(QString filePath)
{
    if (filePath.isEmpty()) {
        QPushButton *buttonSender = qobject_cast<QPushButton *>(sender());
        filePath = buttonSender->objectName();
    } else {
        QFile f(filePath);
        if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QFileInfo fileInfo(f.fileName());
            QString fname(fileInfo.baseName());
            QDateTime attendanceDateTime = QDateTime::fromString(fname, Qt::ISODate);
            QLocale locale = QLocale::system();

            QPushButton *button = new QPushButton();
            button->setText(attendanceDateTime.toString(locale.dateTimeFormat(QLocale::LongFormat)));
            button->setObjectName(filePath);
            button->setFlat(true);
            button->setIcon(QIcon::fromTheme("contact"));
            button->setIconSize(QSize(48, 48));
            QFont font = button->font();
            font.setBold(true);
            button->setFont(font);
            ui->attendances->widget()->layout()->addWidget(button);
            QObject::connect(button, SIGNAL(released()),
                             this, SLOT(attendance_opened()));
            button->show();
        }
    }

    AceClassUtility_Attendance *attendance = new AceClassUtility_Attendance();
    attendance->show();
    QObject::connect(attendance, SIGNAL(finished(int)),
                     this, SLOT(dialog_closed()));
    attendance->opened(AceClassUtility_Attendances::className, filePath);
    hide();
}

void AceClassUtility_Attendances::on_backButton_released()
{
    QDialog::reject();
}

void AceClassUtility_Attendances::on_newAttendanceButton_released()
{
    AceClassUtility_NewAttendance *newAttendance = new AceClassUtility_NewAttendance();
    newAttendance->show();
    QObject::connect(newAttendance, SIGNAL(rejected()),
                     this, SLOT(dialog_closed()));
    QObject::connect(newAttendance, SIGNAL(attendanceCreated(QString)),
                     this, SLOT(attendance_opened(QString)));
    newAttendance->opened(AceClassUtility_Attendances::className);
    hide();
}
