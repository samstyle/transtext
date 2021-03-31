/********************************************************************************
** Form generated from reading UI file 'replacedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPLACEDIALOG_H
#define UI_REPLACEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Replacer
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *leStrF;
    QLabel *label_2;
    QLineEdit *leStrR;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbOk;
    QPushButton *pbCancel;

    void setupUi(QDialog *Replacer)
    {
        if (Replacer->objectName().isEmpty())
            Replacer->setObjectName(QString::fromUtf8("Replacer"));
        Replacer->setWindowModality(Qt::ApplicationModal);
        Replacer->resize(332, 98);
        Replacer->setModal(true);
        verticalLayout = new QVBoxLayout(Replacer);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(Replacer);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        leStrF = new QLineEdit(Replacer);
        leStrF->setObjectName(QString::fromUtf8("leStrF"));

        gridLayout->addWidget(leStrF, 0, 1, 1, 1);

        label_2 = new QLabel(Replacer);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        leStrR = new QLineEdit(Replacer);
        leStrR->setObjectName(QString::fromUtf8("leStrR"));

        gridLayout->addWidget(leStrR, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pbOk = new QPushButton(Replacer);
        pbOk->setObjectName(QString::fromUtf8("pbOk"));

        horizontalLayout->addWidget(pbOk);

        pbCancel = new QPushButton(Replacer);
        pbCancel->setObjectName(QString::fromUtf8("pbCancel"));

        horizontalLayout->addWidget(pbCancel);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Replacer);

        QMetaObject::connectSlotsByName(Replacer);
    } // setupUi

    void retranslateUi(QDialog *Replacer)
    {
        Replacer->setWindowTitle(QCoreApplication::translate("Replacer", "Replace", nullptr));
        label->setText(QCoreApplication::translate("Replacer", "Find", nullptr));
        label_2->setText(QCoreApplication::translate("Replacer", "Replace", nullptr));
        pbOk->setText(QCoreApplication::translate("Replacer", "Replace", nullptr));
        pbCancel->setText(QCoreApplication::translate("Replacer", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Replacer: public Ui_Replacer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPLACEDIALOG_H
