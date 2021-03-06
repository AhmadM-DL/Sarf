#include <QFileInfo>
#include <QScrollArea>
#include "mainwindow.h"
#include "test.h"
#include "sql_queries.h"
#include "ui_mainwindow.h"
#include "database_info_block.h"
#include "hadithCommon.h"
#include "stemmer.h"
#include "timeRecognizer.h"
#include <QFileDialog>
#include "bibleGeneology.h"
#include "geneMainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), browseFileDlg(NULL), m_ui(new Ui::MainWindow) {
    m_ui->setupUi(this);
    m_ui->pushButton->setVisible(false);
    m_ui->EQ_threshold->setText("0.1");
}

MainWindow::~MainWindow() {
    delete m_ui;
}

void MainWindow::report(int value) {
    m_ui->progressBar->setValue(value);
}
void MainWindow::setCurrentAction(const QString &s) {
    m_ui->progressBar->setFormat(s + "(%p%)");
}

void MainWindow::resetActionDisplay() {
    m_ui->progressBar->setFormat("%p%");
    m_ui->progressBar->reset();
}

void MainWindow::tag(int start, int length, QColor color, bool textcolor) {
    QTextBrowser *taggedBox = m_ui->hadith_display;
    QTextCursor c = taggedBox->textCursor();
    c.setPosition(start, QTextCursor::MoveAnchor);
    c.setPosition(start + length, QTextCursor::KeepAnchor);
    taggedBox->setTextCursor(c);

    if (textcolor) {
        taggedBox->setTextColor(color);
    } else {
        taggedBox->setTextBackgroundColor(color);
    }
}

void MainWindow::startTaggingText(QString &text) {
    QTextBrowser *taggedBox = m_ui->hadith_display;
    taggedBox->clear();
    taggedBox->setLayoutDirection(Qt::RightToLeft);
    QTextCursor c = taggedBox->textCursor();
    c.clearSelection();
    c.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    taggedBox->setTextCursor(c);
    taggedBox->setTextBackgroundColor(Qt::white);
    taggedBox->setTextColor(Qt::black);
    taggedBox->setText(text);
}

void MainWindow::finishTaggingText() {
    QTextBrowser *taggedBox = m_ui->hadith_display;
    QTextCursor c = taggedBox->textCursor();
    c.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    taggedBox->setTextCursor(c);
}

QString MainWindow::getFileName() {
    if (browseFileDlg == NULL) {
        QString dir = QDir::currentPath();
        browseFileDlg = new QFileDialog(NULL, QString("Open File"), dir, QString("All Files (*)"));
        browseFileDlg->setOptions(QFileDialog::DontUseNativeDialog);
        browseFileDlg->setFileMode(QFileDialog::ExistingFile);
        browseFileDlg->setViewMode(QFileDialog::Detail);
    }

    if (browseFileDlg->exec()) {
        QStringList files = browseFileDlg->selectedFiles();
        QString fileName = files[0];
        return fileName;
    }

    return "";
}

void MainWindow::changeEvent(QEvent *e) {
    QMainWindow::changeEvent(e);

    switch (e->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;

        default:
            break;
    }
}

// Function called upon pressing "Hadith" button
void MainWindow::on_chk_hadith_toggled(bool checked) {
    if (checked) {
        m_ui->label_narr->setText("NARR Min:");
        m_ui->label_nrc->setText("NRC Max:");
        m_ui->label_eq_delta->setText("EQ delta:");
        m_ui->NARRATOR->setText(QString("%1").arg(hadithParameters.narr_min));
        m_ui->NMC->setText(QString("%1").arg(hadithParameters.nmc_max));
        m_ui->NRC->setText(QString("%1").arg(hadithParameters.nrc_max));
        m_ui->EQ_radius->setText(QString("%1").arg(hadithParameters.equality_radius));
        m_ui->EQ_delta->setText(QString("%1").arg(hadithParameters.equality_delta));
        m_ui->NARRATOR->setAlignment(Qt::AlignCenter);
        m_ui->NMC->setAlignment(Qt::AlignCenter);
        m_ui->NRC->setAlignment(Qt::AlignCenter);
        m_ui->EQ_radius->setAlignment(Qt::AlignCenter);
        m_ui->EQ_delta->setAlignment(Qt::AlignCenter);
    }
}

void MainWindow::on_chk_bible_toggled(bool checked) {
    if (checked) {
        m_ui->label_narr->setText("Level Min:");
        m_ui->label_nrc->setText("Keep Max:");
        m_ui->label_eq_delta->setText("Count Min");
        m_ui->NARRATOR->setText(QString("%1").arg(geneologyParameters.L_min));
        m_ui->NMC->setText(QString("%1").arg(geneologyParameters.theta_0));
        m_ui->NRC->setText(QString("%1").arg(geneologyParameters.C_max));
        m_ui->EQ_radius->setText(QString("%1").arg(geneologyParameters.radius));
        m_ui->EQ_delta->setText(QString("%1").arg(geneologyParameters.N_min));
        m_ui->NARRATOR->setAlignment(Qt::AlignCenter);
        m_ui->NMC->setAlignment(Qt::AlignCenter);
        m_ui->NRC->setAlignment(Qt::AlignCenter);
        m_ui->EQ_radius->setAlignment(Qt::AlignCenter);
        m_ui->EQ_delta->setAlignment(Qt::AlignCenter);
    }
}

QString error_str, output_str, hadith_str;  // strings for error, output, and input hadith

// Function called upon pressing "GO!" button
void MainWindow::on_pushButton_clicked() {
    error_str = "";
    output_str = "";
    /*
     * v1: check for min narrator number conversion
     * v2: check for max NMC tolerance number conversion
     * v3: check for max NRC tolerance number conmversion
     * v4:
     * v5:
     * v6:
     */
    bool v1, v2, v3, v4, v5, v6 = true;
    int narr_min = m_ui->NARRATOR->toPlainText().toInt(&v1); //set minimum number of narrators
    int nmc_max = m_ui->NMC->toPlainText().toInt(&v2);  //max number of name connectors between 2 names ~ NMC
    int nrc_max = m_ui->NRC->toPlainText().toInt(&v3);  //max number of words tolerated between 2 narrator connectors ~ NRC
    int equality_radius = m_ui->EQ_radius->toPlainText().toInt(&v5);

    if (m_ui->chk_hadith->isChecked()) {
        // check if hadith option is selected
        hadithParameters.equality_delta = m_ui->EQ_delta->toPlainText().toDouble(&v4);
        hadithParameters.narr_min = narr_min;
        hadithParameters.nmc_max = nmc_max;
        hadithParameters.nrc_max = nrc_max;
        hadithParameters.equality_radius = equality_radius;
    } else if (m_ui->chk_bible->isChecked()) {
        geneologyParameters.N_min = m_ui->EQ_delta->toPlainText().toInt(&v4);
        geneologyParameters.L_min = narr_min;
        geneologyParameters.theta_0 = nmc_max;
        geneologyParameters.C_max = nrc_max;
        geneologyParameters.radius = equality_radius;
        geneologyParameters.refined = m_ui->chk_genealogyRefined->isChecked();
    }

    hadithParameters.equality_threshold = m_ui->EQ_threshold->toPlainText().toDouble(&v6);
    hadithParameters.display_chain_num = m_ui->chk_chainNum->isChecked();
    hadithParameters.break_cycles = m_ui->chk_breakCycles->isChecked();
    hadithParameters.detailed_statistics = geneologyParameters.detailed_statistics =
                                               m_ui->chk_detailed_Statistics->isChecked();
    sarfParameters.enableRunonwords = m_ui->chk_runon->isChecked();

    if ((m_ui->chk_hadith->isChecked() || m_ui->chk_bible->isChecked()) && (!v1 || !v2 || !v3 || !v4 || !v5 || !v6)) {
        m_ui->errors->setText("Parameters for Hadith/Genealogy Segmentaion are not valid integers/doubles!\n");
        return;
    }

    QString input = m_ui->input->toPlainText(); //get the input text/word from the GUI
    theSarf->out.setString(&output_str);
    theSarf->out.setCodec("utf-8");
    theSarf->in.setString(&input);
    theSarf->in.setCodec("utf-8");
    theSarf->displayed_error.setString(&error_str);
    theSarf->displayed_error.setCodec("utf-8");
    hadith_out.setString(&hadith_str);
    hadith_out.setCodec("utf-8");

    //int rc;
    if (m_ui->chk_verification->isChecked()) {
        verify(input, this);
    } else if (m_ui->chk_hadith->isChecked()) {
        hadith(input, this);    //rc=hadith(input,this);
    } else if (m_ui->chk_morphology->isChecked()) { //checking for morphology option
        morphology(input, this);    //calling morphology routine
    } else if (m_ui->chk_testing->isChecked()) {
        test(input, this);
    } else if (m_ui->chk_AffixBreaker->isChecked()) {
        breakAffix(input, this);
    } else if (m_ui->chk_time->isChecked()) {
        timeRecognize(input, this);
    } else if (m_ui->chk_bible->isChecked()) {
        genealogy(input, this);
    } else if (m_ui->chk_biography->isChecked()) {
        biography(input, this);
    } else if (m_ui->chk_time_anotation->isChecked()) {
        simple_annotation(input, this);
    } else if (m_ui->chk_bible_anotation->isChecked()) {
        bible_annotation(input, this);
    } else if (m_ui->chk_hadith_anotation->isChecked()) {
        hadith_annotation(input, this);
    } else if (m_ui->chk_hadith_name_anotation->isChecked()) {
        hadith_name_annotation(input, this);
    } else if (m_ui->chk_hadith_agreement->isChecked()) {
        hadith_agreement(input, this);
    } else if (m_ui->chk_bible_agreement->isChecked()) {
        genealogy_agreement(input, this);
    } else if (m_ui->chk_narrator_annotation->isChecked()) {
        narrator_annotation(input, this);
    } else if (m_ui->chk_narrator_equality->isChecked()) {
        narrator_equality_comparision(input, this);
    }

    if (!m_ui->chk_hadith->isChecked() && !m_ui->chk_time->isChecked() && !m_ui->chk_bible->isChecked() &&
        !m_ui->chk_testing->isChecked()) {
        m_ui->hadith_display->setText(hadith_str);
    }

    m_ui->errors->setText(error_str);
    m_ui->output->setText(output_str);
}
void MainWindow::displayGraph(AbstractGraph *graph) {
    try {
        system("dot -Tsvg graph.dot -o graph.svg");

        if (graph == NULL) {
            QMainWindow *mw = new QMainWindow(NULL);
            mw->setWindowTitle(QString("Sarf Graph (") + m_ui->input->toPlainText() + ")");
            QScrollArea *sa = new QScrollArea(mw);
            mw->setCentralWidget(sa);
            QLabel *pic = new QLabel(sa);
            pic->setPixmap(QPixmap("./graph.svg"));
            sa->setWidget(pic);
            mw->show();
        } else {
            GeneMainWindow *m = new GeneMainWindow;
            m->show();
            GeneTree *t = (dynamic_cast<GeneTree *>(graph))->duplicate();
            m->display(t, "./graph.svg", true);
        }
    } catch (...) {
    }
}

// Initialize the lexicon
void MainWindow::on_fill_clicked() {
    theSarf = new Sarf();
    bool all_set = theSarf->start(&output_str, &error_str, this);

    if (!all_set) {
        m_ui->errors->setText("Can't set up the project\n");
    }

    Sarf::use(theSarf);
    initialize_other();
    hadith_initialize();
    geneology_initialize();
    time_initialize();
    m_ui->pushButton->setVisible(true);
    m_ui->fill->setVisible(false);
}

void MainWindow::on_cmd_browse_clicked() {
    QString fileName = getFileName();

    if (!fileName.isEmpty()) {
        m_ui->input->setText(fileName);
    }
}

int main(int argc, char *argv[]) {
    QFileInfo fileinfo(argv[0]);
    executable_timestamp = fileinfo.lastModified();
    QApplication app(argc, argv);
    MainWindow mainw;
    mainw.show();
    mainw.raise();
    int r = app.exec();

    if (theSarf != NULL) {
        theSarf->exit();
        delete theSarf;
    }

    return r;
}

void MainWindow::on_exit_clicked() {
    exit(0);
}
