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

#include "aceclassutility_class.h"
#include "ui_aceclassutility_class.h"

#include "../AceClassUtility_Attendances/aceclassutility_attendances.h"
#include "../AceClassUtility_Assignments/aceclassutility_assignments.h"
#include "../AceClassUtility_StudentList/aceclassutility_studentlist.h"

#include <QDir>

AceClassUtility_Class::AceClassUtility_Class(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AceClassUtility_Class)
{
    ui->setupUi(this);
}

AceClassUtility_Class::~AceClassUtility_Class()
{
    delete ui;
}

void AceClassUtility_Class::opened(QString className)
{
    AceClassUtility_Class::className = className;
    ui->classNameLabel->setText(AceClassUtility_Class::className);
    setWindowTitle("Ace Class Utility - " + AceClassUtility_Class::className);
}

void AceClassUtility_Class::dialogClosed()
{
    show();
}

void AceClassUtility_Class::on_backButton_released()
{
    emit backPressed();
    QDialog::reject();
}

void AceClassUtility_Class::on_attendanceButton_released()
{
    AceClassUtility_Attendances *attendances = new AceClassUtility_Attendances();
    attendances->show();

    QObject::connect(attendances, SIGNAL(finished(int)),
                     this, SLOT(dialogClosed()));
    attendances->opened(AceClassUtility_Class::className);
    hide();
}

void AceClassUtility_Class::on_assignmentsButton_released()
{
    AceClassUtility_Assignments *assignments = new AceClassUtility_Assignments();
    assignments->show();

    QObject::connect(assignments, SIGNAL(finished(int)),
                     this, SLOT(dialogClosed()));
    assignments->opened(AceClassUtility_Class::className);
    hide();
}

void AceClassUtility_Class::on_deleteClassButton_released()
{
    QDir d("AceClassUtility/" + AceClassUtility_Class::className);
    if (d.removeRecursively())
        QApplication::exit(0);
    else
        QApplication::exit(1);
}

void AceClassUtility_Class::on_studentListButton_released()
{
    AceClassUtility_StudentList *studentList = new AceClassUtility_StudentList();
    studentList->show();

    QObject::connect(studentList, SIGNAL(finished(int)),
                     this, SLOT(dialogClosed()));
    studentList->opened(AceClassUtility_Class::className);
    hide();
}
