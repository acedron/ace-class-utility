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
#ifndef ACECLASSUTILITY_H
#define ACECLASSUTILITY_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class AceClassUtility; }
QT_END_NAMESPACE

class AceClassUtility : public QMainWindow
{
    Q_OBJECT

public:
    AceClassUtility(QWidget *parent = nullptr);
    ~AceClassUtility();

private slots:
    void startLoading();

    void stopLoading();

    void stopLoadingAndRegenerate();

    void stopLoadingCreatedNewClass(QString newClassName);

    void openClass();

    void closedClass();

    void discardExit();

    void on_newClassButton_released();

private:
    Ui::AceClassUtility *ui;
};
#endif // ACECLASSUTILITY_H
