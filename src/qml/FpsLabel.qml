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

Label{
    id:fbsLabel
    property int fpsRate: 0
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
    color: "yellow"
    font.pixelSize: 20
    text: "FPS: "+String(fbsLabel.fpsRate)
}
