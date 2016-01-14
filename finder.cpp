#include "finder.h"
#include "ui_finder.h"
#include "listdelegate.h"
#include <QtDebug>

using std::string;
using std::vector;

Finder::Finder(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Finder)
{
    ui->setupUi(this);
    toggleDirInput(false);

    InvertedIndex index;
    this->inv_ind = index;
    init();
}

Finder::~Finder() {
    delete ui;
}

void Finder::init() {
    string dir = "/Users/Joyce/Documents/TextFiles/";
    setNewDir(dir);
}

void Finder::setNewDir(string dir) {
    if (!dir.empty()) { //if not empty string, clear index, add specified dir
        this->inv_ind.clearIndex();
        this->inv_ind.addDir(dir);

//        vector<string> allFiles = inv_ind.getFileList();
//        for (string file : allFiles)
//            qDebug() << QString::fromStdString(file);
//        if (allFiles.empty()) {
//            qDebug() << "No files";
//        } else {
//            qDebug() << "Printed some files!";
//        }

        QLabel *dirDisplay = ui->directoryLabel;
        dirDisplay->setText(QString::fromStdString("Searching: " + dir));
    } else {
        QLabel *dirDisplay = ui->directoryLabel;
        dirDisplay->setText("Searching: " + QString::fromStdString(inv_ind.getBaseDir()));
    }
    toggleDirInput(false); //show label
}


void Finder::toggleDirInput(bool show) {
    if (show) {
        ui->directoryLabel->hide();
        ui->inputDir->show();
        ui->inputDir->setText(QString::fromStdString(inv_ind.getBaseDir()));
    } else {
        ui->directoryLabel->show();
        ui->inputDir->hide();
    }
}

void Finder::search() {
    QString input = ui->searchBar->text();
    QListWidget *resultList = ui->resultList;

    resultList->clear();
    ui->searchBar->setText("");
    resultList->setItemDelegate(new ListDelegate(resultList));


    //test
    /*for (int i = 0; i < input.length()/2; i++) {
        resultList->setItemDelegate(new ListDelegate(resultList));
        QString titleTest = QString::fromStdString("Test test test");
        QListWidgetItem *item = new QListWidgetItem();
        item->setData(Qt::DisplayRole, titleTest);

        string display = "Sample sample sample sample sample sample";
        item->setData(Qt::UserRole, QString::fromStdString(display));
        resultList->addItem(item);
    }*/

    vector<InvertedIndex::Result> results = this->inv_ind.query(input.toStdString());

    if (results.empty()) {
        QListWidgetItem *item = new QListWidgetItem();
        item->setData(Qt::WhatsThisRole, "AlertNone"); //no docs found
        resultList->addItem(item);
    } else {
        for (InvertedIndex::Result res : results) {
            QString title = QString::fromStdString(res.fileName);
            QListWidgetItem *item = new QListWidgetItem();
            item->setData(Qt::DisplayRole, title);

            string display = "...";
            for (int i = 0; i < res.previews.size(); i++) {
                std::pair<string, string> prev = res.previews[i];
                string text = prev.second;
                display += text + "...";
            }
            item->setData(Qt::UserRole, QString::fromStdString(display));

            resultList->addItem(item);
        }
    }
}

void Finder::on_searchButton_clicked() {
    search();
}

void Finder::on_searchBar_returnPressed() {
    search();
}

void Finder::on_changeDirButton_clicked() {
    QLineEdit* input = ui->inputDir;

    if (input->isVisible())         // if input is already showing, set input
        setNewDir((input->text()).toStdString());
    else                            // else show input
        toggleDirInput(true);
}

void Finder::on_inputDir_returnPressed() {
    QString dir = ui->inputDir->text();
    setNewDir((dir).toStdString());
}
