QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    confirmpage.cpp \
    main.cpp \
    mainwindow.cpp \
    openprojectthread.cpp \
    pictureanimationwidget.cpp \
    picturebutton.cpp \
    pictureshow.cpp \
    picturestatebutton.cpp \
    previewlistitem.cpp \
    previewlistwidget.cpp \
    projectsetpage.cpp \
    projecttree.cpp \
    projecttreeitem.cpp \
    projecttreethread.cpp \
    projecttreewidget.cpp \
    removeprojectdialog.cpp \
    slideshowdialog.cpp \
    wizard.cpp

HEADERS += \
    confirmpage.h \
    const.h \
    mainwindow.h \
    openprojectthread.h \
    pictureanimationwidget.h \
    picturebutton.h \
    pictureshow.h \
    picturestatebutton.h \
    previewlistitem.h \
    previewlistwidget.h \
    projectsetpage.h \
    projecttree.h \
    projecttreeitem.h \
    projecttreethread.h \
    projecttreewidget.h \
    removeprojectdialog.h \
    slideshowdialog.h \
    wizard.h

FORMS += \
    confirmpage.ui \
    mainwindow.ui \
    pictureshow.ui \
    projectsetpage.ui \
    projecttree.ui \
    removeprojectdialog.ui \
    slideshowdialog.ui \
    wizard.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
