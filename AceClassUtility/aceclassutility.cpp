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

#include "aceclassutility.h"
#include "ui_aceclassutility.h"

#include "../AceClassUtility_Class/aceclassutility_class.h"
#include "../AceClassUtility_NewClass/aceclassutility_newclass.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>

AceClassUtility::AceClassUtility(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AceClassUtility)
{
    ui->setupUi(this);

    AceClassUtility::loading_start();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    ui->classes->widget()->setLayout(layout);

    if (!QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).exists())
        QDir().mkdir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));

    QDir d(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if (d.isEmpty()) {
        AceClassUtility_NewClass *newClass = new AceClassUtility_NewClass();
        QObject::connect(newClass, SIGNAL(finished(int)),
                         this, SLOT(loading_stop()));
        newClass->show();
    } else
        AceClassUtility::loading_stop();
}

AceClassUtility::~AceClassUtility()
{
    delete ui;
}

void AceClassUtility::loading_start()
{
    ui->loading->show();
    ui->loading->setEnabled(true);
    ui->main->hide();
    ui->main->setEnabled(false);
}

void AceClassUtility::loading_stop(QString newClassName)
{
    ui->loading->hide();
    ui->loading->setEnabled(false);
    ui->main->show();
    ui->main->setEnabled(true);

    QDir d(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    QStringList classes = d.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    if (newClassName.isEmpty())
        for (int i = 0; i < classes.size(); i++) {
            QPushButton *button = new QPushButton();
            QObject::connect(button, SIGNAL(released()),
                             this, SLOT(class_opened()));
            button->setText(classes[i]);
            button->setObjectName(classes[i]);
            button->setFlat(true);
            button->setIcon(QIcon::fromTheme("accessories-text-editor"));
            QFont font = button->font();
            font.setBold(true);
            button->setFont(font);
            ui->classes->widget()->layout()->addWidget(button);
            button->show();
        }
    else {
        QPushButton *button = new QPushButton();
        QObject::connect(button, SIGNAL(released()),
                         this, SLOT(class_opened()));
        button->setText(newClassName);
        button->setObjectName(newClassName);
        button->setFlat(true);
        button->setIcon(QIcon::fromTheme("accessories-text-editor"));
        QFont font = button->font();
        font.setBold(true);
        button->setFont(font);
        ui->classes->widget()->layout()->addWidget(button);
        button->show();
    }

}

void AceClassUtility::class_opened()
{
    QPushButton *buttonSender = qobject_cast<QPushButton *>(sender());
    QString className = buttonSender->text();

    AceClassUtility_Class *classDialog = new AceClassUtility_Class();
    QObject::connect(classDialog, SIGNAL(backPressed()),
                     this, SLOT(class_closed()));
    QObject::connect(classDialog, SIGNAL(classDeleted(QString)),
                     this, SLOT(class_deleted(QString)));
    classDialog->opened(className);
    classDialog->show();
    hide();
}

void AceClassUtility::class_deleted(QString className)
{
    AceClassUtility::class_closed();
    QPushButton *button = ui->classes->widget()->findChild<QPushButton *>(className);
    button->deleteLater();
}

void AceClassUtility::class_closed()
{
    show();
}

void AceClassUtility::on_newClassButton_released()
{
    AceClassUtility_NewClass *newClass = new AceClassUtility_NewClass();
    QObject::connect(newClass, SIGNAL(rejected()),
                     this, SLOT(loading_stop()));
    QObject::connect(newClass, SIGNAL(createdClass(QString)),
                     this, SLOT(loading_stop(QString)));
    newClass->show();
    AceClassUtility::loading_start();
}
