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

GroupBox{
    id:flyPathRect
    title: "Fly animations:"
    property var items: new Array
    Grid{
        id:grid
        columns: Number(window.width/320)
    }
    property Component switchComponent: Switch{
        property int index: -1
        checked: osgControl.flyIndex===index
        onToggled: {
            if(checked===true){
                osgControl.playFly(index)
            }else{
                osgControl.stopFly()
            }
        }
    }
    function clear(){
        for(var i in items){
            var item=items[i]
            item.destroy()
        }
        items.length=0
    }
    function create(){
        clear()
        for(var i in osgControl.flyPosList){
            var fly=osgControl.flyPosList[i]
            var item=switchComponent.createObject(grid)
            item.index=i
            item.text=fly.name
            items.push(item)
        }
    }
}
