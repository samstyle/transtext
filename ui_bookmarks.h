/********************************************************************************
** Form generated from reading UI file 'bookmarks.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BOOKMARKS_H
#define UI_BOOKMARKS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Bookmarks
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *filter;
    QTableView *table;

    void setupUi(QDialog *Bookmarks)
    {
        if (Bookmarks->objectName().isEmpty())
            Bookmarks->setObjectName(QString::fromUtf8("Bookmarks"));
        Bookmarks->resize(368, 267);
        verticalLayout = new QVBoxLayout(Bookmarks);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        filter = new QLineEdit(Bookmarks);
        filter->setObjectName(QString::fromUtf8("filter"));

        verticalLayout->addWidget(filter);

        table = new QTableView(Bookmarks);
        table->setObjectName(QString::fromUtf8("table"));

        verticalLayout->addWidget(table);


        retranslateUi(Bookmarks);

        QMetaObject::connectSlotsByName(Bookmarks);
    } // setupUi

    void retranslateUi(QDialog *Bookmarks)
    {
        Bookmarks->setWindowTitle(QCoreApplication::translate("Bookmarks", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Bookmarks: public Ui_Bookmarks {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BOOKMARKS_H
