#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

#include "ModelPart.h"
#include "ModelPartList.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void statusUpdateMessage(const QString &message, int timeout);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    // Exercise 5 (ตาม worksheet)
    void handleTreeClicked(const QModelIndex &index);

    void on_actionOpen_File_triggered();
    void on_actionItem_Options_triggered();

private:
    Ui::MainWindow *ui;
    ModelPartList *partList;   // worksheet บอกให้มีตัวนี้ :contentReference[oaicite:1]{index=1}


};

#endif // MAINWINDOW_H
