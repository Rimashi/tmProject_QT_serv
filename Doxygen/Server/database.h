#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <QJsonObject>

/**
 * @brief Класс для работы с SQLite базой данных пользователей и результатов тестов.
 * 
 * Реализован как синглтон — используется метод instance().
 */
class Database : public QObject {
    Q_OBJECT
public:

    /**
     * @brief Получить экземпляр класса Database (синглтон).
     * @return Ссылка на единственный экземпляр Database.
     */
    static Database& instance();

    /**
     * @brief Открывает подключение к базе данных по указанному пути.
     * @param path Путь к SQLite-файлу.
     * @return true если успешно, false в случае ошибки.
     */
    bool open(const QString& path);

    /**
     * @brief Закрывает соединение с базой данных.
     */
    void close();

    /**
     * @brief Проверяет, существует ли пользователь с данным логином.
     * @param login Логин пользователя.
     * @return true если пользователь существует, иначе false.
     */
    bool userExists(const QString& login);

    /**
     * @brief Регистрирует нового пользователя в базе данных.
     * @param login Логин.
     * @param passwordHash Хэш пароля (SHA-384).
     * @param name Имя.
     * @param surname Фамилия.
     * @return true если регистрация прошла успешно, иначе false.
     */
    bool registerUser(const QString& login, const QString& passwordHash, const QString& name, const QString& surname);

    /**
     * @brief Проверяет корректность логина и пароля.
     * @param login Логин.
     * @param passwordHash Хэш пароля.
     * @param userData Объект JSON, куда будут записаны имя, фамилия и роль при успехе.
     * @return true если пользователь найден и пароль верен, иначе false.
     */
    bool validateUser(const QString& login, const QString& passwordHash, QJsonObject &userData);

    /**
     * @brief Инициализирует администратора, если он отсутствует.
     * 
     * Логин: admin, пароль: admin123, имя и фамилия: Admin.
     */
    void initializeAdmin();

    /**
     * @brief Сохраняет результат теста в базу данных.
     * @param login Логин студента.
     * @param testNumber Номер теста.
     * @param score Результат в процентах (0–100).
     * @return true если сохранено успешно, иначе false.
     */
    bool saveTestResult(const QString& login, int testNumber, int score);

    /**
     * @brief Получает полную статистику студента.
     * @param login Логин студента.
     * @return JSON-объект с ФИО и массивом результатов тестов.
     */
    QJsonObject getStudentStatistics(const QString& login);

    /**
     * @brief Возвращает список всех студентов в системе (без админа).
     * @return JSON-объект с массивом студентов.
     */
    QJsonObject getStudents();

    /**
     * @brief Получает общую статистику по всем тестам всех студентов.
     * @return JSON-объект с массивом всех попыток тестов.
     */
    QJsonObject getTestStatistics();

    /**
     * @brief Возвращает текст последней ошибки.
     * @return Строка с описанием ошибки.
     */
    QString lastError() const;

private:
    /**
     * @brief Приватный конструктор для реализации синглтона.
     */
    explicit Database(QObject *parent = nullptr);

    /**
     * @brief Деструктор — закрывает базу данных.
     */
    ~Database();

    QSqlDatabase m_db;          ///< Объект подключения к базе данных.
    QString m_lastError;        ///< Последнее сообщение об ошибке.

    // Запрещаем копирование
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
};

#endif // DATABASE_H
