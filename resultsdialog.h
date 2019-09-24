#ifndef RESULTSDIALOG_H
#define RESULTSDIALOG_H

#include <QDialog>
#include <QAbstractItemModel>

namespace Ui {
class ResultsDialog;
}

class ResultsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultsDialog(QAbstractItemModel *pModel, QWidget *parent = 0);
    ~ResultsDialog();

private:
    Ui::ResultsDialog *ui;
};

#endif // RESULTSDIALOG_H
