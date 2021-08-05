pragma Singleton
import QtQuick 2.12
import Qt.labs.settings 1.1

Item {
    property int curSlide: 1
    property int slideCount: 12
    property string imageName: "qrc:/images/slide" + ("%1").arg(curSlide) + ".jpg"
    property alias faBrand: faBrand.name
    property alias faRegular: faRegular.name
    property alias faSolid: faSolid.name

    NumberAnimation on curSlide {
        from: 1
        to: slideCount
        loops: Animation.Infinite
        duration: 70000
    }
    FontLoader {
        id: faBrand
        source: "qrc:/fonts/fa-brands.ttf"
    }
    FontLoader {
        id: faRegular
        source: "qrc:/fonts/fa-regular.ttf"
    }
    FontLoader {
        id: faSolid
        source: "qrc:/fonts/fa-solid.ttf"
    }
}
