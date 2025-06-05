#ifndef SERVER_API_H
#define SERVER_API_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonObject>
#include <QSqlDatabase>
#include <QMap>

class Func2Serv;

/**
 * @brief Класс TCP-сервера для обработки клиентских подключений и работы с БД.
 * 
 * Обрабатывает входящие подключения, обмен ключами шифрования (RSA + AES),
 * а также маршрутизирует действия клиента через Func2Serv.
 */
class Server_api : public QTcpServer {
    Q_OBJECT
public:
    /**
     * @brief Конструктор сервера.
     * @param parent Родительский объект.
     */
    explicit Server_api(QObject *parent = nullptr);

    /**
     * @brief Добавляет AES-ключ, связанный с определённым сокетом клиента.
     * @param socket Сокет клиента.
     * @param key AES-ключ в виде байтового массива.
     */
    void addAesKey(QTcpSocket* socket, const QByteArray& key);

protected:
    /**
     * @brief Обработка нового входящего подключения.
     * @param socketDescriptor Дескриптор нового сокета.
     */
    void incomingConnection(qintptr socketDescriptor) override;

private:
    /**
     * @brief Настраивает подключение к базе данных SQLite.
     * Создаёт таблицы, если их ещё нет.
     */
    void setupDatabase();

    QSqlDatabase db;                             ///< Объект базы данных.
    Func2Serv* func2Serv;                        ///< Указатель на обработчик функционала.
    QMap<QTcpSocket*, QByteArray> clientAesKeys; ///< Хранилище AES-ключей для клиентов.
};

#endif // SERVER_API_H
