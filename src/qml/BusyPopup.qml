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
import QtQuick.Controls 2.15

Popup{
    id:busyPopup
    closePolicy:Popup.NoAutoClose
    leftPadding: 40
    rightPadding: 40
    topPadding: 20
    bottomPadding: 20
    enter: Transition {
        ParallelAnimation{
            NumberAnimation {
                property: "opacity"
                easing.type: Easing.OutCubic
                duration: 300
                from: 0.0
                to: 1.0
            }
            NumberAnimation {
                property: "scale"
                easing.type: Easing.OutCubic
                duration: 300
                from: 1.2
                to: 1.0
            }
        }
    }
    exit: Transition {
        ParallelAnimation{
            NumberAnimation {
                property: "opacity"
                easing.type: Easing.OutCubic
                duration: 300
                from: 1.0
                to: 0.0
            }
            NumberAnimation {
                property: "scale"
                easing.type: Easing.OutCubic
                duration: 300
                from: 1.0
                to: 1.2
            }
        }
    }
    Row{
        anchors.centerIn: parent
        spacing: 20
        Label {
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 25
            text: "Loading..."
        }
        BusyIndicator{
            anchors.verticalCenter: parent.verticalCenter
            running: busyPopup.visible
            width: 40
            height: 40
        }
    }
}
