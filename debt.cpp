#include "debt.h"
#include "ui_debt.h"

Debt::Debt(QString number,QString time,int flag,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Debt),number(number),time(time),flag(flag)
{
    ui->setupUi(this);

    db = QSqlDatabase::database("conn");
    query=new QSqlQuery(db);

    ui->checkBox->setChecked(true);
    ui->dateEdit->setEnabled(false);
    ui->dateEdit->setDate(QDate::currentDate());

    ui->comboBox_subject->setCurrentIndex(-1);
    ui->comboBox_type->setCurrentIndex(-1);
    ui->comboBox_semester->setCurrentIndex(-1);
    ui->comboBox_year->setCurrentIndex(-1);

    connect(ui->pushButton_add1,&QPushButton::clicked,this,&Debt::clickPushButton_add1);
    connect(ui->pushButton_add2,&QPushButton::clicked,this,&Debt::clickPushButton_add2);
    connect(ui->pushButton_add3,&QPushButton::clicked,this,&Debt::clickPushButton_add3);
    connect(ui->pushButton_add4,&QPushButton::clicked,this,&Debt::clickPushButton_add3);
    connect(ui->pushButton,&QPushButton::clicked,this,&Debt::clickPushButton);
    connect(ui->checkBox,&QCheckBox::stateChanged,this,[this](){
        if(ui->checkBox->isChecked())
            ui->dateEdit->setEnabled(false);
        else
            ui->dateEdit->setEnabled(true);
    });

    updataDataDictionary();

    if(flag==1) {
        if(query->exec(QString("SELECT * FROM debt WHERE time = '%1'").arg(time))) {
            query->first();
            ui->comboBox_subject->setCurrentText(query->value(1).toString());
            ui->lineEdit_grade->setText(query->value(2).toString());
            ui->comboBox_type->setCurrentText(query->value(3).toString());
            if(query->value(4).toString()!="") {
                ui->checkBox->setChecked(false);
                ui->dateEdit->setDate(QDate::fromString(query->value(4).toString(),"d/M/yyyy"));
            }
            ui->comboBox_semester->setCurrentText(query->value(5).toString());
            ui->comboBox_year->setCurrentText(query->value(6).toString());;
        }
    }
}

Debt::~Debt() { delete ui; }

void Debt::clickPushButton() {
    QString data;
    if(ui->checkBox->isChecked())
        data="";
    else
        data=ui->dateEdit->text();
    QString str;
    if(flag==0) {
        str=QString("INSERT INTO `debt`(`number`, `subject`, `grade`,`type`, `date`, `semester`, `year`, `time`) VALUES "
                    "('%1','%2','%3','%4','%5','%6','%7','%8')").arg(number,ui->comboBox_subject->currentText(),ui->lineEdit_grade->text(),ui->comboBox_type->currentText(),
                                                          data,ui->comboBox_semester->currentText(),ui->comboBox_year->currentText(),
                                                                QDateTime::currentDateTime().toString());
    } else {
        str=QString("UPDATE debt set subject='%1',grade='%2',type='%3',date='%4',semester='%5',year='%6' WHERE "
                    "time='%7'").arg(ui->comboBox_subject->currentText(),ui->lineEdit_grade->text(),ui->comboBox_type->currentText(),
                                                          data,ui->comboBox_semester->currentText(),ui->comboBox_year->currentText(),time);
    }

    if(query->exec(str)) {
        emit needUpdataData();
        delete this;
    } else {
        qDebug()<<str;
        QMessageBox::information(this,"提示","保存失败。");
    }
}

void Debt::updataDataDictionary() {
    if(query->exec("select name from subject")) {
        int index=ui->comboBox_subject->currentIndex();
        ui->comboBox_subject->clear();
        while (query->next()) {
            QString name = query->value("name").toString();
            if(!name.isEmpty()) {
                ui->comboBox_subject->addItem(name);
            }
        }
        ui->comboBox_subject->setCurrentIndex(index);
    }
    if(query->exec("select name from typeofexam")) {
        int index=ui->comboBox_type->currentIndex();
        ui->comboBox_type->clear();
        while (query->next()) {
            QString name = query->value("name").toString();
            if(!name.isEmpty()) {
                ui->comboBox_type->addItem(name);
            }
        }
        ui->comboBox_type->setCurrentIndex(index);
    }
    if(query->exec("select name from semester")) {
        int index=ui->comboBox_semester->currentIndex();
        ui->comboBox_semester->clear();
        while (query->next()) {
            QString name = query->value("name").toString();
            if(!name.isEmpty()) {
                ui->comboBox_semester->addItem(name);
            }
        }
        ui->comboBox_semester->setCurrentIndex(index);
    }
    if(query->exec("select name from year")) {
        int index=ui->comboBox_year->currentIndex();
        ui->comboBox_year->clear();
        while (query->next()) {
            QString name = query->value("name").toString();
            if(!name.isEmpty()) {
                ui->comboBox_year->addItem(name);
            }
        }
        ui->comboBox_year->setCurrentIndex(index);
    }
}

void Debt::clickPushButton_add1() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("添加科目"),tr("名称:"), QLineEdit::Normal,"", &ok);
    if (ok && !text.isEmpty()) {
        if(query->exec(QString("INSERT INTO `subject` (`name`) VALUES ('%1')").arg(text))) {
            qDebug()<<tr("Debt:添加科目成功。");
        }
        updataDataDictionary();
    }
}

void Debt::clickPushButton_add2() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("添加类型"),tr("名称:"), QLineEdit::Normal,"", &ok);
    if (ok && !text.isEmpty()) {
        if(query->exec(QString("INSERT INTO `typeofexam` (`name`) VALUES ('%1')").arg(text))) {
            qDebug()<<tr("Debt:添加类型成功。");
        }
        updataDataDictionary();
    }
}

void Debt::clickPushButton_add3() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("添加学期"),tr("学期:"), QLineEdit::Normal,"", &ok);
    if (ok && !text.isEmpty()) {
        if(query->exec(QString("INSERT INTO `semester` (`name`) VALUES ('%1')").arg(text))) {
            qDebug()<<tr("Debt:添加学期成功。");
        }
        updataDataDictionary();
    }
}

void Debt::clickPushButton_add4() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("添加年份"),tr("年份:"), QLineEdit::Normal,"", &ok);
    if (ok && !text.isEmpty()) {
        if(query->exec(QString("INSERT INTO `year` (`name`) VALUES ('%1')").arg(text))) {
            qDebug()<<tr("Debt:添加年份成功。");
        }
        updataDataDictionary();
    }
}
