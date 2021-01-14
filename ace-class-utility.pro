QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 static

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AceClassUtility_Assignment/aceclassutility_assignment.cpp \
    AceClassUtility_Assignments/aceclassutility_assignments.cpp \
    AceClassUtility_Attendances/aceclassutility_attendances.cpp \
    AceClassUtility_Attendance/aceclassutility_attendance.cpp \
    AceClassUtility_Class/aceclassutility_class.cpp \
    AceClassUtility_CreateAssignment/aceclassutility_createassignment.cpp \
    AceClassUtility_CreateClass/aceclassutility_createclass.cpp \
    AceClassUtility_StudentList/aceclassutility_studentlist.cpp \
    AceClassUtility_TakeAttendance/aceclassutility_takeattendance.cpp \
    main.cpp \
    AceClassUtility/aceclassutility.cpp

HEADERS += \
    AceClassUtility/aceclassutility.h \
    AceClassUtility_Assignment/aceclassutility_assignment.h \
    AceClassUtility_Assignments/aceclassutility_assignments.h \
    AceClassUtility_Attendances/aceclassutility_attendances.h \
    AceClassUtility_Attendance/aceclassutility_attendance.h \
    AceClassUtility_Class/aceclassutility_class.h \
    AceClassUtility_CreateAssignment/aceclassutility_createassignment.h \
    AceClassUtility_CreateClass/aceclassutility_createclass.h \
    AceClassUtility_StudentList/aceclassutility_studentlist.h \
    AceClassUtility_TakeAttendance/aceclassutility_takeattendance.h

FORMS += \
    AceClassUtility/aceclassutility.ui \
    AceClassUtility_Assignment/aceclassutility_assignment.ui \
    AceClassUtility_Assignments/aceclassutility_assignments.ui \
    AceClassUtility_Attendances/aceclassutility_attendances.ui \
    AceClassUtility_Attendance/aceclassutility_attendance.ui \
    AceClassUtility_Class/aceclassutility_class.ui \
    AceClassUtility_CreateAssignment/aceclassutility_createassignment.ui \
    AceClassUtility_CreateClass/aceclassutility_createclass.ui \
    AceClassUtility_StudentList/aceclassutility_studentlist.ui \
    AceClassUtility_TakeAttendance/aceclassutility_takeattendance.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    main.qrc
