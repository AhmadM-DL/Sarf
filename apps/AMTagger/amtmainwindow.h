#ifndef AMTMAINWINDOW_H
#define AMTMAINWINDOW_H

#include <QMainWindow>
#include<QFileDialog>
#include<QTextBrowser>
#include<QTreeWidget>
#include<QTextEdit>

#include <QMenu>
#include <QMenuBar>
#include <QSignalMapper>
#include <QTabWidget>
#include <QLabel>
#include <QGraphicsView>
#include "graphedge.h"
#include "graphnode.h"
#include "ATMProgressIFC.h"
#include "commonS.h"
#include "addtagview.h"
#include "addtagtypeview.h"
#include "removetagtypeview.h"
#include "global.h"
#include "edittagtypeview.h"
#include "customsttview.h"
#include "diffview.h"
#include "customizemsfview.h"

class AMTMainWindow : public QMainWindow,public ATMProgressIFC
{
    Q_OBJECT

    protected:
         //void contextMenuEvent(QContextMenuEvent *event);

     public slots:
         void save();
         void showContextMenu(const QPoint &pt);
         void tagWord(int start, int length, QColor fcolor, QColor bcolor,int font, bool underline, bool italic, bool bold);
         void tag(QString tagValue);
         void untag(QString tagValue);
         void itemSelectionChanged(QTreeWidgetItem*,int);

     private slots:
         void _new();
         void open();
         bool saveas();
         void edittagtypes();
         void tagtypeadd();
         void tagtyperemove();
         void addtagtype();
         void viewMBFTags();
         void viewMSFTags();
         void about();
         void aboutQt();
         void sarfTagging(bool color=true);
         void customizeSarfTags();
         void customizeMSFs();
         void runMERFSimulator();
         void editUserCrossRelations();
         void extractUserCrossRelations();
         void extractDefaultCrossRelations();
         void loadText_clicked();
         void loadTagTypes_clicked();
         void closeEvent(QCloseEvent *event);
         void difference();

public:
         virtual void report(int value);
         virtual void tag(int start, int length,QColor color, bool textcolor);
         virtual void setCurrentAction(const QString & s);
         virtual void resetActionDisplay();
         virtual QString getFileName();

         void fillTreeWidget(Source Data, int basic=0);
         void finishTaggingText();
         void process(QByteArray & json);
         void applyTags(int basic=0);
         //void process_TagTypes(QByteArray &tagtypedata);
         //bool readMSF(MSFormula* formula, QVariant data, MSF* parent);
         //bool readMatch(MSFormula* formula, QVariant data, Match* parent);

private:
         void createActions();
         void createMenus();
         void createTagMenu();
         void createUntagMenu();
         void createDockWindows(bool open);
         void startTaggingText(QString & text);
         bool saveFile(const QString &fileName, QByteArray &tagD);
         void setLineSpacing(int lineSpacing);

         QMenu *fileMenu;
         QMenu *tagMenu;
         QMenu *tagtypeMenu;
         QMenu *editMenu;
         QMenu *viewMenu;
         QMenu *paneMenu;
         QMenu *helpMenu;
         QMenu *mTags;
         QMenu *umTags;
         QMenu *sarfMenu;
         QMenu *analyseMenu;

         QAction *newAct;
         QAction *openAct;
         QAction *saveAct;
         QAction *saveasAct;
         QAction *exitAct;
         //QAction *tagaddAct;
         //QAction *tagremoveAct;
         QAction *edittagtypesAct;
         QAction *tagtypeaddAct;
         QAction *tagtyperemoveAct;
         QAction *tagAct;
         QAction *untagMAct;
         QAction *addtagAct;
         QAction *viewMBFTagAct;
         QAction *viewMSFTagAct;
         QAction *aboutAct;
         QAction *sarftagsAct;
         QAction *sarfAct;
         QAction *editMSFAct;
         QAction *simulatorAct;
         QAction *edituserCrossRefAct;
         QAction *userCrossRefAct;
         QAction *defaultCrossRefAct;
         QAction *diffAct;

         QTextBrowser * txtBrwsr;
         QTreeWidget * tagDescription;
         QTreeWidget * descBrwsr;

         QLabel *lblTFName;
         QLabel *lblTTFName;

         QPushButton *btnTFName;
         QPushButton *btnTTFName;

         QLineEdit *lineEditTFName;
         QLineEdit *lineEditTTFName;

         QTabWidget* tab;
         QGraphicsScene *scene;
         QGraphicsView *graphics;
         QGraphicsScene *relationScene;
         QGraphicsView *relationGraphics;
public:
         explicit AMTMainWindow(QWidget *parent = 0);
         ~AMTMainWindow();

protected:
         void wheelEvent(QWheelEvent *event);
         void scaleView(qreal scaleFactor);
         void relationScaleView(qreal scaleFactor);
private:
         QFileDialog * browseFileDlg;
         QSignalMapper *signalMapper;
         QSignalMapper *signalMapperM;
         QSignalMapper *signalMapperU;
         QSignalMapper *signalMapperUM;
         QString error_str;
         QString output_str;
         QTextCursor myTC;
         bool dirty;
};

#endif // AMTMAINWINDOW_H
