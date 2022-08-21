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

#ifndef GOSGKEYMAP_H
#define GOSGKEYMAP_H

#include <map>
#include <qnamespace.h>

namespace GOsgKeyMap {
extern std::map<unsigned int, int> keyMap;

inline int transKey(int key, const char* keyText)
{
    std::map<unsigned int, int>::iterator it = keyMap.find(key);
    return it == keyMap.end() ? static_cast<int>(*(keyText)) : it->second;
}

}

#endif // GOSGKEYMAP_H
