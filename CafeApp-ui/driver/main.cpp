#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <model/table-model.h>
#include <model/menu-list-model.h>
#include <controller/master-controller.h>
#include <controller/login-form-controller.h>
#include <controller/signup-form-controller.h>
#include <controller/master-view-controller.h>
#include <controller/staff-view-controller.h>
#include <controller/menu-form-controller.h>
#include <controller/tooltip-description-controller.h>
#include <framework/error-handler.h>
#include <utility/rsa-encryption.h>
#include <qicon.h>
#include <QFile>
#include <QTextStream>
#include <QDebug>

constexpr const char *appname = "CafeApp";

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon("../../CafeApp-ui/assets/icons/cafeapp-logo-icon.ico"));


    qmlRegisterType<cafe::models::TableModel>( appname, 1, 0, "TableModel");
    qmlRegisterType<cafe::models::MenuListModel>( appname, 1, 0, "MenuListModel");
    qmlRegisterType<cafe::controllers::MasterController>( appname, 1, 0, "MasterController");
    qmlRegisterType<cafe::controllers::LogInFormController>( appname, 1, 0, "LogInFormController");
    qmlRegisterType<cafe::controllers::SignUpFormController>( appname, 1, 0, "SignUpFormController");
    qmlRegisterType<cafe::controllers::MasterViewController>( appname, 1, 0, "MasterViewController");
    qmlRegisterType<cafe::controllers::StaffViewController>( appname, 1, 0, "StaffViewController");
    qmlRegisterType<cafe::controllers::MenuFormController>( appname, 1, 0, "MenuFormController");
    qmlRegisterType<cafe::controllers::ToolTipDescriptionController>( appname, 1, 0, "ToolTipDescriptionController");
    qmlRegisterType<cafe::framework::ErrorHandler>( appname, 1, 0, "ErrorHandler");
    qmlRegisterType<cafe::utility::RSAEncryption>( appname, 1, 0, "RSAEncryption");
    cafe::controllers::MasterController masterController;
    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/");
    engine.rootContext()->setContextProperty("masterController", &masterController);
    engine.load(QUrl(QStringLiteral("qrc:/views/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
