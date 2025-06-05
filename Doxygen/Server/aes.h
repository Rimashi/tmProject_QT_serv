#ifndef AES_H
#define AES_H

#include <QByteArray>
#include <QString>

/**
 * @brief Реализация алгоритма шифрования AES-128 (Advanced Encryption Standard)
 * 
 * Класс предоставляет статические методы для генерации ключей, шифрования и дешифрования данных
 * с использованием 128-битного ключа и режима ECB (Electronic Codebook).
 * @warning Не используйте ECB режим для чувствительных данных! Для реальных проектов 
 *          рекомендуется реализовать CBC или другие более безопасные режимы.
 */
class AES {
public:
    /**
     * @brief Генерирует случайный 128-битный ключ
     * @return Сгенерированный ключ длиной 16 байт (128 бит)
     */
    static QByteArray generateKey();
    
    /**
     * @brief Шифрует данные с использованием AES-128
     * @param data Исходные данные для шифрования
     * @param key 128-битный ключ шифрования
     * @return Зашифрованные данные в формате QByteArray
     * @note Автоматически добавляет PKCS#7 padding
     */
    static QByteArray encrypt(const QByteArray& data, const QByteArray& key);
    
    /**
     * @brief Дешифрует данные с использованием AES-128
     * @param data Зашифрованные данные
     * @param key 128-битный ключ шифрования
     * @return Расшифрованные данные в формате QByteArray
     * @note Автоматически удаляет PKCS#7 padding
     */
    static QByteArray decrypt(const QByteArray& data, const QByteArray& key);

private:
    /// Таблица замены (S-box) для этапа SubBytes
    static const uint8_t sbox[256];
    
    /// Обратная таблица замены для этапа InvSubBytes
    static const uint8_t invSbox[256];
    
    /// Таблица констант для расширения ключа
    static const uint8_t rcon[10];
    
    /**
     * @brief Этап SubBytes в AES
     * @param state Текущее состояние блока (16 байт)
     */
    static void subBytes(QByteArray& state);
    
    /**
     * @brief Этап ShiftRows в AES
     * @param state Текущее состояние блока (16 байт)
     */
    static void shiftRows(QByteArray& state);
    
    /**
     * @brief Этап MixColumns в AES
     * @param state Текущее состояние блока (16 байт)
     */
    static void mixColumns(QByteArray& state);
    
    /**
     * @brief Этап AddRoundKey в AES
     * @param state Текущее состояние блока (16 байт)
     * @param roundKey Раундовый ключ (16 байт)
     */
    static void addRoundKey(QByteArray& state, const QByteArray& roundKey);
    
    /**
     * @brief Обратный этап InvSubBytes
     * @param state Текущее состояние блока (16 байт)
     */
    static void invSubBytes(QByteArray& state);
    
    /**
     * @brief Обратный этап InvShiftRows
     * @param state Текущее состояние блока (16 байт)
     */
    static void invShiftRows(QByteArray& state);
    
    /**
     * @brief Обратный этап InvMixColumns
     * @param state Текущее состояние блока (16 байт)
     */
    static void invMixColumns(QByteArray& state);
    
    /**
     * @brief Генерация раундовых ключей (Key Expansion)
     * @param key Исходный 128-битный ключ
     * @return Расширенный ключ (176 байт для 11 раундов)
     */
    static QByteArray expandKey(const QByteArray& key);
    
    /**
     * @brief Умножение в поле Галуа (GF(2^8))
     * @param a Первый операнд
     * @param b Второй операнд
     * @return Результат умножения
     */
    static uint8_t gmul(uint8_t a, uint8_t b);
};

#endif // AES_H