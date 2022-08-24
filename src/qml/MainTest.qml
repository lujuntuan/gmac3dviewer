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

Item{
    id:test
    function init(){
        testPopup.create([
                             "空调测试（开）",
                             "空调测试（热）",
                             "空调测试（冷）",
                             "空调测试（关）",
                             "氛围灯测试（红）",
                             "氛围灯测试（绿）",
                             "氛围灯测试（蓝）",
                             "氛围灯测试（关）",
                             "主驾驶座椅测试（向后）",
                             "主驾驶座椅测试（向前）",
                             "副驾驶座椅测试（向后）",
                             "副驾驶座椅测试（向前）",
                             "主驾驶玻璃测试（下降）",
                             "主驾驶玻璃测试（上升）",
                             "副驾驶玻璃测试（下降）",
                             "副驾驶玻璃测试（上升）",
                         ])
    }
    Connections{
        target: testPopup
        function onTestChanged(index,enable,text){
            switch(text){
            case "空调测试（开）":
                functions.setHvac(true,0,1)
                break
            case "空调测试（热）":
                functions.setHvac(true,1,2)
                break
            case "空调测试（冷）":
                functions.setHvac(true,2,4)
                break
            case "空调测试（关）":
                functions.setHvac(false,0,3)
                break
            case "氛围灯测试（红）":
                functions.setLamp(true,0)
                break
            case "氛围灯测试（绿）":
                functions.setLamp(true,1)
                break
            case "氛围灯测试（蓝）":
                functions.setLamp(true,2)
                break
            case "氛围灯测试（关）":
                functions.setLamp(false,3)
                break
            case "主驾驶座椅测试（向后）":
                functions.setSeat("seat_left1",0,0,5,0)
                break
            case "主驾驶座椅测试（向前）":
                functions.setSeat("seat_left1",0,1,0,5)
                break
            case "副驾驶座椅测试（向后）":
                functions.setSeat("seat_right1",1,0,5,0)
                break
            case "副驾驶座椅测试（向前）":
                functions.setSeat("seat_right1",1,1,0,5)
                break
            case "主驾驶玻璃测试（下降）":
                functions.setGlass("glass_left1",0,true)
                break
            case "主驾驶玻璃测试（上升）":
                functions.setGlass("glass_left1",0,false)
                break
            case "副驾驶玻璃测试（下降）":
                functions.setGlass("glass_right1",1,true)
                break
            case "副驾驶玻璃测试（上升）":
                functions.setGlass("glass_right1",1,false)
                break
            case 0:
                break
            default:
                break
            }
        }
    }
}
