#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ModelPartList.h"
#include "ModelPart.h"
#include "optiondialog.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QStatusBar>

#include <QVTKOpenGLNativeWidget.h>

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkNew.h>

// (สำหรับ cylinder demo)
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ---- VTK render window ----
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->widget->setRenderWindow(renderWindow);

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    // ---- Tree model ----
    partList = new ModelPartList("Parts List", this);
    ui->treeView->setModel(partList);
    ui->treeView->expandAll();

    // ให้ treeView มี action options (คลิกขวาได้ถ้าตั้ง context menu ใน UI แล้ว)
    ui->treeView->addAction(ui->actionItem_Options);

    connect(this, &MainWindow::statusUpdateMessage,
            ui->statusbar, &QStatusBar::showMessage);

    connect(ui->treeView, &QTreeView::clicked,
            this, &MainWindow::handleTreeClicked);

    // ---- สร้างของเดิมใน tree (เหมือน worksheet) ----
    ModelPart *rootItem = partList->getRootItem();

    for (int i = 0; i < 3; i++)
    {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true"); // เก็บเป็น string เฉย ๆ

        ModelPart *childItem = new ModelPart({ name, visible });
        rootItem->appendChild(childItem);

        for (int j = 0; j < 5; j++)
        {
            QString n2 = QString("Item %1,%2").arg(i).arg(j);
            QString v2("true");

            ModelPart *childChildItem = new ModelPart({ n2, v2 });
            childItem->appendChild(childChildItem);
        }
    }
    ui->treeView->expandAll();

    // ---- Cylinder demo (เพื่อให้เห็นว่าหน้าจอดำทำงาน) ----
    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(24);

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(cylinder->GetOutputPort());

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(1.0, 0.0, 0.35);

    renderer->AddActor(actor);
    renderer->ResetCamera();
    renderWindow->Render();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    emit statusUpdateMessage("Add button was clicked", 0);

    ModelPartList *model = static_cast<ModelPartList*>(ui->treeView->model());
    QModelIndex parentIndex = ui->treeView->currentIndex();

    QList<QVariant> data;
    data << "New Part" << "true";

    model->appendChild(parentIndex, data);
    ui->treeView->expandAll();
}

void MainWindow::on_pushButton_2_clicked()
{
    OptionDialog dialog(this);
    dialog.exec();
}

void MainWindow::handleTreeClicked(const QModelIndex &index)
{
    Q_UNUSED(index);

    QModelIndex idx = ui->treeView->currentIndex();
    if (!idx.isValid()) return;

    ModelPart *selectedPart = static_cast<ModelPart*>(idx.internalPointer());
    if (!selectedPart) return;

    QString text = selectedPart->data(0).toString();
    emit statusUpdateMessage(QString("The selected item is : ") + text, 0);
}

void MainWindow::on_actionOpen_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, "Open STL", "", "STL Files (*.stl)"
        );
    if (fileName.isEmpty()) return;

    QModelIndex parentIndex = ui->treeView->currentIndex();

    QList<QVariant> data;
    data << QFileInfo(fileName).fileName() << "true";

    ModelPartList* model = static_cast<ModelPartList*>(ui->treeView->model());
    QModelIndex newIndex = model->appendChild(parentIndex, data);

    ModelPart* part = static_cast<ModelPart*>(newIndex.internalPointer());
    if (!part) return;

    // โหลด STL เข้า actor ของ part
    part->loadSTL(fileName);

    // อัปเดต render (แต่ "ยังไม่เช็ค visible" แบบจริงจัง — ตามที่คุณขอ)
    updateRender();

    ui->treeView->expandAll();
}

void MainWindow::on_actionItem_Options_triggered()
{
    QModelIndex idx = ui->treeView->currentIndex();
    if (!idx.isValid()) return;

    ModelPart *part = static_cast<ModelPart*>(idx.internalPointer());
    if (!part) return;

    OptionDialog dlg(this);

    // set ค่าใน dialog
    dlg.setName(part->data(0).toString());
    dlg.setPartVisible(part->visible());
    dlg.setColour(part->getColour().red(), part->getColour().green(), part->getColour().blue());

    if (dlg.exec() == QDialog::Accepted) {
        // อัปเดตข้อมูลใน part
        part->set(0, dlg.getName());
        part->setVisible(dlg.getPartVisible());
        part->setColour(dlg.getColour());

        // NOTE: โหมด “ตั้งหลัก” = ยังไม่บังคับให้ visible ส่งผลกับ renderer
        // (เดี๋ยวเราค่อยทำรอบแก้ visible แบบถูกทาง)
        updateRender();

        ui->treeView->viewport()->update();
    }
}

void MainWindow::updateRender()
{
    renderer->RemoveAllViewProps();

    // เอา actor ของทุก part ใส่หมด (เลยทับกัน ถ้าไม่ได้ set position)
    updateRenderFromTree(QModelIndex());

    renderer->ResetCamera();
    renderWindow->Render();
}

void MainWindow::updateRenderFromTree(const QModelIndex &parent)
{
    int rows = partList->rowCount(parent);
    for (int r = 0; r < rows; ++r) {

        QModelIndex idx0 = partList->index(r, 0, parent);
        ModelPart *part = static_cast<ModelPart*>(idx0.internalPointer());

        if (part && part->visible()) {              // ✅ เช็คตรงนี้
            auto act = part->getActor();
            if (act) renderer->AddActor(act);
        }

        updateRenderFromTree(idx0);
    }
}
