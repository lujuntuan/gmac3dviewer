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
    id:messagePopup
    closePolicy:Popup.NoAutoClose
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
    property string guid: ""
    property string title: "Message"
    property string content: ""
    property int index: -1
    property string buttonText: ""
    property int buttonCount: 0
    property var items: new Array
    signal clicked(int index,string buttonText)
    Component.onCompleted: {
        var oneItem=buttonComponent.createObject(row)
        oneItem.index=0
        oneItem.text="Ok"
        items.push(oneItem)
        buttonCount=items.length
    }
    onClosed: {
        clear()
    }
    function showMessage(guid,title,content,buttons){
        clear()
        messagePopup.guid=guid
        messagePopup.title=title
        messagePopup.content=content
        if(buttons===undefined){
            var oneItem=buttonComponent.createObject(row)
            oneItem.index=0
            oneItem.text="Ok"
            items.push(oneItem)
            buttonCount=items.length
        }else{
            for(var i in buttons){
                var item=buttonComponent.createObject(row)
                item.index=i
                item.text=buttons[i]
                items.push(item)
            }
            buttonCount=items.length
        }
        messagePopup.open()
    }
    function clear(){
        for(var i in items){
            var item=items[i]
            item.destroy()
        }
        items.length=0
        messagePopup.guid=""
        messagePopup.title="Message"
        messagePopup.content=""
        messagePopup.index=-1
        messagePopup.buttonText=""
    }
    function requestClose(){
        messagePopup.showMessage("EXIT","Prompt","Quit ?",["Yes","No"])
    }
    Column{
        id:column
        spacing: 20
        Label{
            id:titleLabel
            text: messagePopup.title
            font.pixelSize: 20
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Label{
            id:contentLabel
            text: messagePopup.content
            font.pixelSize: 16
            font.bold: false
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Row{
            id:row
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20
        }
    }
    property Component buttonComponent: Button{
        property int index: -1
        onClicked: {
            messagePopup.index=index
            messagePopup.buttonText=text
            messagePopup.clicked(messagePopup.index,messagePopup.buttonText)
            if(messagePopup.guid==="EXIT"&&messagePopup.index===0){
                Qt.quit()
            }
            messagePopup.close()
        }
    }
    Rectangle{
        parent: scene3d
        width: scene3d.width
        height: scene3d.height
        color: Qt.rgba(0.1,0.1,0.1,0.8)
        visible: messagePopup.visible
        z:1000
        MouseArea{
            anchors.fill: parent
        }
    }
}
