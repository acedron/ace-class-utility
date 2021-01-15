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

#include "../AceClassUtility_NewAttendance/aceclassutility_newattendance.h"
#include "../AceClassUtility_Attendance/aceclassutility_attendance.h"

#include <QStandardPaths>
#include <QDir>
#include <QFile>
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
            button->setIcon(QIcon::fromTheme("document-info"));
            button->setIconSize(QSize(48, 48));
            QFont font = button->font();
            font.setBold(true);
            button->setFont(font);
            ui->attendances->widget()->layout()->addWidget(button);
            QObject::connect(button, SIGNAL(released()),
                             this, SLOT(attendanceOpened()));
            button->show();
            f.close();
        }
    }
}

void AceClassUtility_Attendances::dialogClosed()
{
    show();
}

void AceClassUtility_Attendances::attendanceTaken(QString filePath)
{
    AceClassUtility_Attendance *attendance = new AceClassUtility_Attendance();
    attendance->show();
    QObject::connect(attendance, SIGNAL(finished(int)),
                     this, SLOT(dialogClosed()));
    attendance->opened(AceClassUtility_Attendances::className, filePath);
}

void AceClassUtility_Attendances::attendanceOpened()
{
    QPushButton *buttonSender = qobject_cast<QPushButton *>(sender());
    QString filePath = buttonSender->objectName();

    AceClassUtility_Attendance *attendance = new AceClassUtility_Attendance();
    attendance->show();
    QObject::connect(attendance, SIGNAL(finished(int)),
                     this, SLOT(dialogClosed()));
    attendance->opened(AceClassUtility_Attendances::className, filePath);
    hide();
}

void AceClassUtility_Attendances::on_backButton_released()
{
    QDialog::reject();
}

void AceClassUtility_Attendances::on_takeAttendanceButton_released()
{
    AceClassUtility_NewAttendance *takeAttendance = new AceClassUtility_NewAttendance();
    takeAttendance->show();
    QObject::connect(takeAttendance, SIGNAL(rejected()),
                     this, SLOT(dialogClosed()));
    QObject::connect(takeAttendance, SIGNAL(attendanceTaken(QString)),
                     this, SLOT(attendanceTaken(QString)));
    takeAttendance->opened(AceClassUtility_Attendances::className);
    hide();
}
