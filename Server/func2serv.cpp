// func2serv.cpp
#include "func2serv.h"
#include "server_api.h"
#include "rsa.h"
#include "aes.h"
#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QJsonArray>
#include <cmath>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QImage>
#include <QRgb>

Func2Serv::Func2Serv(Server_api* serverApi, QObject *parent) : QObject(parent), serverApi(serverApi) {
    if (!db.open("test.sqlite")) {
        qCritical() << "Func2Serv: Не удалось подключиться к базе данных!";
        // Можно кинуть исключение или обработать ошибку
    }
    db.initializeAdmin();
}

QJsonObject Func2Serv::handleRegistration(const QJsonObject &data) {
    QJsonObject response;
    QJsonObject responseData;
    response["action"] = "register_result";

    QString surname = data["surname"].toString();
    QString name = data["name"].toString();
    QString login = data["login"].toString();
    QString password = data["password"].toString();

    qDebug() << surname << "-" << name << "-" << login;

    if (db.userExists(login)) {
        responseData["success"] = false;
        responseData["error"] = "Пользователь с таким логином уже существует";
        return response;
    }

    // QString passwordHash = SHA384::hash(password); // хешируем пароль
    QString passwordHash = password;

    bool registered = db.registerUser(login, passwordHash, name, surname);
    if (registered) {
        responseData["success"] = true;
    } else {
        responseData["success"] = false;
        responseData["error"] = "Ошибка регистрации";
    }

    response["data"] = responseData;

    return response;
}

QJsonObject Func2Serv::handleLogin(const QJsonObject &data) {
    QJsonObject response;
    QJsonObject responseData;
    response["action"] = "login_result";

    QString login = data["login"].toString();
    QString password = data["password"].toString();

    // QString passwordHash = SHA384::hash(password); // хешируем пароль
    QString passwordHash = password;

    QJsonObject userData;
    if (db.validateUser(login, passwordHash, userData)) {
        qDebug() << userData ;
        responseData["success"] = true;
        responseData["login"] = login;
        responseData["surname"] = userData["surname"];
        responseData["name"] = userData["name"];
        responseData["role"] = userData["role"];
    } else {
        responseData["success"] = false;
        responseData["error"] = "Неверный логин или пароль";
    }

    response["data"] = responseData;

    return response;
}

QJsonObject Func2Serv::handlePublicKey(const QJsonObject &data, QTcpSocket *socket){
    QJsonObject response;
    QJsonObject responseData;

    if (!data.contains("public_key")) {
        response["action"] = "error";
        response["message"] = "Missing public key";
        return response;
    }

    QString clientPublicKey = data["public_key"].toString();
    qDebug() << "Client public key:" << clientPublicKey;

    if (clientPublicKey.isEmpty()) {
        qCritical() << "Received empty public key!";
        response["action"] = "error";
        response["message"] = "Empty public key received";
        return response;
    }

    // Генерация AES ключа
    QByteArray aesKey = AES::generateKey();  // 128-битный ключ
    qDebug() << "Generated AES key:" << aesKey.toHex();

    // Сохраняем AES ключ для этого клиента
    serverApi->addAesKey(socket, aesKey);

    // Шифруем AES ключ с помощью публичного ключа клиента
    QByteArray encryptedAes = RSA::encryptBytes(aesKey, clientPublicKey);
    qDebug() << "Encrypted AES key:" << encryptedAes.toHex();

    responseData["aes_key"] = QString(encryptedAes.toBase64());
    qDebug() << "Base64 encoded encrypted key:" << responseData["aes_key"].toString();

    // Отправляем зашифрованный AES ключ клиенту
    response["action"] = "server_aes_key";
    response["data"] = responseData;

    return response;
}

QJsonObject Func2Serv::get_user_statistic(const QJsonObject &data){
    QString login = data["login"].toString();
    qDebug() << login;

    QJsonObject response;

    response["action"] = "send_statistics_student";
    response["data"] = db.getStudentStatistics(login);

    return response;
}

QJsonObject Func2Serv::get_users_admin(){
    QJsonObject response;

    response["action"] = "send_users_admin";
    response["data"] = db.getStudents();

    return response;
}

QJsonObject Func2Serv::get_statistics_admin(){
    QJsonObject response;

    response["action"] = "send_statistics_admin";
    response["data"] = db.getTestStatistics();

    return response;
}

QJsonObject Func2Serv::get_user_info(const QJsonObject &data){
    QString login = data["login"].toString();
    qDebug() << login;

    QJsonObject response;

    response["action"] = "send_user_info";
    response["data"] = db.getStudentStatistics(login);

    return response;
}

QString Func2Serv::generateRandomEquation() {
    int a = QRandomGenerator::global()->bounded(1, 21);  // 1-20
    int b = QRandomGenerator::global()->bounded(-10, 11); // -10..10
    int c = QRandomGenerator::global()->bounded(-20, 1);  // -20..0

    // Форматирование с учетом знаков коэффициентов
    return QString("f(x) = %1 * x^2 %2 %3 * x %4 %5")
        .arg(a)
        .arg(b >= 0 ? "+" : "-").arg(std::abs(b))
        .arg(c >= 0 ? "+" : "-").arg(std::abs(c));
}

QJsonObject Func2Serv::generateTest1Task(const QJsonObject &data) {
    QJsonObject response;
    response["action"] = "send_task1";

    QString task = this->generateRandomEquation(); // например: "x^3 - 2x - 5 = 0"
    QJsonObject taskData;
    taskData["task"] = task;

    response["data"] = taskData;
    return response;
}

double Func2Serv::solveChordFromString(const QString& eqn, double eps) {
    // Парсинг коэффициентов квадратного уравнения
    QRegularExpression quad_re(R"(f\(x\) = (\d+) \* x\^2 ([+-]) (\d+) \* x ([+-]) (\d+))");
    QRegularExpressionMatch match = quad_re.match(eqn);

    if (!match.hasMatch()) {
        qWarning() << "Неизвестный формат уравнения:" << eqn;
        return NAN;
    }

    // Извлечение коэффициентов
    bool ok;
    double a = match.captured(1).toDouble(&ok);
    if (!ok) return NAN;

    double b = match.captured(3).toDouble(&ok);
    if (!ok) return NAN;
    if (match.captured(2) == "-") b = -b;

    double c = match.captured(5).toDouble(&ok);
    if (!ok) return NAN;
    if (match.captured(4) == "-") c = -c;

    qDebug() << "Коэффициенты: a =" << a << "b =" << b << "c =" << c;

    // Лямбда-функция для вычисления значения уравнения
    auto f = [a, b, c](double x) {
        return a * x * x + b * x + c;
    };

    // 1. Определяем интервал для положительного корня
    double x0 = 0.0;
    double x1 = 10.0; // Начальная правая граница

    // Ищем смену знака (f(0)*f(x1) < 0)
    while (f(x0) * f(x1) > 0 && x1 < 1000.0) {
        x1 *= 2.0; // Расширяем интервал
    }

    if (f(x0) * f(x1) > 0) {
        qDebug() << "Не удалось найти интервал смены знака";
        return NAN;
    }

    // 2. Реализация метода хорд
    double x = x0;
    double prev_x = x1;
    int iter = 0;
    const int max_iter = 1000;

    do {
        // Рассчитываем новое приближение
        double fx0 = f(x0);
        double fx1 = f(x1);

        // Формула метода хорд
        x = x0 - fx0 * (x1 - x0) / (fx1 - fx0);

        // Определяем новый интервал
        if (f(x) * fx0 < 0) {
            x1 = x;
        } else {
            x0 = x;
        }

        // Критерий остановки
        if (std::abs(x - prev_x) < eps) break;

        prev_x = x;
        iter++;
    } while (iter < max_iter);

    if (iter >= max_iter) {
        qDebug() << "Достигнут предел итераций: " << max_iter;
        return NAN;
    }

    qDebug() << "Метод хорд: корень =" << x << ", итераций:" << iter;
    return x;
}

QJsonObject Func2Serv::checkTest1(const QJsonObject &data) {
    QJsonObject response;
    QJsonObject responseData;
    response["action"] = "check_test1_result";

    QString login = data["login"].toString();
    QString task = data["task"].toString();
    QString answerStr = data["answer"].toString();

    qDebug() << "Проверка решения:";
    qDebug() << "Студент:" << login;
    qDebug() << "Уравнение:" << task;
    qDebug() << "Ответ:" << answerStr;

    // Преобразование ответа студента
    bool ok = false;
    double studentAnswer = answerStr.toDouble(&ok);
    if (!ok) {
        responseData["success"] = false;
        responseData["error"] = "Неверный формат ответа";
        response["data"] = responseData;
        return response;
    }

    // Решение уравнения
    double root = solveChordFromString(task, 1e-6);
    if (std::isnan(root)) {
        responseData["success"] = false;
        responseData["error"] = "Ошибка при решении уравнения";
        response["data"] = responseData;
        return response;
    }

    // Проверка точности
    double diff = std::abs(root - studentAnswer);
    double percent = 100.0;

    if (std::abs(root) > 1e-6) {
        percent = 100.0 - (diff / std::abs(root)) * 100.0;
        if (percent < 0) percent = 0;
    } else {
        percent = (diff < 1e-3) ? 100.0 : 0;
    }

    qDebug() << "Проверка завершена. Точность:" << percent << "%";

    // Сохранение результата
    db.saveTestResult(login, 1, static_cast<int>(percent));

    responseData["success"] = true;
    responseData["score"] = static_cast<int>(percent);
    response["data"] = responseData;

    return response;
}

// Генерация случайной битовой строки для стеганографии
QString Func2Serv::generateRandomBitString(int length) {
    QString bits;
    for (int i = 0; i < length; ++i) {
        bits.append(QRandomGenerator::global()->bounded(2) ? '1' : '0');
    }
    return bits;
}

// Генерация задания для теста 2
QJsonObject Func2Serv::generateTest2Task(const QJsonObject &data) {
    QJsonObject response;
    response["action"] = "send_task2";

    // Генерируем случайную битовую последовательность (128 бит)
    QString task = generateRandomBitString(128);

    QJsonObject taskData;
    taskData["task"] = task;
    response["data"] = taskData;

    return response;
}

// Проверка теста 2 (стеганография)
QJsonObject Func2Serv::checkTest2(const QJsonObject &data) {
    QJsonObject response;
    QJsonObject responseData;
    response["action"] = "check_test2_result";

    QString login = data["login"].toString();
    QString task = data["task"].toString();
    QString pictureBase64 = data["picture"].toString();

    // Проверка размера изображения (не более 2MB)
    if (pictureBase64.size() > 2 * 1024 * 1024) {
        responseData["success"] = false;
        responseData["error"] = "Изображение слишком большое (макс. 2MB)";
        response["data"] = responseData;
        return response;
    }

    QByteArray imageData = QByteArray::fromBase64(pictureBase64.toUtf8());
    QImage image;

    // Проверка формата изображения
    if (!image.loadFromData(imageData, "PNG")) {
        responseData["success"] = false;
        responseData["error"] = "Неподдерживаемый формат изображения. Используйте PNG";
        response["data"] = responseData;
        return response;
    }

    // Проверка размеров изображения
    if (image.width() > 4096 || image.height() > 4096) {
        responseData["success"] = false;
        responseData["error"] = "Слишком большие размеры изображения (макс. 4096x4096)";
        response["data"] = responseData;
        return response;
    }

    // Извлекаем битовую последовательность из изображения
    QString extractedBits = extractBitsFromImage(image, task.length());

    // Сравниваем с оригинальной последовательностью
    int errorCount = 0;
    for (int i = 0; i < task.length(); ++i) {
        if (i >= extractedBits.length() || task[i] != extractedBits[i]) {
            errorCount++;
        }
    }

    // Рассчитываем процент правильности
    double percent = 100.0 * (1.0 - static_cast<double>(errorCount) / task.length());

    // Сохраняем результат
    db.saveTestResult(login, 2, static_cast<int>(percent));

    responseData["success"] = true;
    responseData["score"] = static_cast<int>(percent);
    response["data"] = responseData;

    return response;
}

// Извлечение битов из изображения (LSB)
QString Func2Serv::extractBitsFromImage(const QImage &image, int bitCount) {
    QString bits;
    int bitsExtracted = 0;

    for (int y = 0; y < image.height() && bitsExtracted < bitCount; ++y) {
        for (int x = 0; x < image.width() && bitsExtracted < bitCount; ++x) {
            QRgb pixel = image.pixel(x, y);

            // Извлекаем младшие биты из каждого канала
            int r = qRed(pixel) & 1;
            int g = qGreen(pixel) & 1;
            int b = qBlue(pixel) & 1;

            bits.append(r ? '1' : '0');
            bitsExtracted++;
            if (bitsExtracted >= bitCount) break;

            bits.append(g ? '1' : '0');
            bitsExtracted++;
            if (bitsExtracted >= bitCount) break;

            bits.append(b ? '1' : '0');
            bitsExtracted++;
            if (bitsExtracted >= bitCount) break;
        }
    }

    return bits;
}


