#include <QScrollBar>

#include "resultsdialog.h"
#include "ui_resultsdialog.h"
#include <QDebug>

ResultsDialog::ResultsDialog(QAbstractItemModel *pModel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultsDialog)
{
    ui->setupUi(this);
    if (pModel) {
        ui->tv_results->setModel(pModel);
        connect(ui->tv_results->verticalScrollBar(), &QScrollBar::rangeChanged, [this] (int min, int max){
            Q_UNUSED(min)
            Q_UNUSED(max)
            ui->tv_results->verticalScrollBar()->setValue(ui->tv_results->verticalScrollBar()->maximum());
        });

    }
}

ResultsDialog::~ResultsDialog()
{
    delete ui;
}
