#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAbstractProxyModel>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QFileDialog>
#include <QDir>
#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include <QtPrintSupport/QPrintDialog>
#include <QScrollBar>

#include "tableprinter.h"
#include "racersloader.h"
#include "resultsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_startList = new RacerModel(DEFAULT_RACERS_COLUMNS, this);

    QMap<QString, int> resultColums = DEFAULT_RACERS_COLUMNS;
    resultColums.insert("Cas", 7);
    resultColums.insert("Ztrata na viteze", 8);
    resultColums.insert("Poradi v kategorii", 9);
    resultColums.insert("Celkove poradi", 10);
    m_resultList = new RacerModel(resultColums, this);

    m_filterStartList = new QSortFilterProxyModel(this);
    m_filterResultList = new QSortFilterProxyModel(this);
    m_exportResultList = new QSortFilterProxyModel(this);

    m_filterStartList->setSourceModel(m_startList);
    m_filterResultList->setSourceModel(m_resultList);
    m_exportResultList->setSourceModel(m_resultList);

    ui->lbl_time->setText(QTime().currentTime().toString("HH:mm:ss"));

    m_timer = new QTimer();
    m_timer->setInterval(1000);

    connect(m_timer, &QTimer::timeout, [this](){
        ui->lbl_time->setText(QTime().currentTime().toString("HH:mm:ss"));
        if (ui->te_startTime->time() != QTime(0, 0, 0)) {
            ui->lbl_current10->setText(QTime::fromMSecsSinceStartOfDay(ui->te_startTime->time().msecsTo(QTime::currentTime())).toString("HH:mm:ss"));
        }

        if (ui->te_startTime4->time() != QTime(0, 0, 0)) {
            ui->lbl_current4->setText(QTime::fromMSecsSinceStartOfDay(ui->te_startTime4->time().msecsTo(QTime::currentTime())).toString("HH:mm:ss"));
        }

        if (ui->te_startTimeChildren->time() != QTime(0, 0, 0)) {
            ui->lbl_currentChildren->setText(QTime::fromMSecsSinceStartOfDay(ui->te_startTimeChildren->time().msecsTo(QTime::currentTime())).toString("HH:mm:ss"));
        }
    });
    m_timer->start();

    //m_filterResultList->sort(6, Qt::AscendingOrder);
    ui->tv_startList->setModel(m_filterStartList);
    ui->tv_results->setModel(m_filterResultList);

    QHeaderView *startHeader = ui->tv_startList->horizontalHeader();
    connect(startHeader, &QHeaderView::sectionDoubleClicked, [this](int logicalIndex) {
        Qt::SortOrder order = Qt::SortOrder::AscendingOrder;
        if (m_filterStartList->sortColumn() == logicalIndex) {
            order = (m_filterStartList->sortOrder() == Qt::SortOrder::AscendingOrder) ? Qt::SortOrder::DescendingOrder : Qt::SortOrder::AscendingOrder;
        }
        m_filterStartList->sort(logicalIndex, order);
    });

    QHeaderView *header = ui->tv_results->horizontalHeader();
    connect(header, &QHeaderView::sectionDoubleClicked, [this](int logicalIndex) {
        Qt::SortOrder order = Qt::SortOrder::AscendingOrder;
        if (m_filterResultList->sortColumn() == logicalIndex) {
            order = (m_filterResultList->sortOrder() == Qt::SortOrder::AscendingOrder) ? Qt::SortOrder::DescendingOrder : Qt::SortOrder::AscendingOrder;
        }
        m_filterResultList->sort(logicalIndex, order);
    });

    m_resultsDialog = new ResultsDialog(m_filterResultList, this);
    m_resultsDialog->show();
}

MainWindow::~MainWindow()
{
    delete ui;

    if (m_timer) {
        delete m_timer;
        m_timer = 0;
    }

    if (m_startList) {
        delete m_startList;
        delete m_filterStartList;
        m_startList = 0;
        m_filterStartList = 0;
    }

    if (m_resultList) {
        delete m_resultList;
        delete m_filterResultList;
        delete m_exportResultList;
        m_resultList = 0;
        m_filterResultList = 0;
        m_exportResultList = nullptr;
    }

    if (m_resultsDialog) {
        delete m_resultsDialog;
        m_resultsDialog = 0;
    }
}

void MainWindow::printModel(QAbstractProxyModel *pModel, QList<int> excludedColumns)
{
    QPrinter printer;
    // printer setup

    QPrintDialog dialog(&printer, this);
    dialog.setWindowTitle(tr("Print Document"));
    if (dialog.exec() == QDialog::Accepted) {
        QPainter painter;
        painter.begin(&printer);
        TablePrinter tablePrinter(&painter, &printer);
        QVector<int> stretch;
        QVector<QString> headers;
        for (int i = 0; i < pModel->columnCount(QModelIndex()); i++) {
            if (i == 0) {
                stretch.append(2);
            } else {
                stretch.append(5);
            }
            headers.append(pModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString());
        }
        QVector<int> columnStretch = stretch;
        tablePrinter.printTable(pModel, columnStretch, headers, excludedColumns);
        painter.end();
    }
}

void MainWindow::on_pb_start_clicked()
{
    ui->te_startTime->setTime(QTime().currentTime());
}

void MainWindow::on_le_runnerID_returnPressed()
{
    short racerID = ui->le_runnerID->text().toInt();
    Racer racer = m_startList->findRacer(racerID);

    if (racer.startNumber() == racerID && m_resultList->findRacer(racerID).startNumber() == -1) {
        QTime time;
        if (racer.category().endsWith("10")) {

            time = QTime::fromMSecsSinceStartOfDay(ui->te_startTime->time().msecsTo(QTime::currentTime()));
        } else if (racer.category().endsWith("4")) {
            time = QTime::fromMSecsSinceStartOfDay(ui->te_startTime4->time().msecsTo(QTime::currentTime()));
        } else {
            time = QTime::fromMSecsSinceStartOfDay(ui->te_startTimeChildren->time().msecsTo(QTime::currentTime()));
        }

        if (!m_winnersTime.contains(racer.category())) {
            m_winnersTime.insert(racer.category(), time);
        }

        racer.setTrackRank(m_lastTrackRank[racer.track()]);
        racer.setCategoryRank(m_lastCategoryRank[racer.category()]);
        racer.setTime(time);
        racer.setTimeByWinner(QTime::fromMSecsSinceStartOfDay(m_winnersTime[racer.category()].msecsTo(time)));

        m_lastTrackRank[racer.track()]++;
        m_lastCategoryRank[racer.category()]++;
        m_resultList->addRacer(racer);

        QApplication::processEvents();
        ui->tv_results->verticalScrollBar()->setValue(ui->tv_results->verticalScrollBar()->maximum());

        RacersLoader loader;
        QString dbFilePath = QString("%1/racers-%2.csv").arg(QDir::homePath()).arg(ui->te_startTime->time().toString("HH-mm-ss"));
        loader.storeRacers(dbFilePath, m_resultList->racers());
    }

    ui->le_runnerID->clear();
}

void MainWindow::on_pb_readFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open racers list"), QDir::homePath(),
                                                    tr("CSV file (*.csv)"));

    if (!fileName.isEmpty()) {
        RacersLoader loader;
        QList<Racer> racers = loader.loadRacers(fileName);

        for (Racer racer : racers) {
            if (!m_lastCategoryRank.contains(racer.category())) {
                m_lastCategoryRank.insert(racer.category(), 1);
            }
            if (!m_lastTrackRank.contains(racer.track())) {
                m_lastTrackRank.insert(racer.track(), 1);
            }
        }
        m_startList->addRacers(racers);
    }
}

void MainWindow::on_pb_loadResults_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open results list"), QDir::homePath(),
                                                    tr("CSV file (*.csv)"));

    if (!fileName.isEmpty()) {
        RacersLoader loader;
        QList<Racer> racers = loader.loadRacers(fileName);

        m_resultList->addRacers(racers);
    }
}

void MainWindow::on_pb_printStartList_clicked()
{
    printModel(m_filterStartList);
}

void MainWindow::on_pb_printResults_clicked()
{
    printModel(m_filterResultList);
}

void MainWindow::on_pb_start4_clicked()
{
    ui->te_startTime4->setTime(QTime().currentTime());
}

void MainWindow::on_pb_startChildren_clicked()
{
    ui->te_startTimeChildren->setTime(QTime().currentTime());
}

void MainWindow::on_action10_km_triggered()
{
    auto columns = m_resultList->columns();
    QList<int> excludedColumns;
    excludedColumns.append(columns["Ztrata na viteze"]);
    excludedColumns.append(columns["Poradi v kategorii"]);
    m_exportResultList->setFilterKeyColumn(columns["Trat"]);
    m_exportResultList->setFilterFixedString("10 Km");
    m_exportResultList->sort(columns["Celkove poradi"],
                             Qt::SortOrder::AscendingOrder);

    printModel(m_exportResultList, excludedColumns);
}

void MainWindow::on_action4_km_triggered()
{
    auto columns = m_resultList->columns();
    QList<int> excludedColumns;
    excludedColumns.append(columns["Ztrata na viteze"]);
    excludedColumns.append(columns["Poradi v kategorii"]);
    m_exportResultList->setFilterKeyColumn(columns["Trat"]);
    m_exportResultList->setFilterFixedString("4 Km");
    m_exportResultList->sort(columns["Celkove poradi"],
                             Qt::SortOrder::AscendingOrder);

    printModel(m_exportResultList, excludedColumns);
}
