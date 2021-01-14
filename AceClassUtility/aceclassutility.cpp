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

#include "../AceClassUtility_NewClass/aceclassutility_newclass.h"
#include "../AceClassUtility_Class/aceclassutility_class.h"

#include <QFile>
#include <QDir>

AceClassUtility::AceClassUtility(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AceClassUtility)
{
    ui->setupUi(this);

    AceClassUtility::startLoading();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    ui->classes->widget()->setLayout(layout);

    if (!QDir("AceClassUtility").exists())
        QDir().mkdir("AceClassUtility");

    QDir d("AceClassUtility");
    if (d.isEmpty()) {
        AceClassUtility_NewClass *newClass = new AceClassUtility_NewClass();
        newClass->show();

        QObject::connect(newClass, SIGNAL(rejected()),
                         this, SLOT(discardExit()));
        QObject::connect(newClass, SIGNAL(accepted()),
                         this, SLOT(stopLoadingAndRegenerate()));
    } else
        AceClassUtility::stopLoadingAndRegenerate();
}

AceClassUtility::~AceClassUtility()
{
    delete ui;
}

void AceClassUtility::startLoading()
{
    ui->loading->show();
    ui->loading->setEnabled(true);
    ui->main->hide();
    ui->main->setEnabled(false);


}

void AceClassUtility::stopLoading()
{
    ui->loading->hide();
    ui->loading->setEnabled(false);
    ui->main->show();
    ui->main->setEnabled(true);
}

void AceClassUtility::stopLoadingAndRegenerate()
{
    AceClassUtility::stopLoading();

    QDir d("AceClassUtility");
    QStringList classes = d.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (int i = 0; i < classes.size(); i++) {
        QPushButton *button = new QPushButton();
        button->setText(classes[i]);
        button->setObjectName("classButton_" + QString::number(i));
        button->setFlat(true);
        button->setIcon(QIcon::fromTheme("accessories-text-editor"));
        QFont font = button->font();
        font.setBold(true);
        button->setFont(font);
        ui->classes->widget()->layout()->addWidget(button);
        button->show();
        QObject::connect(button, SIGNAL(released()),
                         this, SLOT(openClass()));
    }
}

void AceClassUtility::stopLoadingCreatedNewClass(QString newClassName)
{
    QDir d("AceClassUtility");
    QStringList classes = d.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    QPushButton *button = new QPushButton();
    button->setText(newClassName);
    button->setObjectName("classButton_" + QString::number(classes.size() - 1));
    button->setFlat(true);
    button->setIcon(QIcon(QString("accessories-text-editor")));
    QFont font = button->font();
    font.setBold(true);
    button->setFont(font);
    ui->classes->widget()->layout()->addWidget(button);
    button->show();
    QObject::connect(button, SIGNAL(released()),
                     this, SLOT(openClass()));

    AceClassUtility::stopLoading();
}

void AceClassUtility::openClass()
{
    QPushButton *buttonSender = qobject_cast<QPushButton *>(sender());
    QString className = buttonSender->text();

    AceClassUtility_Class *classWindow = new AceClassUtility_Class();
    classWindow->show();
    QObject::connect(classWindow, SIGNAL(backPressed()),
                     this, SLOT(closedClass()));
    classWindow->opened(className);
    hide();
}

void AceClassUtility::closedClass()
{
    show();
}

void AceClassUtility::discardExit()
{
    QDir d("AceClassUtility");

    if (d.removeRecursively())
        QApplication::exit(0);
    else
        QApplication::exit(1);
}

void AceClassUtility::on_newClassButton_released()
{
    AceClassUtility_NewClass *newClass = new AceClassUtility_NewClass();
    newClass->show();
    AceClassUtility::startLoading();

    QObject::connect(newClass, SIGNAL(rejected()),
                     this, SLOT(stopLoading()));
    QObject::connect(newClass, SIGNAL(createdClass(QString)),
                     this, SLOT(stopLoadingCreatedNewClass(QString)));
}
