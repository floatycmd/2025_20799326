#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

#include <vtkSmartPointer.h>

class vtkRenderer;
class vtkGenericOpenGLRenderWindow;

class ModelPartList;

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
    void handleTreeClicked(const QModelIndex &index);

    void on_actionOpen_File_triggered();
    void on_actionItem_Options_triggered();

private:
    void updateRender();
    void updateRenderFromTree(const QModelIndex &parent);

private:
    Ui::MainWindow *ui = nullptr;
    ModelPartList *partList = nullptr;

    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
};

#endif // MAINWINDOW_H
