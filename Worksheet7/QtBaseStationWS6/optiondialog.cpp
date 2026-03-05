#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)
{
    ui->setupUi(this);

    ui->spinBoxR->setLocale(QLocale::c());
    ui->spinBoxG->setLocale(QLocale::c());
    ui->spinBoxB->setLocale(QLocale::c());

    ui->spinBoxR->setMinimumWidth(70);
    ui->spinBoxG->setMinimumWidth(70);
    ui->spinBoxB->setMinimumWidth(70);

    auto setup = [](QSpinBox* b){
        b->setEnabled(true);
        b->setReadOnly(false);
        b->setRange(0, 255);
        b->setSingleStep(1);
        b->setValue(0);
        b->setFocusPolicy(Qt::StrongFocus);
    };

    setup(ui->spinBoxR);
    setup(ui->spinBoxG);
    setup(ui->spinBoxB);
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

void OptionDialog::setName(const QString &name)
{
    ui->lineEditName->setText(name);
}

QString OptionDialog::getName() const
{
    return ui->lineEditName->text();
}

void OptionDialog::setPartVisible(bool visible)
{
    ui->checkBoxVisible->setChecked(visible);
}

bool OptionDialog::getPartVisible() const
{
    return ui->checkBoxVisible->isChecked();
}
void OptionDialog::setColour(int r, int g, int b)
{
    ui->spinBoxR->setValue(r);
    ui->spinBoxG->setValue(g);
    ui->spinBoxB->setValue(b);
}

QColor OptionDialog::getColour() const
{
    return QColor(ui->spinBoxR->value(),
                  ui->spinBoxG->value(),
                  ui->spinBoxB->value());
}
