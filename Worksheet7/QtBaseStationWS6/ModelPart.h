#ifndef VIEWER_MODELPART_H
#define VIEWER_MODELPART_H

#include <QString>
#include <QList>
#include <QVariant>
#include <QColor>

#include <vtkSmartPointer.h>

class vtkSTLReader;
class vtkMapper;
class vtkActor;

class ModelPart {
public:
    ModelPart(const QList<QVariant>& data, ModelPart* parent = nullptr);
    ~ModelPart();
    void appendChild(ModelPart* item);
    ModelPart* child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    void set(int column, const QVariant& value);
    bool setData(int column, const QVariant &value);

    ModelPart* parentItem();
    int row() const;

    void setColour(const QColor& c) { colour = c; }
    QColor getColour() const { return colour; }

    void setVisible(bool isVisible);
    bool visible();

    void loadSTL(QString fileName);
    vtkSmartPointer<vtkActor> getActor();

private:
    QList<ModelPart*>   m_childItems;
    QList<QVariant>     m_itemData;
    ModelPart*          m_parentItem = nullptr;

    QColor colour = QColor(200, 200, 200);
    bool isVisible = true;

    vtkSmartPointer<vtkSTLReader> file;
    vtkSmartPointer<vtkMapper>    mapper;
    vtkSmartPointer<vtkActor>     actor;
};

#endif
