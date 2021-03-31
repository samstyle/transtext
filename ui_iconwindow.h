/********************************************************************************
** Form generated from reading UI file 'iconwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ICONWINDOW_H
#define UI_ICONWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_IconWin
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QToolButton *tbAdd;
    QSpacerItem *horizontalSpacer;
    QToolButton *tbDel;
    QListWidget *list;

    void setupUi(QDialog *IconWin)
    {
        if (IconWin->objectName().isEmpty())
            IconWin->setObjectName(QString::fromUtf8("IconWin"));
        IconWin->setWindowModality(Qt::ApplicationModal);
        IconWin->resize(400, 300);
        IconWin->setModal(true);
        verticalLayout = new QVBoxLayout(IconWin);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tbAdd = new QToolButton(IconWin);
        tbAdd->setObjectName(QString::fromUtf8("tbAdd"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbAdd->setIcon(icon);

        horizontalLayout->addWidget(tbAdd);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        tbDel = new QToolButton(IconWin);
        tbDel->setObjectName(QString::fromUtf8("tbDel"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbDel->setIcon(icon1);

        horizontalLayout->addWidget(tbDel);


        verticalLayout->addLayout(horizontalLayout);

        list = new QListWidget(IconWin);
        list->setObjectName(QString::fromUtf8("list"));
        list->setEditTriggers(QAbstractItemView::AnyKeyPressed|QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);
        list->setIconSize(QSize(32, 32));
        list->setGridSize(QSize(45, 45));
        list->setViewMode(QListView::IconMode);
        list->setWordWrap(true);

        verticalLayout->addWidget(list);


        retranslateUi(IconWin);

        QMetaObject::connectSlotsByName(IconWin);
    } // setupUi

    void retranslateUi(QDialog *IconWin)
    {
        IconWin->setWindowTitle(QCoreApplication::translate("IconWin", "Icons pool", nullptr));
        tbAdd->setText(QCoreApplication::translate("IconWin", "Add", nullptr));
        tbDel->setText(QCoreApplication::translate("IconWin", "Del", nullptr));
    } // retranslateUi

};

namespace Ui {
    class IconWin: public Ui_IconWin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ICONWINDOW_H
