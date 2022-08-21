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
import GOsg 1.0

GOsgControl{
    id:osgControl
    rootNode: getUrlForLocal("./sources/car.fbx")
    rootNodeMatrix:{
        "translate":"0,0,0",
        "rotate":"1,-1,-1,1",
        "scale":"1,1,1",
    }
    particleMatrix: {
        "translate":"-2220,0,3650",
        "rotate":"0,0.707107,0,0.707107",
        "scale":"1,1,1",
    }
    flyPosList: [
        {
            "name":"main",
            "path":[
                {
                    "time": 3,
                    "position": "-16650.3,-17765.7,8309.48",
                    "rotation": "0.555573,-0.234268,-0.309971,0.735102",
                    "scale": "1,1,1"
                }
            ]
        },
        {
            "name":"hvac",
            "path":[
                {
                    "time": 2,
                    "position": "-93.689,-2996.29,4626.48",
                    "rotation": "0.56068,0.116917,0.167339,0.802475",
                    "scale": "1,1,1"
                }
            ]
        },
        {
            "name":"lamp",
            "path":[
                {
                    "time": 2,
                    "position": "2801.81,-13.6075,5314.29",
                    "rotation": "0.357543,0.365387,0.614281,0.601094",
                    "scale": "1,1,1"
                }
            ]
        },
        {
            "name":"seat_left",
            "path":[
                {
                    "time": 2,
                    "position":"901.331,1766.29,5664.08",
                    "rotation":"0.0924232,0.440191,0.874076,0.183523",
                    "scale":"1.0, 1.0, 1.0"
                }
            ]
        },
        {
            "name":"seat_right",
            "path":[
                {
                    "time": 2,
                    "position":"950.407,-1647.49,5834.17",
                    "rotation":"0.443918,0.0921224,0.181109,0.872725",
                    "scale":"1.0, 1.0, 1.0"
                }
            ]
        },
        {
            "name":"glass_left",
            "path":[
                {
                    "time": 2,
                    "position": "11616.6,-14102.8,8718.81",
                    "rotation": "0.546186,0.165791,0.238493,0.785694",
                    "scale": "1,1,1"
                }
            ]
        },
        {
            "name":"glass_right",
            "path":[
                {
                    "time": 2,
                    "position": "12528.4,13722.7,7999.21",
                    "rotation": "0.198331,0.560355,0.758074,0.268311",
                    "scale": "1,1,1"
                }
            ]
        },
    ]
}
