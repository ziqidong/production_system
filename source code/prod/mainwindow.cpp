#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "infer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("植物识别产生式系统"));

    ii = new infer();

    int count = 0;
    QVector<QString>::iterator iter;
    for (iter=ii->statements.begin();iter!=ii->statements.end();iter++)
    {
        count++;
        QListWidgetItem *item = new QListWidgetItem(*iter);
        item -> setCheckState(Qt::Unchecked);
        ui -> listWidget -> addItem(item);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_reasoning_clicked() //点击推理按键后做的事
{
    //初始化 QVector<fact> facts 和 QVector<Rules> rules
    ii->result = "";
    for(int i = 0; i < (ui -> listWidget -> count()); i++){
        if(ui -> listWidget ->item(i) -> checkState() == Qt::Unchecked){
            QVector<fact>::iterator iter;
            for (iter=ii->facts.begin();iter!=ii->facts.end();iter++)
            {
                if((*iter).word == (ui -> listWidget ->item(i) ->text()))   (*iter).state = false;
            }
        }
    }
    //推理
    ii -> reasoning();
    //更新结论框内容
    ui -> listView_2 ->reset();
    ItemModel = new QStandardItemModel(this);
    QStandardItem *item1 = new QStandardItem(ii -> result);
    ItemModel->appendRow(item1);
    ui->listView_2 ->setModel(ItemModel);
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *it) //左边规则库被点击时做下面的事
{
    if (it -> checkState() == Qt::Checked){
        it -> setCheckState(Qt::Unchecked);

        QVector<fact>::iterator iter; //修改事实状态
        for (iter=ii->facts.begin();iter!=ii->facts.end();iter++)
        {
            if((*iter).word == (it ->text()))   (*iter).state = false;
        }
    }else{
        it -> setCheckState(Qt::Checked);

        QVector<fact>::iterator iter; //修改事实状态
        for (iter=ii->facts.begin();iter!=ii->facts.end();iter++)
        {
            if((*iter).word == (it ->text()))   (*iter).state = true;
        }
    }

    ui -> listView ->reset(); //更新已选规则框内容
    ItemModel = new QStandardItemModel(this);
    for(int i = 0; i < (ui -> listWidget -> count()); i++){
        if(ui -> listWidget ->item(i) -> checkState() == Qt::Checked){
            QStandardItem *item1 = new QStandardItem(ui -> listWidget -> item(i) -> text());
            ItemModel->appendRow(item1);
        }
    }
    ui->listView->setModel(ItemModel);
}

void MainWindow::on_clean_clicked()
{
    ii->result = "";//重置规则库
    for(int i = 0; i < (ui -> listWidget -> count()); i++){
        ui -> listWidget ->item(i) -> setCheckState(Qt::Unchecked);
    }
    ui -> listView ->reset(); //重置已选规则框
    ItemModel = new QStandardItemModel(this);
    ui->listView->setModel(ItemModel);
    ui -> listView_2 ->reset(); //重置结论框
    ui->listView_2->setModel(ItemModel);

    //重置infer类中所有结果和所有事实
    ii->result = "";
    QVector<fact>::iterator iter;
    for (iter=ii->facts.begin();iter!=ii->facts.end();iter++)
    {
        (*iter).state = false;
    }
}
