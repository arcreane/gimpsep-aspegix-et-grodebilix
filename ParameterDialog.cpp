#include "ParameterDialog.h"
#include "ui_ParameterDialog.h"

ParameterDialog::ParameterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParameterDialog)
{
    ui->setupUi(this);
    ui->spinBox->setRange(0, 255);
    ui->spinBox->setValue(100);  // default value
}

ParameterDialog::~ParameterDialog() {
    delete ui;
}

int ParameterDialog::getValue() const {
    return ui->spinBox->value();
}
