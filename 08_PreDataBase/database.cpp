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

/*!
 * \brief Метод добавляет БД к экземпляру класса QSqlDataBase
 * \param driver драйвер БД
 * \param nameDB имя БД (Если отсутствует Qt задает имя по умолчанию)
 */
void DataBase::AddDataBase(QString driver, QString nameDB)
{
    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);
    modelTable= new QSqlTableModel(this, *dataBase);

}

/*!
 * \brief Метод подключается к БД
 * \param для удобства передаем контейнер с данными необходимыми для подключения
 * \return возвращает тип ошибки
 */
void DataBase::ConnectToDataBase(QVector<QString> data)
{

    dataBase->setHostName(data[hostName]);
    dataBase->setDatabaseName(data[dbName]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[pass]);
    dataBase->setPort(data[port].toInt());

    ///Тут должен быть код ДЗ


    bool status;
    status = dataBase->open( );
    emit sig_SendStatusConnection(status);

}
/*!
 * \brief Метод производит отключение от БД
 * \param Имя БД
 */
void DataBase::DisconnectFromDataBase(QString nameDb)
{

    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();

}
/*!
 * \brief Метод формирует запрос к БД.
 * \param request - SQL запрос
 * \return
 */
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

/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
