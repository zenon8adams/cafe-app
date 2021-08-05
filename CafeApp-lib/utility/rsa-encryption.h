#ifndef RSAENCRYPTION_H
#define RSAENCRYPTION_H

#include <QObject>
#include <QScopedPointer>
#include <cafeapp-lib_global.h>

namespace cafe {
namespace utility {

class CAFEAPPLIBSHARED_EXPORT RSAEncryption : public QObject
{
    Q_OBJECT
public:
    RSAEncryption(const std::pair<intmax_t, intmax_t>& seed = { 0x11, 0x13 }, QObject *parent = nullptr);
    ~RSAEncryption();
    QString encrypt(QString plaintext, int nulls = 0, bool append_key = false);
    QString decrypt(QString cipheredtext);
    QString extractKey(QString &cipheredtext);
    void setPublicKey(const QString& rep);
private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};


}
}
#endif // RSAENCRYPTION_H
