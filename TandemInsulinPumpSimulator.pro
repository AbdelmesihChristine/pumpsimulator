QT       += core gui widgets charts

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AlertDialog.cpp \
    BolusDeliveryWidget.cpp \
    BolusDialog.cpp \
    BolusHistoryManager.cpp \
    BolusSafetyManager.cpp \
    CGMGraphWidget.cpp \
    CgmSimulator.cpp \
    HistoryDialog.cpp \
    MainWindow.cpp \
    ProfileDialog.cpp \
    PumpController.cpp \
    UserProfileManager.cpp \
    main.cpp

HEADERS += \
    AlertDialog.h \
    BolusDeliveryWidget.h \
    BolusDialog.h \
    BolusHistoryManager.h \
    BolusRecord.h \
    BolusSafetyManager.h \
    CGMGraphWidget.h \
    CgmSimulator.h \
    HistoryDialog.h \
    MainWindow.h \
    ProfileDialog.h \
    PumpController.h \
    UserProfile.h \
    UserProfileManager.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
