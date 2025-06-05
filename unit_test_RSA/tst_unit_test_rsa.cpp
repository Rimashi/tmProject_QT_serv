#include <QtTest>

// add necessary includes here

class unit_test_RSA : public QObject
{
    Q_OBJECT

public:
    unit_test_RSA();
    ~unit_test_RSA();

private slots:
    void test_case1();
};

unit_test_RSA::unit_test_RSA() {}

unit_test_RSA::~unit_test_RSA() {}

void unit_test_RSA::test_case1() {}

QTEST_APPLESS_MAIN(unit_test_RSA)

#include "tst_unit_test_rsa.moc"
