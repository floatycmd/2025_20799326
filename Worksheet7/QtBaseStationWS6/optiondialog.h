#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include <QColor>

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog();

    // Name
    void setName(const QString &name);
    QString getName() const;

    // Visible (อย่าใช้ชื่อ setVisible เพราะชนกับ QWidget/QDialog)
    void setPartVisible(bool visible);
    bool getPartVisible() const;

    // Colour (RGB)
    void setColour(int r, int g, int b);
    QColor getColour() const;

private:
    Ui::OptionDialog *ui;
};

#endif // OPTIONDIALOG_H
