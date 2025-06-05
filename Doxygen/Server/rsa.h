#ifndef RSA_H
#define RSA_H

#include <QString>
#include <QByteArray>
#include <gmpxx.h>
#include <QDebug>

/**
 * @brief Реализация алгоритма асимметричного шифрования RSA
 * 
 * Класс предоставляет функционал для генерации ключевых пар, 
 * шифрования и дешифрования данных с использованием библиотеки GMP
 * для работы с большими числами.
 */
class RSA {
public:
    /**
     * @brief Пара ключей RSA (открытый и закрытый)
     */
    struct KeyPair {
        QString publicKey;   ///< Открытый ключ в формате "e:n" (строка)
        QString privateKey;  ///< Закрытый ключ в формате "d:n" (строка)
    };

    /**
     * @brief Генерирует пару RSA-ключей
     * @param bits Размер модуля n в битах (обычно 2048+)
     * @return Сгенерированная пара ключей
     * @note Испует e=65537 по умолчанию, при необходимости генерирует другое
     */
    static KeyPair generateKeys(unsigned int bits);
    
    /**
     * @brief Шифрует строку с Base64 кодированием результата
     * @param data Исходная строка для шифрования
     * @param publicKey Открытый ключ в формате "e:n"
     * @return Зашифрованные данные в Base64
     */
    static QString encrypt(const QString& data, const QString& publicKey);
    
    /**
     * @brief Дешифрует строку из Base64 представления
     * @param encrypted Зашифрованные данные в Base64
     * @param privateKey Закрытый ключ в формате "d:n"
     * @return Расшифрованная исходная строка
     */
    static QString decrypt(const QString& encrypted, const QString& privateKey);
    
    /**
     * @brief Шифрует сырые байты с блочной обработкой
     * @param data Данные для шифрования
     * @param publicKey Открытый ключ в формате "e:n"
     * @return Зашифрованные данные в Base64
     * @note Автоматически разбивает данные на блоки
     */
    static QByteArray encryptBytes(const QByteArray& data, const QString& publicKey);
    
    /**
     * @brief Дешифрует сырые байты
     * @param encrypted Зашифрованные данные (без Base64)
     * @param privateKey Закрытый ключ в формате "d:n"
     * @return Расшифрованные исходные данные
     */
    static QByteArray decryptBytes(const QByteArray& encrypted, const QString& privateKey);

private:
    /**
     * @brief Проверка числа на простоту
     * @param n Число для проверки
     * @param k Количество тестов Миллера-Рабина
     * @return true если число вероятно простое
     */
    static bool isPrime(const mpz_class& n, int k = 25);
    
    /**
     * @brief Вычисление модульной инверсии
     * @param e Исходное число
     * @param phi Модуль (функция Эйлера)
     * @return d: (e * d) mod phi = 1
     */
    static mpz_class modInverse(const mpz_class& e, const mpz_class& phi);
    
    /**
     * @brief Генерация случайного простого числа
     * @param result Результат (выходной параметр)
     * @param randGen Генератор случайных чисел GMP
     * @param bits Требуемый размер числа в битах
     * @note Использует вероятностный тест Миллера-Рабина
     */
    static void generatePrime(mpz_class& result, gmp_randclass& randGen, unsigned int bits);
};

#endif // RSA_H