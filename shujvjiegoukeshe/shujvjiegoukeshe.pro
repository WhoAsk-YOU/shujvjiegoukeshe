QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    choose_widget.cpp \
    destination_recommendation.cpp \
    diary_management.cpp \
    diary_read.cpp \
    diary_write.cpp \
    main.cpp \
    route_strategy.cpp \
    search_architect.cpp \
    start_widget.cpp

HEADERS += \
    Structure.h \
    choose_widget.h \
    destination_recommendation.h \
    diary_management.h \
    diary_read.h \
    diary_write.h \
    route_strategy.h \
    search_architect.h \
    start_widget.h

FORMS += \
    start_widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES +=
