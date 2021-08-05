#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <QObject>
#include <cafeapp-lib_global.h>

namespace cafe {
namespace framework {

class CAFEAPPLIBSHARED_EXPORT ErrorHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ui_errorMsg READ errorMsg WRITE setErrorMsg NOTIFY errorMsgChanged)

public:
    explicit ErrorHandler(QObject *parent = nullptr);
    ~ErrorHandler();
    const QString& errorMsg()const;
signals:
    void errorMsgChanged(const QString& errorMsg);
public slots:
    void setErrorMsg(const QString& errorMsg);
private:
    QString lastError;
};

}
}

#endif // ERRORHANDLER_H
