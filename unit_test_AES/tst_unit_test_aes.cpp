#include <QtTest>

// add necessary includes here

class unit_test_AES : public QObject
{
    Q_OBJECT

public:
    unit_test_AES();
    ~unit_test_AES();

private slots:
    void test_case1();
};

unit_test_AES::unit_test_AES() {}

unit_test_AES::~unit_test_AES() {}

void unit_test_AES::test_case1() {}

QTEST_APPLESS_MAIN(unit_test_AES)

#include "tst_unit_test_aes.moc"
