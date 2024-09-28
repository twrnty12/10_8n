#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{

    dataBase = new QSqlDatabase();
    modelQuery= new QSqlQueryModel(this);
}

DataBase::~DataBase()
{
    delete dataBase;
}


void DataBase::AddDataBase(QString driver, QString nameDB)
{
    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);
    modelTable= new QSqlTableModel(this, *dataBase);

}


void DataBase::ConnectToDataBase(QVector<QString> data)
{

    dataBase->setHostName(data[hostName]);
    dataBase->setDatabaseName(data[dbName]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[pass]);
    dataBase->setPort(data[port].toInt());




    bool status;
    status = dataBase->open( );
    emit sig_SendStatusConnection(status);

}

void DataBase::DisconnectFromDataBase(QString nameDb)
{

    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();

}

void DataBase::RequestToDB(QString request)
{
    ///Тут должен быть код ДЗ
    if(request == "*")
    {
        modelTable->setTable("film");
        modelTable->select();
        modelTable->setHeaderData(1, Qt::Horizontal, QObject::tr("Название фильма"));
        modelTable->setHeaderData(2, Qt::Horizontal, QObject::tr("Описание фильма"));
        emit sig_SendTableFromDB(modelTable);
    }
    else
    {
        modelQuery->setQuery(request, *dataBase);
        modelQuery->setHeaderData(0, Qt::Horizontal, QObject::tr("Название фильма"));
        modelQuery->setHeaderData(1, Qt::Horizontal, QObject::tr("Описание фильма"));
        emit sig_SendQueryFromDB(modelQuery);
    }
}


QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
