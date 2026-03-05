#include "ModelPart.h"

#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>

ModelPart::ModelPart(const QList<QVariant>& data, ModelPart* parent)
    : m_itemData(data), m_parentItem(parent)
{
    // default visible = true ถ้า column1 เป็น "true"
    if (m_itemData.size() >= 2) {
        QString s = m_itemData[1].toString().trimmed().toLower();
        isVisible = (s == "true" || s == "1");
    }
}

ModelPart::~ModelPart() {
    qDeleteAll(m_childItems);
}

void ModelPart::appendChild(ModelPart* item) {
    item->m_parentItem = this;
    m_childItems.append(item);
}

ModelPart* ModelPart::child(int row) {
    if (row < 0 || row >= m_childItems.size()) return nullptr;
    return m_childItems.at(row);
}

int ModelPart::childCount() const { return m_childItems.count(); }
int ModelPart::columnCount() const { return m_itemData.count(); }

QVariant ModelPart::data(int column) const {
    if (column < 0 || column >= m_itemData.size()) return QVariant();
    return m_itemData.at(column);
}

void ModelPart::set(int column, const QVariant& value)
{
    if (column < 0 || column >= m_itemData.size()) return;
    m_itemData[column] = value;

    if (column == 1) {
        QString s = value.toString().trimmed().toLower();
        setVisible(s == "true" || s == "1");
    }
}

bool ModelPart::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= m_itemData.size()) return false;
    set(column, value);
    return true;
}

ModelPart* ModelPart::parentItem() { return m_parentItem; }

int ModelPart::row() const {
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<ModelPart*>(this));
    return 0;
}

void ModelPart::setVisible(bool v)
{
    isVisible = v;

    if (m_itemData.size() >= 2)
        m_itemData[1] = v ? "true" : "false";

    // ✅ ถ้าโหลด STL แล้ว ให้ซ่อน/โชว์ actor ด้วย
    if (actor) actor->SetVisibility(v ? 1 : 0);
}

bool ModelPart::visible() { return isVisible; }

void ModelPart::loadSTL(QString fileName)
{
    file = vtkSmartPointer<vtkSTLReader>::New();
    file->SetFileName(fileName.toStdString().c_str());
    file->Update();

    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(file->GetOutputPort());

    actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    // ใช้สีจาก colour ที่เก็บไว้
    actor->GetProperty()->SetColor(colour.redF(), colour.greenF(), colour.blueF());
    actor->SetVisibility(isVisible ? 1 : 0);
}

vtkSmartPointer<vtkActor> ModelPart::getActor()
{
    return actor;
}
