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

#include "aceclassutility_attendancereports.h"
#include "ui_aceclassutility_attendancereports.h"

#include "AceClassUtility_AttendanceReport/aceclassutility_attendancereport.h"

#include <QPushButton>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>

#include <QDebug>

AceClassUtility_AttendanceReports::AceClassUtility_AttendanceReports(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AceClassUtility_AttendanceReports)
{
    ui->setupUi(this);
}

AceClassUtility_AttendanceReports::~AceClassUtility_AttendanceReports()
{
    delete ui;
}

void AceClassUtility_AttendanceReports::opened(QString className)
{
    AceClassUtility_AttendanceReports::className = className;
    ui->titleLabel->setText(AceClassUtility_AttendanceReports::className + " - Attendance Reports");
    setWindowTitle("Ace Class Utility - " + AceClassUtility_AttendanceReports::className + " - Attendance Reports");

    QGridLayout *layout = new QGridLayout();
    layout->setAlignment(Qt::AlignTop);
    ui->attendanceReports->widget()->setLayout(layout);

    QDir d("AceClassUtility/" + AceClassUtility_AttendanceReports::className + "/attendance/");
    QStringList reports = d.entryList(QDir::Files | QDir::NoDotAndDotDot, QDir::Time);
    for (int i = 0; i < reports.size(); i++) {
        QFile f(d.filePath(reports[i]));
        QFileInfo fileInfo(f.fileName());
        QString fname(fileInfo.baseName());

        QDateTime attendanceDateTime = QDateTime::fromString(fname, Qt::ISODate);
        QLocale locale = QLocale::system();

        QPushButton *button = new QPushButton();
        button->setText(attendanceDateTime.toString(locale.dateTimeFormat(QLocale::ShortFormat)));
        button->setObjectName(f.fileName());
        button->setFlat(true);
        button->setIcon(QIcon::fromTheme("document-properties"));
        button->setIconSize(QSize(48, 48));
        QFont font = button->font();
        font.setBold(true);
        button->setFont(font);
        ui->attendanceReports->widget()->layout()->addWidget(button);
        button->show();
        QObject::connect(button, SIGNAL(released()),
                         this, SLOT(attendanceReportOpened()));
    }
}

void AceClassUtility_AttendanceReports::attendanceReportOpened()
{
    QPushButton *buttonSender = qobject_cast<QPushButton *>(sender());
    QString filePath = buttonSender->objectName();

    AceClassUtility_AttendanceReport *attendanceReport = new AceClassUtility_AttendanceReport();
    attendanceReport->show();
    QObject::connect(attendanceReport, SIGNAL(finished(int)),
                     this, SLOT(dialogClosed()));
    attendanceReport->opened(AceClassUtility_AttendanceReports::className, filePath);
    hide();
}

void AceClassUtility_AttendanceReports::dialogClosed()
{
    show();
}

void AceClassUtility_AttendanceReports::on_backButton_released()
{
    QDialog::reject();
}
