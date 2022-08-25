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
    id:testPopup
    padding: 20
    rightPadding: 20
    topPadding: 10
    bottomPadding: 10
    width: (titleLabel.width>grid.width?titleLabel.width:grid.width)+padding*2
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
    property var items: new Array
    signal testChanged(int index,bool enable,string text)
    Column{
        id:column
        spacing: 20
        Label{
            id:titleLabel
            text: "Test Popup"
            font.pixelSize: 20
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Grid{
            id:grid
            anchors.horizontalCenter: parent.horizontalCenter
            columns: 4
            padding: 10
            spacing: 10
        }
    }
    property Component buttonComponent: Button{
        property int index: -1
        onClicked: {
            testPopup.testChanged(index,checked,text)
            testPopup.close()
        }
    }
    function clear(){
        for(var i in items){
            var btn=items[i]
            btn.destroy()
        }
        items.length=0
    }
    function create(list){
        clear()
        for(var i in list){
            var item=buttonComponent.createObject(grid)
            item.index=i
            item.text=list[i]
            items.push(item)
        }
    }
}
