#include "finder.h"
#include "ui_finder.h"
#include "listdelegate.h"

using std::string;
using std::vector;

Finder::Finder(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Finder)
{
    ui->setupUi(this);

    InvertedIndex index;
    this->inv_ind = index;
    this->inv_ind.addDir("/Users/Joyce/Documents/TextFiles/");
}

Finder::~Finder()
{
    delete ui;
}

void Finder::on_searchButton_clicked() {
    search();
}

void Finder::on_searchBar_returnPressed() {
    search();
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

