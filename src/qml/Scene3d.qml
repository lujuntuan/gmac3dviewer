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
import QtQuick.Window 2.9
import GOsg 1.0

GOsgRenderItem{
    id:scene3d
    anchors.fill: parent
    focus: true
    backgroundColor: "transparent"
    targetFpsRate: 0
    osgControl: MainOsgControl{
        id:osgControl
        onLoadingChanged: {
            busyPopup.visible=osgControl.loading
        }
        onErrorMessageChanged: {
            errorPopup.pushError(osgControl.errorMessage)
        }
    }
    Keys.onPressed: {
        if (event.key === Qt.Key_F1) {
            functionRect.visible=!functionRect.visible
        }else if(event.key === Qt.Key_F2){
            testPopup.visible=!testPopup.visible
        }else if(event.key === Qt.Key_F3){
            if(window.visibility===Window.FullScreen){
                window.showNormal()
            }else{
                window.showFullScreen()
            }
        }else if(event.key === Qt.Key_F4){
            messagePopup.requestClose()
        }else if(event.key === Qt.Key_Escape){
            if(window.visibility===Window.FullScreen){
                window.showNormal()
            }else{
                messagePopup.requestClose()
            }
        }
    }
    FpsLabel{
        id:fpsLabel
        fpsRate:scene3d.currentFpsRate
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        anchors.right: parent.right
        anchors.rightMargin: 30
    }
    Rectangle{
        id:functionRect
        width: parent.width
        height: animationRect.height>flyPathRect.height?animationRect.height+10:flyPathRect.height+10
        anchors.horizontalCenter: parent.horizontalCenter
        color: Qt.rgba(0.1,0.1,1,0.2)
        visible: false
        AnimationRect{
            id:animationRect
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 5
        }
        FlyPathRect{
            id:flyPathRect
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 5
        }
    }
    MainFunctions{
        id:functions
    }
    MainTest{
        id:test
    }
    Timer{
        id:startFlyTimer
        repeat: false
        interval: 500
        onTriggered: {
            osgControl.playFlyForName("main")
        }
    }
    Connections{
        target: osgControl
        function onRootNodeChanged(){
            if(!osgControl.hasError){
                functions.init()
                test.init()
                //startFlyTimer.restart()
                animationRect.create()
                flyPathRect.create()
            }
        }
    }
}
