#ifndef INFER_H
#define INFER_H
#include <QString>
#include <QVector>
#include <QDebug>
#include <cstring>
#include <string>
#include <QFile>
#include <iostream>
using namespace std;

struct fact
{
    bool state;
    QString word;
};

typedef struct rs{
    QString conclusion;
    QVector<QString> conditions;
}Rules;

class infer{
private:
public:
    QVector<QString> rawdata;
    QVector<fact> facts; //存放事实及其状态，如：{有毛发,true},{是哺乳动物,false}
    QVector<Rules> rules; //存放规则，如：{{有犬齿,有爪,眼盯前方},是食肉动物}
    QVector<QString> statements; //包含全部事实，用于显示在窗体左边"事实库"list中
    QString result; //最终结果
    infer();
    void extractFact();
    void extractRules();
    void reasoning();
    QVector<QString>* splitStr(QString s, QString b);
    bool indb(QString s);
};

#endif // INFER_H
