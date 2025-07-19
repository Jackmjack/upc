#ifndef ADDANNOUNCEMENTWIDGET_H
#define ADDANNOUNCEMENTWIDGET_H

#include "database.h"
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>

namespace Ui {
class AddAnnouncementWidget;
}

class AddAnnouncementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AddAnnouncementWidget(QWidget *parent = nullptr);
    ~AddAnnouncementWidget();

signals:
    void backToAnnouncement();

private slots:
    void onSubmitClicked();
    void onBackClicked();

private:
    Ui::AddAnnouncementWidget *ui;
    QLineEdit* titleEdit;
    QTextEdit* textEdit;
    Database db;
};

#endif // ADDANNOUNCEMENTWIDGET_H
