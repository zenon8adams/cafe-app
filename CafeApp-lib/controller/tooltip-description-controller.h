#ifndef TOOLTIPDESCRIPTIONCONTROLLER_H
#define TOOLTIPDESCRIPTIONCONTROLLER_H

#include <QObject>
#include <controller/db-controller.h>
#include <QScopedPointer>

#include <cafeapp-lib_global.h>

namespace cafe {
namespace controllers {

class CAFEAPPLIBSHARED_EXPORT ToolTipDescriptionController : public QObject
{
    Q_OBJECT
public:
    explicit ToolTipDescriptionController(QObject *parent = nullptr, IDBController *dbController = nullptr);
    ~ToolTipDescriptionController();
    Q_INVOKABLE QString at(const QString& staffName)const;
signals:

public slots:
    Q_INVOKABLE void rebuildToolTipDescription();
private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};

}
}
#endif // TOOLTIPDESCRIPTIONCONTROLLER_H
