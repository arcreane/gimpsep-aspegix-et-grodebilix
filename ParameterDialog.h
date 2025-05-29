#ifndef PARAMETERDIALOG_H
#define PARAMETERDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class ParameterDialog; }
QT_END_NAMESPACE

class ParameterDialog : public QDialog {
    Q_OBJECT
public:
    explicit ParameterDialog(QWidget *parent = nullptr);
    ~ParameterDialog();

    int getValue() const;

private:
    Ui::ParameterDialog *ui;
};

#endif // PARAMETERDIALOG_H
