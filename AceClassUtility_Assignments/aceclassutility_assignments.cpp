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

#include "aceclassutility_assignments.h"
#include "ui_aceclassutility_assignments.h"

#include "../AceClassUtility_Assignment/aceclassutility_assignment.h"
#include "../AceClassUtility_NewAssignment/aceclassutility_newassignment.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>

AceClassUtility_Assignments::AceClassUtility_Assignments(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AceClassUtility_Assignments)
{
    ui->setupUi(this);
}

AceClassUtility_Assignments::~AceClassUtility_Assignments()
{
    delete ui;
}

void AceClassUtility_Assignments::opened(QString className)
{
    AceClassUtility_Assignments::className = className;
    ui->titleLabel->setText(AceClassUtility_Assignments::className + " - Assignments");
    setWindowTitle("Ace Class Utility - " + AceClassUtility_Assignments::className + " - Assignments");

    QGridLayout *layout = new QGridLayout();
    layout->setAlignment(Qt::AlignTop);
    ui->assignments->widget()->setLayout(layout);

    QDir d(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" +
           AceClassUtility_Assignments::className + "/assignments/");
    QStringList assignments = d.entryList(QDir::Files | QDir::NoDotAndDotDot, QDir::Time);
    for (int i = 0; i < assignments.size(); i++) {
        QFile f(d.filePath(assignments[i]));
        if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QFileInfo fileInfo(f.fileName());
            QString fname(fileInfo.baseName());

            QString jsonString = f.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
            QJsonObject obj = doc.object();
            QDateTime attendanceDateTime = QDateTime::fromString(obj.value("dueDate").toString(), Qt::ISODate);
            QLocale locale = QLocale::system();

            QPushButton *button = new QPushButton();
            button->setText(fname + " (" + attendanceDateTime.toString(locale.dateTimeFormat(QLocale::ShortFormat)) + ")");
            button->setObjectName(d.filePath(assignments[i]));
            button->setFlat(true);
            button->setIcon(QIcon::fromTheme("document"));
            button->setIconSize(QSize(48, 48));
            QFont font = button->font();
            font.setBold(true);
            button->setFont(font);
            ui->assignments->widget()->layout()->addWidget(button);
            QObject::connect(button, SIGNAL(released()),
                             this, SLOT(assignment_opened()));
            button->show();
            f.close();
        }
    }
}

void AceClassUtility_Assignments::dialog_closed()
{
    show();
}

void AceClassUtility_Assignments::assignment_opened(QString filePath)
{
    if (filePath.isEmpty()) {
        QPushButton *buttonSender = qobject_cast<QPushButton *>(sender());
        filePath = buttonSender->objectName();
    } else {
        QFile f(filePath);
        if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QFileInfo fileInfo(f.fileName());
            QString fname(fileInfo.baseName());

            QString jsonString = f.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
            QJsonObject obj = doc.object();
            QDateTime attendanceDateTime = QDateTime::fromString(obj.value("dueDate").toString(), Qt::ISODate);
            QLocale locale = QLocale::system();

            QPushButton *button = new QPushButton();
            button->setText(fname + " (" + attendanceDateTime.toString(locale.dateTimeFormat(QLocale::ShortFormat)) + ")");
            button->setObjectName(filePath);
            button->setFlat(true);
            button->setIcon(QIcon::fromTheme("document"));
            button->setIconSize(QSize(48, 48));
            QFont font = button->font();
            font.setBold(true);
            button->setFont(font);
            ui->assignments->widget()->layout()->addWidget(button);
            QObject::connect(button, SIGNAL(released()),
                             this, SLOT(assignment_opened()));
            button->show();
            f.close();
        }
    }

    AceClassUtility_Assignment *assignment = new AceClassUtility_Assignment();
    assignment->opened(AceClassUtility_Assignments::className, filePath);
    QObject::connect(assignment, SIGNAL(rejected()),
                     this, SLOT(dialog_closed()));
    QObject::connect(assignment, SIGNAL(assignmentDeleted(QString)),
                     this, SLOT(assignment_deleted(QString)));
    assignment->show();
    hide();
}

void AceClassUtility_Assignments::assignment_deleted(QString filePath)
{
    QPushButton *button = ui->assignments->widget()->findChild<QPushButton *>(filePath);
    button->deleteLater();
}

void AceClassUtility_Assignments::on_backButton_released()
{
    QDialog::reject();
}

void AceClassUtility_Assignments::on_newAssignmentButton_released()
{
    AceClassUtility_NewAssignment *newAssignment = new AceClassUtility_NewAssignment();
    newAssignment->opened(AceClassUtility_Assignments::className);
    QObject::connect(newAssignment, SIGNAL(rejected()),
                     this, SLOT(dialog_closed()));
    QObject::connect(newAssignment, SIGNAL(assignmentCreated(QString)),
                     this, SLOT(assignment_opened(QString)));
    newAssignment->show();
    hide();
}
