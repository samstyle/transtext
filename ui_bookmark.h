/********************************************************************************
** Form generated from reading UI file 'bookmark.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BOOKMARK_H
#define UI_BOOKMARK_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AddBookmark
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label_4;
    QPlainTextEdit *tedescr;
    QLabel *label_2;
    QLabel *label_5;
    QLineEdit *lepage;
    QLineEdit *lename;
    QLabel *label_3;
    QLineEdit *leline;
    QLineEdit *leid;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbAdd;

    void setupUi(QDialog *AddBookmark)
    {
        if (AddBookmark->objectName().isEmpty())
            AddBookmark->setObjectName(QString::fromUtf8("AddBookmark"));
        AddBookmark->resize(390, 290);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/bookmark.png"), QSize(), QIcon::Normal, QIcon::Off);
        AddBookmark->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(AddBookmark);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_4 = new QLabel(AddBookmark);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 0, 0, 1, 1);

        tedescr = new QPlainTextEdit(AddBookmark);
        tedescr->setObjectName(QString::fromUtf8("tedescr"));

        gridLayout->addWidget(tedescr, 4, 1, 1, 1);

        label_2 = new QLabel(AddBookmark);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 3, 0, 1, 1);

        label_5 = new QLabel(AddBookmark);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 1, 0, 1, 1);

        lepage = new QLineEdit(AddBookmark);
        lepage->setObjectName(QString::fromUtf8("lepage"));
        lepage->setEnabled(false);

        gridLayout->addWidget(lepage, 0, 1, 1, 1);

        lename = new QLineEdit(AddBookmark);
        lename->setObjectName(QString::fromUtf8("lename"));

        gridLayout->addWidget(lename, 3, 1, 1, 1);

        label_3 = new QLabel(AddBookmark);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout->addWidget(label_3, 4, 0, 1, 1);

        leline = new QLineEdit(AddBookmark);
        leline->setObjectName(QString::fromUtf8("leline"));
        leline->setEnabled(false);

        gridLayout->addWidget(leline, 1, 1, 1, 1);

        leid = new QLineEdit(AddBookmark);
        leid->setObjectName(QString::fromUtf8("leid"));
        leid->setEnabled(false);

        gridLayout->addWidget(leid, 2, 1, 1, 1);

        label = new QLabel(AddBookmark);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 2, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pbAdd = new QPushButton(AddBookmark);
        pbAdd->setObjectName(QString::fromUtf8("pbAdd"));
        pbAdd->setIcon(icon);

        horizontalLayout->addWidget(pbAdd);


        verticalLayout->addLayout(horizontalLayout);

        QWidget::setTabOrder(lepage, leline);
        QWidget::setTabOrder(leline, leid);
        QWidget::setTabOrder(leid, lename);
        QWidget::setTabOrder(lename, tedescr);
        QWidget::setTabOrder(tedescr, pbAdd);

        retranslateUi(AddBookmark);

        QMetaObject::connectSlotsByName(AddBookmark);
    } // setupUi

    void retranslateUi(QDialog *AddBookmark)
    {
        AddBookmark->setWindowTitle(QCoreApplication::translate("AddBookmark", "Add bookmark", nullptr));
        label_4->setText(QCoreApplication::translate("AddBookmark", "Page", nullptr));
        label_2->setText(QCoreApplication::translate("AddBookmark", "Name", nullptr));
        label_5->setText(QCoreApplication::translate("AddBookmark", "Line", nullptr));
        label_3->setText(QCoreApplication::translate("AddBookmark", "Descr", nullptr));
        label->setText(QCoreApplication::translate("AddBookmark", "id", nullptr));
        pbAdd->setText(QCoreApplication::translate("AddBookmark", "Add bookmark", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AddBookmark: public Ui_AddBookmark {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BOOKMARK_H
