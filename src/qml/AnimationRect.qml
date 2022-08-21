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
    id:animationRect
    title: "Frame animations:"
    property var items: new Array
    Grid{
        id:grid
        columns: Number(window.width/320)
    }
    property Component switchComponent: Switch{
        property int index: -1
        checked: osgControl.animationsStatus[index]!==undefined?osgControl.animationsStatus[index].running:false
        onToggled: {
            if(checked===true){
                osgControl.playAnimation(index,true,5)
            }else{
                osgControl.stopAnimation(index,true)
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
        for(var i in osgControl.animationList){
            var animation=osgControl.animationList[i]
            var item=switchComponent.createObject(grid)
            item.index=i
            item.text=animation
            items.push(item)
        }
    }
}
