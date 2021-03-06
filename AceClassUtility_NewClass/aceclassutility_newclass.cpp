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

#include "aceclassutility_newclass.h"
#include "ui_aceclassutility_newclass.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>

AceClassUtility_NewClass::AceClassUtility_NewClass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AceClassUtility_NewClass)
{
    ui->setupUi(this);
}

AceClassUtility_NewClass::~AceClassUtility_NewClass()
{
    delete ui;
}

void AceClassUtility_NewClass::on_cancelButton_released()
{
    QDialog::reject();
}

void AceClassUtility_NewClass::on_confirmButton_released()
{
    QString className = ui->classNameEdit->text();
    if (className.isEmpty())
        ui->statusLabel->setText("Please enter a class name first!");
    else {
        QDir d(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
        if (d.exists(className + "/"))
            ui->statusLabel->setText("Class already exists!");
        else {
            d.mkdir(className);

            QDir dd(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" +
                    className);
            dd.mkdir("attendances");
            dd.mkdir("assignments");

            QFile f(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" +
                    className + "/class.json");
            if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QJsonObject obj;
                obj.insert("name", QJsonValue(className));

                QJsonDocument doc(obj);
                QTextStream out(&f);
                out << doc.toJson();
                f.close();

                emit createdClass(className);
                QDialog::accept();
            } else
                ui->statusLabel->setText("An error occured while creating class!");
        }
    }
}
