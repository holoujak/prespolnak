#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QMap>
#include "racermodel.h"
#include "resultsdialog.h"

namespace Ui {
class MainWindow;
}

class QTimer;
class QTime;
class QAbstractProxyModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void printModel(QAbstractProxyModel *pModel, QList<int> excludedColumns = QList<int>());
    void on_category_export_triggered(const QString category);
    void on_track_export_triggered(const QString track);

private slots:
    void on_pb_start_clicked();

    void on_le_runnerID_returnPressed();

    void on_pb_readFile_clicked();

    void on_pb_loadResults_clicked();

    void on_pb_printStartList_clicked();

    void on_pb_printResults_clicked();

    void on_pb_start4_clicked();

    void on_pb_startChildren_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *m_timer;
    RacerModel *m_startList;
    RacerModel *m_resultList;
    QSortFilterProxyModel *m_filterStartList;
    QSortFilterProxyModel *m_filterResultList;
    QSortFilterProxyModel *m_exportResultList;
    ResultsDialog *m_resultsDialog;

    QMap<QString, QTime> m_winnersTime;
    QMap<QString, short> m_lastTrackRank;
    QMap<QString, short> m_lastCategoryRank;
};

#endif // MAINWINDOW_H
