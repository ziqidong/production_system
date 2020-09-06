#include "infer.h"

infer::infer(){
    QFile file(":/data/plant.txt");
//    QFile file(":/data/animal.txt"); //可更换规则库
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
    }
    while(!file.atEnd())
    {
        QByteArray line = file.readLine();
        QString str(line);
        rawdata.push_back(str.trimmed()); //trimmed()去掉换行符
    }
    extractFact();
    extractRules();
}

void infer::extractFact(){ //从rawdata里提取事实存入QVector<fact> v
    QVector<QString>::iterator ie;
    for (ie=rawdata.begin();ie!=rawdata.end();ie++){
        QVector<QString>* processed = splitStr(*ie,"->");
        QVector<QString>::iterator ite;
        for (ite=processed->begin();ite!=processed -> end();ite++)
        {
            QVector<QString>* processed = splitStr(*ite,"^");
            QVector<QString>::iterator ite1;
            for (ite1=processed->begin();ite1!=processed -> end();ite1++)
            {
                if(!indb(*ite1))    {
                    fact a = {false,*ite1};
                    facts.push_back(a);
                    statements.push_back(*ite1);
                }
            }
        }
    }
}

void infer::extractRules(){ //从rawdata中提取推理规则存入QVector<rules> r
    QVector<QString>::iterator ie;
    for (ie=rawdata.begin();ie!=rawdata.end();ie++){
        QVector<QString>* processed = splitStr(*ie,"->");
        QVector<QString>::iterator ite;
        ite=processed->begin();
        QVector<QString>* processed1 = splitStr(*ite,"^");

        Rules* tmp;
        tmp = new Rules();
        tmp->conclusion=*(processed->end()-1);
        QVector<QString>::iterator itee;
        for (itee=processed1->begin();itee!=processed1->end();itee++){
            tmp->conditions.push_back(*itee);
        }
        rules.push_back(*tmp); //注意是Rules型不是Rules指针型
    }
};

void infer::reasoning(){ //核心推理过程，遍历QVector<Rules> rules中所有规则进行推理
    QVector<QString>* res = new QVector<QString>;
    bool change=true;
    while (change){
        change=false;

        QVector<Rules>::iterator ite;
        for (ite=rules.begin();ite!=rules.end();ite++) //逐个检查每个条件是否满足
        {
            Rules tmp = *ite;
            bool flag=true;
            QVector<QString>::iterator ite1;
            for (ite1=tmp.conditions.begin();ite1!=tmp.conditions.end();ite1++) //逐个检查每个条件的所有conditions是否满足
            {
                if(flag){
                    QVector<fact>::iterator ite2;
                    for (ite2=facts.begin();ite2!=facts.end();ite2++)
                    {
                        if((*ite2).word == *ite1 && (*ite2).state == false){ //state值为false表示该事实不是已知事实，则该条规则匹配失败，break
                            flag = false;
                            break;
                        }
                    }
                }else{break;}
            }
            if(flag){

                bool flag1=true; //判断被推理出的规则是否产生的新的右部，有则添加进result，没有则不添加
                QVector<QString>::iterator it;
                for (it=res->begin();it!=res->end();it++){
                    if(*it == tmp.conclusion){
                        flag1=false;
                        break;
                    }
                }
                if(flag1) res->push_back(tmp.conclusion);

                if(flag1){ //若有新的右部，则将新的右部加入已知条件，并将change置为true准备下一次循环
                    change = true;
                    QVector<fact>::iterator ite3;
                    for (ite3=facts.begin();ite3!=facts.end();ite3++)
                    {
                        if((*ite3).word == tmp.conclusion){
                            (*ite3).state = true;
                        }
                    }
                }
            }
        }
    }
    QVector<QString>::iterator it; //将推理出的所有结论连接成语句存入QString result
    for (it=res->begin();it!=res->end();it++){
        result += *it + "\n";
    }
    delete res;
}

QVector<QString>* infer::splitStr(QString s,QString b){ //把字符串s按照子串b进行分隔
    QVector<QString>* t = new(QVector<QString>);
    string str = s.toStdString();
    string b1 = b.toStdString();
    char *cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());
    char *p = strtok(cstr, b1.c_str()); //c_str() string转char
    while (p != 0)
    {
        string a = p;
        t->push_back(QString::fromStdString(a));
        p = strtok(NULL, b1.c_str());
    }
    return t;
}

bool infer::indb(QString s){ //判断一条语句是否已经被加入QVector<QString> statements中
    QVector<QString>::iterator ite1;
    for (ite1=statements.begin();ite1!=statements.end();ite1++)
    {
        if((*ite1) == s){
            return true;
        }
    }
    return false;
}
