#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ModelPartList.h"

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>

/**
 * @class MainWindow
 * @brief Main application window handling UI and VTK rendering.
 *
 * This class manages:
 * - File loading
 * - Tree view of model parts
 * - VTK rendering pipeline
 */

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief Main application window handling UI and VTK rendering.
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void handleButton();
    void handleTreeClicked();
    /**
 * @brief Opens a file and loads a CAD model.
 */
    void on_actionOpen_File_triggered();
    void handleOptionsButton();
    void on_actionItem_Options_triggered();
    void updateRender();
    void updateRenderFromTree(const QModelIndex& index);

signals :
    void statusUpdateMessage(const QString & message ,int timeout);

private:
    Ui::MainWindow *ui;
    ModelPartList *partList;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;

};
#endif // MAINWINDOW_H
