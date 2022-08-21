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

#include "gosgkeymap.h"
#include <osgGA/GUIEventAdapter>

namespace GOsgKeyMap {

std::map<unsigned int, int> makeKeyMap()
{
    std::map<unsigned int, int> map;
    map[Qt::Key_Escape] = osgGA::GUIEventAdapter::KEY_Escape;
    map[Qt::Key_Delete] = osgGA::GUIEventAdapter::KEY_Delete;
    map[Qt::Key_Home] = osgGA::GUIEventAdapter::KEY_Home;
    map[Qt::Key_Enter] = osgGA::GUIEventAdapter::KEY_KP_Enter;
    map[Qt::Key_End] = osgGA::GUIEventAdapter::KEY_End;
    map[Qt::Key_Return] = osgGA::GUIEventAdapter::KEY_Return;
    map[Qt::Key_PageUp] = osgGA::GUIEventAdapter::KEY_Page_Up;
    map[Qt::Key_PageDown] = osgGA::GUIEventAdapter::KEY_Page_Down;
    map[Qt::Key_Left] = osgGA::GUIEventAdapter::KEY_Left;
    map[Qt::Key_Right] = osgGA::GUIEventAdapter::KEY_Right;
    map[Qt::Key_Up] = osgGA::GUIEventAdapter::KEY_Up;
    map[Qt::Key_Down] = osgGA::GUIEventAdapter::KEY_Down;
    map[Qt::Key_Backspace] = osgGA::GUIEventAdapter::KEY_BackSpace;
    map[Qt::Key_Tab] = osgGA::GUIEventAdapter::KEY_Tab;
    map[Qt::Key_Space] = osgGA::GUIEventAdapter::KEY_Space;
    map[Qt::Key_Delete] = osgGA::GUIEventAdapter::KEY_Delete;
    map[Qt::Key_Alt] = osgGA::GUIEventAdapter::KEY_Alt_L;
    map[Qt::Key_Shift] = osgGA::GUIEventAdapter::KEY_Shift_L;
    map[Qt::Key_Control] = osgGA::GUIEventAdapter::KEY_Control_L;
    map[Qt::Key_Meta] = osgGA::GUIEventAdapter::KEY_Meta_L;
    map[Qt::Key_F1] = osgGA::GUIEventAdapter::KEY_F1;
    map[Qt::Key_F2] = osgGA::GUIEventAdapter::KEY_F2;
    map[Qt::Key_F3] = osgGA::GUIEventAdapter::KEY_F3;
    map[Qt::Key_F4] = osgGA::GUIEventAdapter::KEY_F4;
    map[Qt::Key_F5] = osgGA::GUIEventAdapter::KEY_F5;
    map[Qt::Key_F6] = osgGA::GUIEventAdapter::KEY_F6;
    map[Qt::Key_F7] = osgGA::GUIEventAdapter::KEY_F7;
    map[Qt::Key_F8] = osgGA::GUIEventAdapter::KEY_F8;
    map[Qt::Key_F9] = osgGA::GUIEventAdapter::KEY_F9;
    map[Qt::Key_F10] = osgGA::GUIEventAdapter::KEY_F10;
    map[Qt::Key_F11] = osgGA::GUIEventAdapter::KEY_F11;
    map[Qt::Key_F12] = osgGA::GUIEventAdapter::KEY_F12;
    map[Qt::Key_F13] = osgGA::GUIEventAdapter::KEY_F13;
    map[Qt::Key_F14] = osgGA::GUIEventAdapter::KEY_F14;
    map[Qt::Key_F15] = osgGA::GUIEventAdapter::KEY_F15;
    map[Qt::Key_F16] = osgGA::GUIEventAdapter::KEY_F16;
    map[Qt::Key_F17] = osgGA::GUIEventAdapter::KEY_F17;
    map[Qt::Key_F18] = osgGA::GUIEventAdapter::KEY_F18;
    map[Qt::Key_F19] = osgGA::GUIEventAdapter::KEY_F19;
    map[Qt::Key_F20] = osgGA::GUIEventAdapter::KEY_F20;
    map[Qt::Key_hyphen] = '-';
    map[Qt::Key_Equal] = '=';
    map[Qt::Key_division] = osgGA::GUIEventAdapter::KEY_KP_Divide;
    map[Qt::Key_multiply] = osgGA::GUIEventAdapter::KEY_KP_Multiply;
    map[Qt::Key_Minus] = '-';
    map[Qt::Key_Plus] = '+';
    map[Qt::Key_Insert] = osgGA::GUIEventAdapter::KEY_KP_Insert;
    return map;
}

std::map<unsigned int, int> keyMap(makeKeyMap());

}
