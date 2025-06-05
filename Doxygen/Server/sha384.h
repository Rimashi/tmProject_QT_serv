#ifndef SHA384_H
#define SHA384_H

#include <QString>
#include <vector>
#include <QJsonObject>

/**
 * @brief Реализация алгоритма криптографического хеширования SHA-384
 * 
 * Класс предоставляет статический метод для вычисления SHA-384 хеша.
 * Соответствует стандарту FIPS 180-4.
 */
class SHA384 {
public:
    /**
     * @brief Вычисляет SHA-384 хеш для входной строки
     * @param input Входная строка для хеширования
     * @return Хеш в виде шестнадцатеричной строки длиной 96 символов
     */
    static QString hash(const QString& input);
    
    /// Константы для алгоритма SHA-384 (K-константы раундов)
    static const uint64_t K[80];

private:
    /**
     * @brief Циклический сдвиг вправо
     * @param x Исходное 64-битное значение
     * @param n Количество бит для сдвига
     */
    static uint64_t rotateRight(uint64_t x, uint64_t n);
    
    /**
     * @brief Логический сдвиг вправо
     * @param x Исходное 64-битное значение
     * @param n Количество бит для сдвига
     */
    static uint64_t shiftRight(uint64_t x, uint64_t n);
    
    /**
     * @brief Функция выбора (Choice function)
     * @param x, y, z 64-битные входные значения
     * @return (x AND y) XOR (NOT x AND z)
     */
    static uint64_t ch(uint64_t x, uint64_t y, uint64_t z);
    
    /**
     * @brief Функция большинства (Majority function)
     * @param x, y, z 64-битные входные значения
     * @return (x AND y) XOR (x AND z) XOR (y AND z)
     */
    static uint64_t maj(uint64_t x, uint64_t y, uint64_t z);
    
    /**
     * @brief Основная функция преобразования (Sigma0)
     * @param x 64-битное входное значение
     * @return ROTR(x,28) XOR ROTR(x,34) XOR ROTR(x,39)
     */
    static uint64_t sigma0(uint64_t x);
    
    /**
     * @brief Основная функция преобразования (Sigma1)
     * @param x 64-битное входное значение
     * @return ROTR(x,14) XOR ROTR(x,18) XOR ROTR(x,41)
     */
    static uint64_t sigma1(uint64_t x);
    
    /**
     * @brief Вспомогательная функция преобразования (Gamma0)
     * @param x 64-битное входное значение
     * @return ROTR(x,1) XOR ROTR(x,8) XOR SHR(x,7)
     */
    static uint64_t gamma0(uint64_t x);
    
    /**
     * @brief Вспомогательная функция преобразования (Gamma1)
     * @param x 64-битное входное значение
     * @return ROTR(x,19) XOR ROTR(x,61) XOR SHR(x,6)
     */
    static uint64_t gamma1(uint64_t x);
    
    /**
     * @brief Добавление padding к сообщению
     * @param message Сообщение для дополнения (модифицируется на месте)
     * 
     * Алгоритм дополнения:
     * 1. Добавить бит '1'
     * 2. Добавить k бит '0'
     * 3. Добавить 128-битную длину сообщения (big-endian)
     */
    static void padMessage(std::vector<uint8_t>& message);
    
    /**
     * @brief Обработка 1024-битного блока данных
     * @param block Указатель на блок данных (128 байт)
     * @param state Текущее состояние хеша (модифицируется на месте)
     */
    static void processBlock(const uint8_t* block, uint64_t* state);
};

#endif // SHA384_H