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

#include "../AceClassUtility_StudentList/aceclassutility_studentlist.h"
#include "../AceClassUtility_TakeAttendance/aceclassutility_takeattendance.h"

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

void AceClassUtility_Attendance::opened(QString className)
{
    AceClassUtility_Attendance::className = className;
    ui->classNameLabel->setText(AceClassUtility_Attendance::className + " - Attendance");
    setWindowTitle("Ace Class Utility - " + AceClassUtility_Attendance::className + " - Attendance");
}

void AceClassUtility_Attendance::dialogClosed()
{
    show();
}

void AceClassUtility_Attendance::attendanceTaken(QString filePath)
{
    // Open attendance report for <filePath>.
}

void AceClassUtility_Attendance::on_backButton_released()
{
    QDialog::reject();
}

void AceClassUtility_Attendance::on_studentListButton_released()
{
    AceClassUtility_StudentList *studentList = new AceClassUtility_StudentList();
    studentList->show();
    QObject::connect(studentList, SIGNAL(finished(int)),
                     this, SLOT(dialogClosed()));
    studentList->opened(AceClassUtility_Attendance::className);
    hide();
}

void AceClassUtility_Attendance::on_takeAttendanceButton_released()
{
    AceClassUtility_TakeAttendance *takeAttendance = new AceClassUtility_TakeAttendance();
    takeAttendance->show();
    QObject::connect(takeAttendance, SIGNAL(rejected()),
                     this, SLOT(dialogClosed()));
    QObject::connect(takeAttendance, SIGNAL(attendanceTaken(QString)),
                     this, SLOT(attendanceTaken(QString)));
    takeAttendance->opened(AceClassUtility_Attendance::className);
    hide();
}
