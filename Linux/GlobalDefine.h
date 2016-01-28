/** Global Definition
 * author: gfxmode@live.com
 * date: 2017-12-27
 *
 */

#ifndef GLOBALDEFINE_H
#define GLOBALDEFINE_H

const int WAIT_FOREVER = -1;
const int MAX_DEBUG_STRING = 256;
const int MAX_FILE_NAME = 128;

//return the length of result string. support only 10 radix for easy use and better performa
namespace GlobalFun
{
    int my_itoa(int val, char* buf);
}

#endif // GLOBALDEFINE_H
