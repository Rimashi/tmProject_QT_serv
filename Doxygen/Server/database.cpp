// database.cpp
#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

/**
 * @brief Конструктор класса Database
 * @param parent Родительский объект
 */
Database::Database(QObject *parent) : QObject(parent) {
    if (QSqlDatabase::contains("test")) {
        m_db = QSqlDatabase::database("test");
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE", "test");
    }
}

/**
 * @brief Деструктор класса Database — закрывает подключение к базе
 */
Database::~Database() {
    close();
}

/**
 * @brief Открывает SQLite-базу данных и инициализирует необходимые таблицы
 * @param path Путь к файлу базы данных
 * @return true при успешном открытии и инициализации, иначе false
 */
bool Database::open(const QString& path) {
    if (m_db.isOpen()) return true;

    m_db.setDatabaseName(path);
    qDebug() << "Opening database:" << path;
    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        qDebug() << "Database: Ошибка открытия БД:" << m_lastError;
        return false;
    }
    qDebug() << "Database opened successfully";


    QSqlQuery query(m_db);
    bool success = query.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "login TEXT UNIQUE NOT NULL,"
        "password TEXT NOT NULL,"
        "name TEXT NOT NULL,"
        "surname TEXT NOT NULL,"
        "role TEXT DEFAULT 'student'"
        ");"
        );

    // Создаем таблицу для результатов тестов
    success = query.exec(
        "CREATE TABLE IF NOT EXISTS test_results ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "login TEXT NOT NULL,"         // Логин студента
        "test_number INTEGER NOT NULL,"     // Номер теста (1, 2, ...)
        "score INTEGER NOT NULL,"           // Результат в процентах (0-100)
        "date DATETIME DEFAULT CURRENT_TIMESTAMP," // Дата прохождения
        "FOREIGN KEY(login) REFERENCES users(login) ON DELETE CASCADE"
        ");"
        );

    if (!success) {
        qCritical() << "Database: Ошибка создания таблицы users:" << query.lastError().text();
        return false;
    }

    // Создаем индекс для ускорения выборки по пользователю
    success = query.exec("CREATE INDEX IF NOT EXISTS idx_test_results_login ON test_results(login)");
    if (!success) {
        qWarning() << "Database: Ошибка создания индекса:" << query.lastError().text();
    }

    return true;
}

/**
 * @brief Закрывает подключение к базе данных
 */
void Database::close() {
    if (m_db.isOpen()) m_db.close();
    // QSqlDatabase::removeDatabase("test");
}

/**
 * @brief Проверяет, существует ли пользователь с заданным логином
 * @param login Логин пользователя
 * @return true, если пользователь существует, иначе false
 */
bool Database::userExists(const QString& login) {
    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) FROM users WHERE login = ?");
    query.addBindValue(login);
    if (!query.exec() || !query.next()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return query.value(0).toInt() > 0;
}

/**
 * @brief Регистрирует нового пользователя
 * @param login Логин
 * @param passwordHash Хэш пароля
 * @param name Имя
 * @param surname Фамилия
 * @return true, если регистрация успешна, иначе false
 */
bool Database::registerUser(const QString& login, const QString& passwordHash, const QString& name, const QString& surname) {
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO users (login, password, name, surname, role) VALUES (?, ?, ?, ?, 'student')");
    query.addBindValue(login);
    query.addBindValue(passwordHash);
    query.addBindValue(name);
    query.addBindValue(surname);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qDebug() << "Database: Ошибка регистрации:" << m_lastError;
        return false;
    }
    return true;
}

/**
 * @brief Проверяет логин и хэш пароля, возвращает данные пользователя
 * @param login Логин
 * @param passwordHash Хэш пароля
 * @param userData Объект, в который записываются данные пользователя
 * @return true, если пользователь найден и пароль совпадает, иначе false
 */
bool Database::validateUser(const QString& login, const QString& passwordHash, QJsonObject &userData) {
    QSqlQuery query(m_db);
    query.prepare("SELECT id, surname, name, role FROM users WHERE login = ? AND password = ?");
    query.addBindValue(login);
    query.addBindValue(passwordHash);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    if (query.next()) {
        // userData["login"] = query.value("login").toString();
        userData["surname"] = query.value("surname").toString();
        userData["name"] = query.value("name").toString();
        userData["role"] = query.value("role").toString();
        return true;
    }
    return false;
}

/**
 * @brief Инициализирует администратора, если он отсутствует в базе
 */
void Database::initializeAdmin() {
    if (!userExists("admin")) {
        QString adminPassword = SHA384::hash("admin123");
        QSqlQuery query(m_db);
        query.prepare("INSERT INTO users (login, password, name, surname, role) VALUES (?, ?, ?, ?, ?)");
        query.addBindValue("admin");
        query.addBindValue(adminPassword);
        query.addBindValue("Admin");
        query.addBindValue("Admin");
        query.addBindValue("admin");
        if (query.exec()) {
            qDebug() << "Database: Админ создан (login: admin, пароль: admin123)";
        } else {
            qCritical() << "Database: Ошибка создания админа:" << query.lastError().text();
        }
    }
}

/**
 * @brief Сохраняет результат прохождения теста пользователем
 * @param login Логин пользователя
 * @param testNumber Номер теста
 * @param score Результат (в процентах)
 * @return true, если сохранение прошло успешно, иначе false
 */
bool Database::saveTestResult(const QString& login, int testNumber, int score) {
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO test_results (login, test_number, score) VALUES (?, ?, ?)");
    query.addBindValue(login);
    query.addBindValue(testNumber);
    query.addBindValue(score);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qWarning() << "Database: Ошибка сохранения результата теста:" << m_lastError;
        return false;
    }
    return true;
}

/**
 * @brief Получает статистику результатов тестов по конкретному студенту
 * @param login Логин студента
 * @return JSON-объект с ФИО и списком тестов
 */
QJsonObject Database::getStudentStatistics(const QString& login) {
    QJsonObject result;

    // Получаем ФИО студента
    QSqlQuery userQuery(m_db);
    userQuery.prepare("SELECT surname, name FROM users WHERE login = ?");
    userQuery.addBindValue(login);

    if (!userQuery.exec() || !userQuery.next()) {
        m_lastError = userQuery.lastError().text();
        qWarning() << "Database: Ошибка получения данных студента:" << m_lastError;
        return result;
    }

    QString surname = userQuery.value("surname").toString();
    QString name = userQuery.value("name").toString();

    // Получаем статистику тестов
    QSqlQuery statsQuery(m_db);
    statsQuery.prepare(
        "SELECT test_number, score, strftime('%Y-%m-%dT%H:%M:%S', date) AS date_iso "
        "FROM test_results "
        "WHERE login = ? "
        "ORDER BY date DESC" // Сортировка от новых к старым
        );
    statsQuery.addBindValue(login);

    QJsonArray statistics;
    if (statsQuery.exec()) {
        while (statsQuery.next()) {
            QJsonObject testResult;
            testResult["test_number"] = statsQuery.value("test_number").toString();
            testResult["score"] = statsQuery.value("score").toString() + "%";
            testResult["date"] = statsQuery.value("date_iso").toString();

            statistics.append(testResult);
        }
    } else {
        qWarning() << "Database: Ошибка получения статистики:" << statsQuery.lastError().text();
    }

    // Формируем итоговый объект
    result["surname"] = surname;
    result["name"] = name;
    result["statistics"] = statistics;

    return result;
}

/**
 * @brief Получает список всех студентов, кроме админа
 * @return JSON-объект с массивом пользователей
 */
QJsonObject Database::getStudents() {
    QJsonObject result;
    QSqlQuery query(m_db);

    query.prepare("SELECT login, name, surname FROM users WHERE role != 'admin' ORDER BY surname, name");

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qWarning() << "Database: Ошибка получения списка студентов:" << m_lastError;
        result["users"] = QJsonArray(); // Возвращаем пустой массив при ошибке
        return result;
    }

    QJsonArray usersArray;
    while (query.next()) {
        QJsonObject user;
        user["login"] = query.value("login").toString();
        user["name"] = query.value("name").toString();
        user["surname"] = query.value("surname").toString();
        usersArray.append(user);
    }

    result["users"] = usersArray;
    return result;
}

/**
 * @brief Получает объединённую статистику по всем пользователям и тестам
 * @return JSON-объект с общей статистикой
 */
QJsonObject Database::getTestStatistics() {
    QJsonObject result;
    QSqlQuery query(m_db);

    // Запрос объединяет таблицы пользователей и результатов тестов
    query.prepare(
        "SELECT u.surname, u.name, tr.test_number, tr.score, "
        "strftime('%Y-%m-%dT%H:%M:%S', tr.date) AS date_iso "
        "FROM test_results tr "
        "JOIN users u ON tr.login = u.login "
        "WHERE u.role != 'admin' "
        "ORDER BY tr.date DESC"
        );

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qWarning() << "Database: Ошибка получения статистики тестов:" << m_lastError;
        result["statistics"] = QJsonArray();
        return result;
    }

    QJsonArray statisticsArray;
    while (query.next()) {
        QJsonObject testEntry;
        testEntry["surname"] = query.value("surname").toString();
        testEntry["name"] = query.value("name").toString();
        testEntry["test_number"] = query.value("test_number").toString();
        testEntry["score"] = query.value("score").toString() + "%";
        testEntry["date"] = query.value("date_iso").toString();

        statisticsArray.append(testEntry);
    }

    result["statistics"] = statisticsArray;
    return result;
}

/**
 * @brief Возвращает последнюю ошибку, произошедшую в базе данных
 * @return Строка с описанием ошибки
 */
QString Database::lastError() const {
    return m_lastError;
}
