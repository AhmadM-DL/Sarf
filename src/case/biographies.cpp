#include "biographies.h"

void BiographiesWindow::report(int value) {
    progressBar->setValue(value);
}
void BiographiesWindow::setCurrentAction(const QString &s) {
    progressBar->setFormat(s + "(%p%)");
}

void BiographiesWindow::resetActionDisplay() {
    progressBar->setFormat("%p%");
    progressBar->reset();
}

void BiographiesWindow::tag(int start, int length, QColor color, bool textcolor) {
    QTextBrowser *taggedBox = text;
    QTextCursor c = taggedBox->textCursor();
    /*if (length>10000) {
      start=start+length-1;
      length=5;
      color=Qt::red;
      }*/
    c.setPosition(start, QTextCursor::MoveAnchor);
    c.setPosition(start + length, QTextCursor::KeepAnchor);
    taggedBox->setTextCursor(c);

    if (textcolor) {
        taggedBox->setTextColor(color);
    } else {
        taggedBox->setTextBackgroundColor(color);
    }
}

void BiographiesWindow::startTaggingText(QString &text) {
    QTextBrowser *taggedBox = this->text;
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

void BiographiesWindow::finishTaggingText() {
    QTextBrowser *taggedBox = text;
    QTextCursor c = taggedBox->textCursor();
    c.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    taggedBox->setTextCursor(c);
}

void biographies(NarratorGraph *graph) {
    BiographiesWindow *g = new BiographiesWindow(graph);
    g->show();
}
