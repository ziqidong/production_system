#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "infer.h"

#include <QMainWindow>
#include <QListWidgetItem>
#include <QStandardItemModel>

#include <QString>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_reasoning_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_clean_clicked();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *ItemModel;
    infer* ii;
};
#endif // MAINWINDOW_H
