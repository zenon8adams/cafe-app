#include "error-handler.h"

namespace cafe {
namespace framework {

ErrorHandler::ErrorHandler(QObject *parent) : QObject(parent)
{
}

ErrorHandler::~ErrorHandler()
{
}

const QString& ErrorHandler::errorMsg()const
{
    return lastError;
}

void ErrorHandler::setErrorMsg(const QString& errorMsg)
{
    if(lastError == errorMsg)
        return;
    lastError = errorMsg;
    emit errorMsgChanged(errorMsg);
}

}
}
