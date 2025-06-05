#include <QtTest>
#include <string>
#include <iostream>
#include "C:\QTCreator\projects\func2client\func2client.h"

class Sha384Test : public QObject
{
    Q_OBJECT

private slots:
    void test_empty_string();
    void test_known_value();
};

void Sha384Test::test_empty_string()
{
    std::string result = func2client::sha384("");
    std::string expected = "38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da"
                           "274edebfe76f65fbd51ad2f14898b95b";

    qDebug("Result for empty string: %s", result.c_str());
    QVERIFY2(result == expected, "sha384(\"\") returned incorrect hash");
}

void Sha384Test::test_known_value()
{
    std::string result = func2client::sha384("abc");
    std::string expected = "cb00753f45a35e8bb5a03d699ac65007272c32ab0eded1631a8b605a43ff5bed"
                           "8086072ba1e7cc2358baeca134c825a7";

    qDebug("Result for 'abc': %s", result.c_str());
    QVERIFY2(result == expected, "sha384(\"abc\") returned incorrect hash");
}

int main(int argc, char *argv[])
{
    Sha384Test tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "main.moc"
