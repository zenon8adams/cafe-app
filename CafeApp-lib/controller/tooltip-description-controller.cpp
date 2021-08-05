#include "tooltip-description-controller.h"
#include <controller/master-view-controller.h>
#include <QDebug>

namespace cafe {
namespace controllers {

class ToolTipDescriptionController::Implementation
{
public:
    Implementation(ToolTipDescriptionController *_toolTipDescriptionController, IDBController *_dbController)
        : toolTipDescriptionController(_toolTipDescriptionController), dbController(_dbController)
    {
        Q_UNUSED(toolTipDescriptionController)
        buildStaffDescriptionModel();
    }
    ToolTipDescriptionController *toolTipDescriptionController{ nullptr };
    IDBController *dbController{ nullptr };
    QMap<QString, QString> staffsData;

    QString staffDescriptionModel(const QString& staffID, const QString& firstName, const QString& lastName, const QString& dateJoined, const QString& leftOn, QPair<bool, const QString&> presence)
    {
        QString model = "ID: " + staffID + "\n"
                        "First Name: " + firstName + "\n"
                        "Last Name: " + lastName + "\n"
                        "Joined on: " + dateJoined;

        if(presence.first)
        {
            model += "\nSacked: yes\n" +
                     QString("Date: ") + leftOn +
                     QString("\nReason: ");

            if(presence.second.isEmpty())
                model += "Not given";
            else model += presence.second;
        }

        return model;
    }
    void rebuildToolTipDescription()
    {
        staffsData.clear();
        buildStaffDescriptionModel();
    }
    void buildStaffDescriptionModel()
    {
        auto qresult{ dbController->readFrom("SELECT id, first_name, last_name, joinedOn, leftOn, state, ReasonForLeaving FROM %1 WHERE userType='Staff'", "valid_users", 7) };

        if(!qresult.isEmpty())
        {
            for(const auto& elm : qresult)
                staffsData.insert(elm[1].toString(), staffDescriptionModel(elm[0].toString(), elm[1].toString(), elm[2].toString(), elm[3].toString(), elm[4].toString(), {elm[5].toString() != "active", elm[6].toString()}));
        }
    }
};

ToolTipDescriptionController::ToolTipDescriptionController(QObject *parent, IDBController *dbController) : QObject(parent)
{
    implementation.reset(new Implementation(this, dbController));
}
ToolTipDescriptionController::~ToolTipDescriptionController()
{
}
QString ToolTipDescriptionController::at(const QString& staffName)const
{
    return implementation->staffsData[staffName];
}
void ToolTipDescriptionController::rebuildToolTipDescription()
{
    implementation->rebuildToolTipDescription();
}
}
}
