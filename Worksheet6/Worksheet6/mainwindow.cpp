#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ModelPartList.h"
#include <QFileDialog>
#include "optiondialog.h"
#include "ModelPart.h"
#include <QModelIndex>
#include <QFileDialog>
#include <QFileInfo>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->treeView->addAction(ui->actionItem_Options);


    partList = new ModelPartList("Parts List", this);
    ui->treeView->setModel(partList);

    connect(this, &MainWindow::statusUpdateMessage,
            ui->statusbar, &QStatusBar::showMessage);

    connect(ui->treeView, &QTreeView::clicked,
            this, &MainWindow::handleTreeClicked);


    ModelPart *rootItem = partList->getRootItem();

    for (int i = 0; i < 3; i++)
    {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        ModelPart *childItem = new ModelPart({ name, visible });
        rootItem->appendChild(childItem);

        for (int j = 0; j < 5; j++)
        {
            QString name = QString("Item %1,%2").arg(i).arg(j);
            QString visible("true");

            ModelPart *childChildItem = new ModelPart({ name, visible });
            childItem->appendChild(childChildItem);
        }
    }

    ui->treeView->expandAll();
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
    data << "New Part" << "Yes";

    model->appendChild(parentIndex, data);
}

void MainWindow::on_pushButton_2_clicked()
{
    OptionDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted)
    {
        emit statusUpdateMessage("Dialog accepted", 0);
    }
    else
    {
        emit statusUpdateMessage("Dialog cancelled", 0);
    }
}

void MainWindow::handleTreeClicked(const QModelIndex &index)
{
    Q_UNUSED(index);

    /* Get the index of the selected item */
    QModelIndex idx = ui->treeView->currentIndex();

    /* Get a pointer to the item from the index */
    ModelPart *selectedPart = static_cast<ModelPart*>(idx.internalPointer());

    /* Retrieve the name string from the internal QVariant data array */
    QString text = selectedPart->data(0).toString();

    emit statusUpdateMessage(QString("The selected item is : ") + text, 0);
}

void MainWindow::on_actionOpen_File_triggered()
{
    QModelIndex idx = ui->treeView->currentIndex();
    if (!idx.isValid()) {
        emit statusUpdateMessage("Select an item first", 0);
        return;
    }

    ModelPart *part = static_cast<ModelPart*>(idx.internalPointer());
    if (!part) return;

    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Open STL",
        "",
        "STL Files (*.stl);;All Files (*)"
        );

    if (fileName.isEmpty()) return;


    QFileInfo fi(fileName);
    part->set(0, fi.fileName());


    part->loadSTL(fileName);

    ui->treeView->viewport()->update();
    emit statusUpdateMessage("Loaded: " + fi.fileName(), 0);
}
void MainWindow::on_actionItem_Options_triggered()
{
    emit statusUpdateMessage("Options triggered", 0);

    QModelIndex idx = ui->treeView->currentIndex();
    if (!idx.isValid()) return;


    ModelPart *part = static_cast<ModelPart*>(idx.internalPointer());
    if (!part) return;

    OptionDialog dlg(this);


    dlg.setName(part->data(0).toString());
    dlg.setPartVisible(part->visible());


    QString vis = part->data(1).toString().trimmed().toLower();
    bool visibleBool = (vis == "true");
    dlg.setVisible(visibleBool);


    QColor c = part->getColour();
    dlg.setColour(c.red(), c.green(), c.blue());

    if (dlg.exec() == QDialog::Accepted) {

        part->setVisible(dlg.getPartVisible());
        ui->treeView->viewport()->update();
        part->set(0, dlg.getName());
        part->setColour(dlg.getColour());

        emit statusUpdateMessage("Dialog accepted", 0);

        ui->treeView->viewport()->update();
    } else {
        emit statusUpdateMessage("Dialog cancelled", 0);
    }
}
