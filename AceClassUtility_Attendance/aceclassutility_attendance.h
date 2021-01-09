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
#ifndef ACECLASSUTILITY_ATTENDANCE_H
#define ACECLASSUTILITY_ATTENDANCE_H

#include <QDialog>

namespace Ui {
class AceClassUtility_Attendance;
}

class AceClassUtility_Attendance : public QDialog
{
    Q_OBJECT

public:
    explicit AceClassUtility_Attendance(QWidget *parent = nullptr);
    ~AceClassUtility_Attendance();
    QString className;

public slots:
    void opened(QString className);
    void dialogClosed();

private slots:
    void on_backButton_released();

    void on_studentListButton_released();

    void on_takeAttendanceButton_released();

private:
    Ui::AceClassUtility_Attendance *ui;
};

#endif // ACECLASSUTILITY_ATTENDANCE_H
