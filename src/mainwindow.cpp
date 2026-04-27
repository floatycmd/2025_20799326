#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include "optiondialog.h"

#include <vtkSmartPointer.h>
#include <vtkNew.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleOptionsButton);
    ui->treeView->addAction(ui->actionItem_Options);

    connect(this, &MainWindow::statusUpdateMessage,
            ui->statusbar, &QStatusBar::showMessage);
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);

    this->partList = new ModelPartList("PartsList");

    ui->treeView->setModel(this->partList);

    ModelPart* rootItem = this->partList->getRootItem();

    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        ModelPart* childItem = new ModelPart({ name, visible,255,255,255 });
        rootItem->appendChild(childItem);

        for (int j = 0; j < 5; j++) {
            QString subName = QString("Item %1,%2").arg(i).arg(j);
            ModelPart* childChildItem = new ModelPart({ subName, visible,255,255,255 });
            childItem->appendChild(childChildItem);
        }
    }

    // 1. 将渲染窗口与 Qt 部件关联
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->widget->setRenderWindow(renderWindow);

    // 2. 添加渲染器
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    // 7. 重置并设置相机视角
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButton() {
    emit statusUpdateMessage( QString("Add button was clicked") , 0 );
}

void MainWindow::handleTreeClicked() {
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    QString text = selectedPart->data(0).toString();

    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);
}

void MainWindow::on_actionOpen_File_triggered() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\",
        tr("STL Files (*.stl);;Text Files (*.txt)")
        );

    if (!fileName.isEmpty()) {
        QFileInfo fileInfo(fileName);
        QString onlyFileName = fileInfo.fileName(); // 获取包含后缀的文件名
        QModelIndex index = ui->treeView->currentIndex();

        if (index.isValid()) {
            // 获取当前在 TreeView 中选中的节点
            ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

            // 1. 创建一个新节点作为子节点 (这里假设你的 ModelPart 构造函数接受 QList<QVariant> 且有 appendChild 方法)
            // 注意：如果你在 Worksheet 6 中的 TreeModel 实现了专门的 insertRow 方法，请使用你自己的插入逻辑。
            // 将原来的 ModelPart* newItem = new ModelPart({onlyFileName}); 修改为：
            // 1. 创建一个新节点 (此时数组里只需要传名字)
            // 确保是用这行代码创建新节点，不要只传 {onlyFileName}
            ModelPart* newItem = new ModelPart({onlyFileName, "true"}); // 数组里只传 UI 需要显示的文

            // 将它添加到选中的节点下
            selectedPart->appendChild(newItem);
            ui->treeView->model()->layoutChanged();

            // 2. 调用新创建项目的 loadSTL() 函数来读取 STL 文件
            newItem->loadSTL(fileName);

            // 3. 更新 VTK 渲染器，把新加载的 3D 模型显示出来
            updateRender();

            // （可选）重置相机，确保模型在视野正中间
            renderer->ResetCamera();
            renderWindow->Render();

            emit statusUpdateMessage(QString("Loaded STL file: ") + onlyFileName, 0);
        } else {
            emit statusUpdateMessage(QString("File chosen, but no parent item selected in the tree!"), 0);
        }
    }
}

void MainWindow::handleOptionsButton() {
    QModelIndex index = ui->treeView->currentIndex();

    if (!index.isValid()) {
        emit statusUpdateMessage(QString("Please select an item first!"), 0);
        return;
    }

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    OptionDialog dialog(this);

    // 1. 使用我们刚刚写好的专用函数来读取颜色和状态
    dialog.setInitialData(
        selectedPart->data(0).toString(), // 只有名字保留在原生数据列里
        selectedPart->visible(),
        selectedPart->getColourR(),
        selectedPart->getColourG(),
        selectedPart->getColourB()
        );

    if (dialog.exec() == QDialog::Accepted) {

        QString newName = dialog.getName();
        bool newVisible = dialog.getIsVisible();
        int r = dialog.getR();
        int g = dialog.getG();
        int b = dialog.getB();
        selectedPart->set(0, newName); // 名字还是用原生的存
        selectedPart->setVisible(newVisible); // 内部会自动更新 actor 并同步给 TreeView
        selectedPart->setColour(r, g, b);     // 内部会自动更新 a
        // 4. 刷新 3D 画面
        renderWindow->Render();

        emit statusUpdateMessage(QString("Dialog accepted: Item updated"), 0);
    } else {
        emit statusUpdateMessage(QString("Dialog rejected"), 0);
    }
}

void MainWindow:: on_actionItem_Options_triggered () {
    handleOptionsButton();
}

void MainWindow::updateRender() {
    // 移除所有现有的 actor
    renderer->RemoveAllViewProps();

    // 【修改点】：获取所有顶层节点的数量，并循环遍历每一个顶层节点分支
    int topLevelRows = partList->rowCount(QModelIndex());
    for (int i = 0; i < topLevelRows; i++) {
        updateRenderFromTree(partList->index(i, 0, QModelIndex()));
    }

    // 强制立即更新渲染器
    renderer->Render();
}

void MainWindow::updateRenderFromTree(const QModelIndex& index) {
    if (index.isValid()) {
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

        // [补全代码] 从选定的 part 中检索 actor 并添加到渲染器 [cite: 362]
        vtkSmartPointer<vtkActor> partActor = selectedPart->getActor();
        if (partActor != nullptr) {
            renderer->AddActor(partActor);
        }
    }

    // 检查此部件是否有子部件
    if (!partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren)) {
        return;
    }

    // 循环遍历子项并添加它们的 actor
    int rows = partList->rowCount(index);
    for (int i = 0; i < rows; i++) {
        updateRenderFromTree(partList->index(i, 0, index));
    }
}
