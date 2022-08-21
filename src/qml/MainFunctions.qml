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

Item{
    id:functions
    function init(){
        osgControl.playGrow("lamp",Qt.rgba(0,0,1,1))
    }
    function setHvac(enable,type,speed){ //enable:true,false | //type:0,1,2 | speed:0,1,2,3,4,5,6,7
        osgControl.playFlyForName("hvac")
        if(enable){
            if(speed<=0){
                osgControl.stopParticle()
            }else{
                if(type===0){
                    osgControl.playParticle(Qt.rgba(1,1,1,1),Qt.rgba(1,1,1,1),speed*5)
                }else if(type===1){
                    osgControl.playParticle(Qt.rgba(1,0.9,0.9,1),Qt.rgba(1,0,0,1),speed*5)
                }else{
                    osgControl.playParticle(Qt.rgba(0.9,0.9,1,1),Qt.rgba(0,0,1,1),speed*5)
                }
            }
        }else{
            osgControl.stopParticle()
        }
    }
    function setLamp(enable,type){ //enable:true,false |  //type:0,1,2,3
        osgControl.playFlyForName("lamp")
        if(enable){
            if(type===0){
                osgControl.playGrow("lamp",Qt.rgba(1,0,0,1))
            }else if(type===1){
                osgControl.playGrow("lamp",Qt.rgba(0,1,0,1))
            }else if(type===2){
                osgControl.playGrow("lamp",Qt.rgba(0,0,1,1))
            }else{
                osgControl.playGrow("lamp",Qt.rgba(1,1,0,1))
            }
        }else{
            osgControl.stopGrow("lamp")
        }
    }
    function setSeat(name,rotate,direct,currentPos,targetPos){ //rotate:0,1| direct:0,1 | currentPos:0,1,2,3,4,5 | targetPos:0,1,2,3,4,5
        var duration=10
        var vertor=duration/10
        var start=duration/2-currentPos*vertor
        var end=duration/2-targetPos*vertor
        if(rotate===0){
            osgControl.playFlyForName("seat_left")
        }else{
            osgControl.playFlyForName("seat_right")
        }
        if(direct===0){
            osgControl.playAnimationForName(name,false,duration,end,start)
        }else{
            osgControl.playAnimationForName(name,false,duration,duration-end,duration-start)
        }
    }
    function setGlass(name,rotate,open){ //direct1:0,1 | open:true,false
        var duration=6
        if(rotate===0){
            osgControl.playFlyForName("glass_left")
        }else{
            osgControl.playFlyForName("glass_right")
        }
        if(open){
            osgControl.playAnimationForName(name,false,duration,duration/2,0)
        }else{
            osgControl.playAnimationForName(name,false,duration,duration,duration/2)
        }
    }
}
