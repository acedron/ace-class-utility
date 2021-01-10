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
#ifndef ACECLASSUTILITY_TAKEATTENDANCE_H
#define ACECLASSUTILITY_TAKEATTENDANCE_H

#include <QDialog>
#include <QJsonArray>

namespace Ui {
class AceClassUtility_TakeAttendance;
}

class AceClassUtility_TakeAttendance : public QDialog
{
    Q_OBJECT

public:
    explicit AceClassUtility_TakeAttendance(QWidget *parent = nullptr);
    ~AceClassUtility_TakeAttendance();

signals:
    void attendanceTaken(QString filePath);

public slots:
    void opened(QString className);

private slots:
    void on_cancelButton_released();

    void on_confirmButton_released();

    void studentStateChanged(int state);

private:
    Ui::AceClassUtility_TakeAttendance *ui;
    QString className;
    QDateTime attendanceDateTime;
    QJsonArray students;
    QMap<QString, int> attendanceMap;
};

#endif // ACECLASSUTILITY_TAKEATTENDANCE_H
