#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->lb_statusConnect->setStyleSheet("color:red");
    ui->pb_request->setEnabled(false);



    dataDb = new DbData(this);
    dataBase = new DataBase(this);
    msg = new QMessageBox(this);


    dataForConnect.resize(NUM_DATA_FOR_CONNECT_TO_DB);

    
    //Добавим БД

    dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);


     // Устанавливаем данные для подключениея к БД.


    connect(dataDb, &DbData::sig_sendData, this, [&](QVector<QString> receivData){
        dataForConnect = receivData;
    });


    connect(dataBase, &::DataBase::sig_SendQueryFromDB, this, &MainWindow::ScreenQueryFromDB);
    connect(dataBase, &::DataBase::sig_SendTableFromDB, this, &MainWindow::ScreenTableFromDB);
    connect(dataBase, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_act_addData_triggered()
{
  
    dataDb->show();
}


void MainWindow::on_act_connect_triggered()
{

    if(ui->lb_statusConnect->text() == "Отключено"){

        ui->lb_statusConnect->setText("Подключение");
        ui->lb_statusConnect->setStyleSheet("color : black");


        auto conn = [&]{dataBase->ConnectToDataBase(dataForConnect);};
        QtConcurrent::run(conn);

    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        ui->lb_statusConnect->setText("Отключено");
        ui->act_connect->setText("Подключиться");
        ui->lb_statusConnect->setStyleSheet("color:red");
        ui->pb_request->setEnabled(false);
    }

}


 // "Получить"
 
void MainWindow::on_pb_request_clicked()
{
    QString category;
    category = ui->cb_category->currentText();
    ///Тут должен быть код ДЗ
    if (category == "Комедия"){
        request = "SELECT title, description FROM film f JOIN film_category "
                  "fc on f.film_id = fc.film_id JOIN category c"
                  " on c.category_id = fc.category_id WHERE c.name = 'Comedy' ";
    }
    else if (category == "Ужасы"){
        request = "SELECT title, description FROM film f JOIN film_category "
                  "fc on f.film_id = fc.film_id JOIN category c"
                  " on c.category_id = fc.category_id WHERE c.name = 'Horror' ";
    }
    else
    {
        request = "*";
    }

    auto req = [&]{dataBase->RequestToDB(request);};
    QtConcurrent::run(req);


}


void MainWindow::ScreenQueryFromDB(QSqlQueryModel *model)
{
    ui->tb_result->setModel(model);
  
}

void MainWindow::ScreenTableFromDB(QSqlTableModel *model)
{
    ui->tb_result->setModel(model);
  
}

void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if(status){
        ui->act_connect->setText("Отключиться");
        ui->lb_statusConnect->setText("Подключено к БД");
        ui->lb_statusConnect->setStyleSheet("color:green");
        ui->pb_request->setEnabled(true);
    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(dataBase->GetLastError().text());
        ui->lb_statusConnect->setText("Отключено");
        ui->lb_statusConnect->setStyleSheet("color:red");
        msg->exec();
    }

}




void MainWindow::on_pb_clear_clicked()
{
    ui->tb_result->setModel(NULL);
}

