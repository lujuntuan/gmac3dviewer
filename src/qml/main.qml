/*********************************************************************************
 *Copyright(C): Juntuan.Lu, 2020-2030, All rights reserved.
 *Author:  Juntuan.Lu
 *Version: 1.0
 *Date:  2021/10/23
 *Email: 931852884@qq.com
 *Description:
 *Others:
 *Function List:
 *History:
 **********************************************************************************/

import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    id:window
    width: 1024
    height: 768
    minimumWidth: 1024
    minimumHeight: 768
    visible: true
    visibility: Window.Maximized
    title: qsTr("GMAC 3D Viewer")
    color: "black"
    onClosing: {
        close.accepted=false
        messagePopup.requestClose()
    }
    Rectangle{
        id:backgroundRect
        anchors.fill: parent
        gradient: Gradient {
            GradientStop {
                position: 0.00;
                color: "#202020";
            }
            GradientStop {
                position: 0.50;
                color: "#707070";
            }
            GradientStop {
                position: 1.00;
                color: "#202020";
            }
        }
    }
    BusyPopup{
        id:busyPopup
        x: parent.width/2-width/2
        y: parent.height-height-50
        onClosed: {
            scene3d.focus=true
        }
    }
    ErrorPopup{
        id:errorPopup
        x: parent.width/2-width/2
        y: parent.height/2-height/2
        onClosed: {
            scene3d.focus=true
        }
    }
    TestPopup{
        id:testPopup
        x: parent.width/2-width/2
        y: parent.height/2-height/2
        onClosed: {
            scene3d.focus=true
        }
    }
    MessagePopup{
        id:messagePopup
        x: parent.width/2-width/2
        y: parent.height/2-height/2
        onClosed: {
            scene3d.focus=true
        }
    }
    Scene3d{
        id:scene3d
    }
}
