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

import QtQuick 2.9
import QtQuick.Controls 2.9

Popup{
    id:errorPopup
    closePolicy:Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    padding: 20
    rightPadding: 20
    topPadding: 10
    bottomPadding: 10
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
    property string content: "Errored!"
    Label {
        padding: 30
        anchors.centerIn: parent
        font.pixelSize: 16
        text: errorPopup.content
    }
    function pushError(text){
        if(errorPopup.visible){
            content=content+"\n"+text
        }else{
            content=text
            errorPopup.visible=true
        }
    }
}
