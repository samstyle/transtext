/********************************************************************************
** Form generated from reading UI file 'mainwin.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWIN_H
#define UI_MAINWIN_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWin
{
public:
    QAction *actOpen;
    QAction *actNewProj;
    QAction *actNewPage;
    QAction *actOpenSrc;
    QAction *actSaveAs;
    QAction *actDelRows;
    QAction *actSave;
    QAction *actDelPage;
    QAction *actNewDir;
    QAction *actSplit;
    QAction *actSaveSrc;
    QAction *actSort;
    QAction *actGrabCbrd;
    QAction *actInsertSrc;
    QAction *actFindUntrn;
    QAction *actMerge;
    QAction *actClearTrn;
    QAction *actMergePage;
    QAction *actIcon;
    QAction *actSplitLine;
    QAction *actSplitName;
    QAction *actAddBookmark;
    QAction *actRmBookmark;
    QAction *actJoinLine;
    QAction *actSaveBranch;
    QAction *actionList;
    QAction *actBookmarks;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_2;
    QTreeWidget *tree;
    QWidget *tabs;
    QVBoxLayout *verticalLayout;
    QGridLayout *editGrid;
    QLineEdit *trnline;
    QLineEdit *srcline;
    QLineEdit *srcname;
    QLineEdit *trnname;
    QTableView *table;
    QWidget *widFind;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLineEdit *leFind;
    QHBoxLayout *horizontalLayout;
    QToolButton *tbScroll;
    QSpacerItem *horizontalSpacer_2;
    QLabel *labInfo;
    QProgressBar *progress;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuPage;
    QMenu *menuOptions;

    void setupUi(QMainWindow *MainWin)
    {
        if (MainWin->objectName().isEmpty())
            MainWin->setObjectName(QString::fromUtf8("MainWin"));
        MainWin->resize(860, 386);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Japan.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWin->setWindowIcon(icon);
        actOpen = new QAction(MainWin);
        actOpen->setObjectName(QString::fromUtf8("actOpen"));
        actNewProj = new QAction(MainWin);
        actNewProj->setObjectName(QString::fromUtf8("actNewProj"));
        actNewPage = new QAction(MainWin);
        actNewPage->setObjectName(QString::fromUtf8("actNewPage"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/newpage.png"), QSize(), QIcon::Normal, QIcon::Off);
        actNewPage->setIcon(icon1);
        actOpenSrc = new QAction(MainWin);
        actOpenSrc->setObjectName(QString::fromUtf8("actOpenSrc"));
        actSaveAs = new QAction(MainWin);
        actSaveAs->setObjectName(QString::fromUtf8("actSaveAs"));
        actDelRows = new QAction(MainWin);
        actDelRows->setObjectName(QString::fromUtf8("actDelRows"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        actDelRows->setIcon(icon2);
        actSave = new QAction(MainWin);
        actSave->setObjectName(QString::fromUtf8("actSave"));
        actDelPage = new QAction(MainWin);
        actDelPage->setObjectName(QString::fromUtf8("actDelPage"));
        actDelPage->setIcon(icon2);
        actNewDir = new QAction(MainWin);
        actNewDir->setObjectName(QString::fromUtf8("actNewDir"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/folder.png"), QSize(), QIcon::Normal, QIcon::Off);
        actNewDir->setIcon(icon3);
        actSplit = new QAction(MainWin);
        actSplit->setObjectName(QString::fromUtf8("actSplit"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/split.png"), QSize(), QIcon::Normal, QIcon::Off);
        actSplit->setIcon(icon4);
        actSaveSrc = new QAction(MainWin);
        actSaveSrc->setObjectName(QString::fromUtf8("actSaveSrc"));
        actSort = new QAction(MainWin);
        actSort->setObjectName(QString::fromUtf8("actSort"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/sort.png"), QSize(), QIcon::Normal, QIcon::Off);
        actSort->setIcon(icon5);
        actGrabCbrd = new QAction(MainWin);
        actGrabCbrd->setObjectName(QString::fromUtf8("actGrabCbrd"));
        actGrabCbrd->setCheckable(true);
        actInsertSrc = new QAction(MainWin);
        actInsertSrc->setObjectName(QString::fromUtf8("actInsertSrc"));
        actFindUntrn = new QAction(MainWin);
        actFindUntrn->setObjectName(QString::fromUtf8("actFindUntrn"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/find.png"), QSize(), QIcon::Normal, QIcon::Off);
        actFindUntrn->setIcon(icon6);
        actMerge = new QAction(MainWin);
        actMerge->setObjectName(QString::fromUtf8("actMerge"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/merge.png"), QSize(), QIcon::Normal, QIcon::Off);
        actMerge->setIcon(icon7);
        actClearTrn = new QAction(MainWin);
        actClearTrn->setObjectName(QString::fromUtf8("actClearTrn"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/clear.png"), QSize(), QIcon::Normal, QIcon::Off);
        actClearTrn->setIcon(icon8);
        actMergePage = new QAction(MainWin);
        actMergePage->setObjectName(QString::fromUtf8("actMergePage"));
        actIcon = new QAction(MainWin);
        actIcon->setObjectName(QString::fromUtf8("actIcon"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actIcon->setIcon(icon9);
        actSplitLine = new QAction(MainWin);
        actSplitLine->setObjectName(QString::fromUtf8("actSplitLine"));
        actSplitLine->setIcon(icon4);
        actSplitName = new QAction(MainWin);
        actSplitName->setObjectName(QString::fromUtf8("actSplitName"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/talk.png"), QSize(), QIcon::Normal, QIcon::Off);
        actSplitName->setIcon(icon10);
        actAddBookmark = new QAction(MainWin);
        actAddBookmark->setObjectName(QString::fromUtf8("actAddBookmark"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/bookmark.png"), QSize(), QIcon::Normal, QIcon::Off);
        actAddBookmark->setIcon(icon11);
        actRmBookmark = new QAction(MainWin);
        actRmBookmark->setObjectName(QString::fromUtf8("actRmBookmark"));
        actRmBookmark->setIcon(icon2);
        actJoinLine = new QAction(MainWin);
        actJoinLine->setObjectName(QString::fromUtf8("actJoinLine"));
        actJoinLine->setIcon(icon7);
        actSaveBranch = new QAction(MainWin);
        actSaveBranch->setObjectName(QString::fromUtf8("actSaveBranch"));
        actionList = new QAction(MainWin);
        actionList->setObjectName(QString::fromUtf8("actionList"));
        actBookmarks = new QAction(MainWin);
        actBookmarks->setObjectName(QString::fromUtf8("actBookmarks"));
        actBookmarks->setIcon(icon11);
        centralwidget = new QWidget(MainWin);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout_6 = new QHBoxLayout(centralwidget);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tree = new QTreeWidget(centralwidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        tree->setHeaderItem(__qtreewidgetitem);
        tree->setObjectName(QString::fromUtf8("tree"));
        tree->setContextMenuPolicy(Qt::CustomContextMenu);
        tree->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);
        tree->setDragDropMode(QAbstractItemView::InternalMove);
        tree->setDefaultDropAction(Qt::MoveAction);
        tree->setSelectionMode(QAbstractItemView::ExtendedSelection);
        tree->header()->setVisible(false);

        verticalLayout_2->addWidget(tree);


        horizontalLayout_6->addLayout(verticalLayout_2);

        tabs = new QWidget(centralwidget);
        tabs->setObjectName(QString::fromUtf8("tabs"));
        tabs->setEnabled(false);
        verticalLayout = new QVBoxLayout(tabs);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        editGrid = new QGridLayout();
        editGrid->setObjectName(QString::fromUtf8("editGrid"));
        trnline = new QLineEdit(tabs);
        trnline->setObjectName(QString::fromUtf8("trnline"));
        trnline->setContextMenuPolicy(Qt::ActionsContextMenu);

        editGrid->addWidget(trnline, 1, 1, 1, 1);

        srcline = new QLineEdit(tabs);
        srcline->setObjectName(QString::fromUtf8("srcline"));
        srcline->setContextMenuPolicy(Qt::ActionsContextMenu);

        editGrid->addWidget(srcline, 0, 1, 1, 1);

        srcname = new QLineEdit(tabs);
        srcname->setObjectName(QString::fromUtf8("srcname"));

        editGrid->addWidget(srcname, 0, 0, 1, 1);

        trnname = new QLineEdit(tabs);
        trnname->setObjectName(QString::fromUtf8("trnname"));

        editGrid->addWidget(trnname, 1, 0, 1, 1);

        editGrid->setColumnStretch(1, 10);

        verticalLayout->addLayout(editGrid);

        table = new QTableView(tabs);
        table->setObjectName(QString::fromUtf8("table"));
        table->setContextMenuPolicy(Qt::CustomContextMenu);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setGridStyle(Qt::DotLine);

        verticalLayout->addWidget(table);

        widFind = new QWidget(tabs);
        widFind->setObjectName(QString::fromUtf8("widFind"));
        horizontalLayout_3 = new QHBoxLayout(widFind);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label = new QLabel(widFind);
        label->setObjectName(QString::fromUtf8("label"));
        label->setPixmap(QPixmap(QString::fromUtf8(":/find.png")));

        horizontalLayout_3->addWidget(label);

        leFind = new QLineEdit(widFind);
        leFind->setObjectName(QString::fromUtf8("leFind"));

        horizontalLayout_3->addWidget(leFind);


        verticalLayout->addWidget(widFind);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tbScroll = new QToolButton(tabs);
        tbScroll->setObjectName(QString::fromUtf8("tbScroll"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/ArrDown.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbScroll->setIcon(icon12);

        horizontalLayout->addWidget(tbScroll);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        labInfo = new QLabel(tabs);
        labInfo->setObjectName(QString::fromUtf8("labInfo"));

        horizontalLayout->addWidget(labInfo);

        progress = new QProgressBar(tabs);
        progress->setObjectName(QString::fromUtf8("progress"));
        progress->setValue(0);

        horizontalLayout->addWidget(progress);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_6->addWidget(tabs);

        horizontalLayout_6->setStretch(1, 10);
        MainWin->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWin);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 860, 20));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuPage = new QMenu(menubar);
        menuPage->setObjectName(QString::fromUtf8("menuPage"));
        menuOptions = new QMenu(menubar);
        menuOptions->setObjectName(QString::fromUtf8("menuOptions"));
        MainWin->setMenuBar(menubar);
        QWidget::setTabOrder(srcname, srcline);
        QWidget::setTabOrder(srcline, trnname);
        QWidget::setTabOrder(trnname, trnline);
        QWidget::setTabOrder(trnline, table);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuPage->menuAction());
        menubar->addAction(menuOptions->menuAction());
        menuFile->addAction(actNewProj);
        menuFile->addSeparator();
        menuFile->addAction(actOpen);
        menuFile->addAction(actMergePage);
        menuFile->addSeparator();
        menuFile->addAction(actSave);
        menuFile->addAction(actSaveAs);
        menuPage->addAction(actNewPage);
        menuPage->addSeparator();
        menuPage->addAction(actOpenSrc);
        menuPage->addAction(actInsertSrc);
        menuPage->addSeparator();
        menuPage->addAction(actSaveSrc);
        menuOptions->addAction(actGrabCbrd);
        menuOptions->addAction(actBookmarks);

        retranslateUi(MainWin);

        QMetaObject::connectSlotsByName(MainWin);
    } // setupUi

    void retranslateUi(QMainWindow *MainWin)
    {
        MainWin->setWindowTitle(QCoreApplication::translate("MainWin", "TransText 0.5", nullptr));
        actOpen->setText(QCoreApplication::translate("MainWin", "&Open...", nullptr));
#if QT_CONFIG(shortcut)
        actOpen->setShortcut(QCoreApplication::translate("MainWin", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actNewProj->setText(QCoreApplication::translate("MainWin", "&New", nullptr));
        actNewPage->setText(QCoreApplication::translate("MainWin", "&New page", nullptr));
        actOpenSrc->setText(QCoreApplication::translate("MainWin", "&Open source...", nullptr));
        actSaveAs->setText(QCoreApplication::translate("MainWin", "Save &as...", nullptr));
#if QT_CONFIG(tooltip)
        actSaveAs->setToolTip(QCoreApplication::translate("MainWin", "Save as", nullptr));
#endif // QT_CONFIG(tooltip)
        actDelRows->setText(QCoreApplication::translate("MainWin", "Delete rows", nullptr));
        actSave->setText(QCoreApplication::translate("MainWin", "&Save", nullptr));
#if QT_CONFIG(shortcut)
        actSave->setShortcut(QCoreApplication::translate("MainWin", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actDelPage->setText(QCoreApplication::translate("MainWin", "Delete", nullptr));
        actNewDir->setText(QCoreApplication::translate("MainWin", "New dir", nullptr));
        actSplit->setText(QCoreApplication::translate("MainWin", "Split", nullptr));
        actSaveSrc->setText(QCoreApplication::translate("MainWin", "&Save source", nullptr));
        actSort->setText(QCoreApplication::translate("MainWin", "Sort", nullptr));
        actGrabCbrd->setText(QCoreApplication::translate("MainWin", "&Grab clipboard", nullptr));
        actInsertSrc->setText(QCoreApplication::translate("MainWin", "&Insert source...", nullptr));
        actFindUntrn->setText(QCoreApplication::translate("MainWin", "Find untranslated", nullptr));
        actMerge->setText(QCoreApplication::translate("MainWin", "Merge", nullptr));
        actClearTrn->setText(QCoreApplication::translate("MainWin", "Clear translation", nullptr));
        actMergePage->setText(QCoreApplication::translate("MainWin", "&Merge", nullptr));
        actIcon->setText(QCoreApplication::translate("MainWin", "Set icon", nullptr));
        actSplitLine->setText(QCoreApplication::translate("MainWin", "Split line", nullptr));
        actSplitName->setText(QCoreApplication::translate("MainWin", "Split name", nullptr));
        actAddBookmark->setText(QCoreApplication::translate("MainWin", "Edit bookmark", nullptr));
        actRmBookmark->setText(QCoreApplication::translate("MainWin", "Remove bookmark", nullptr));
        actJoinLine->setText(QCoreApplication::translate("MainWin", "Join next line", nullptr));
        actSaveBranch->setText(QCoreApplication::translate("MainWin", "Save branch", nullptr));
        actionList->setText(QCoreApplication::translate("MainWin", "List", nullptr));
        actBookmarks->setText(QCoreApplication::translate("MainWin", "Bookmarks", nullptr));
        label->setText(QString());
#if QT_CONFIG(tooltip)
        tbScroll->setToolTip(QCoreApplication::translate("MainWin", "Find untranslated", nullptr));
#endif // QT_CONFIG(tooltip)
        tbScroll->setText(QCoreApplication::translate("MainWin", "...", nullptr));
        labInfo->setText(QString());
        progress->setFormat(QCoreApplication::translate("MainWin", "%v / %m", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWin", "&File", nullptr));
        menuPage->setTitle(QCoreApplication::translate("MainWin", "&Page", nullptr));
        menuOptions->setTitle(QCoreApplication::translate("MainWin", "&Options", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWin: public Ui_MainWin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWIN_H
