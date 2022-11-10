#ifndef AKA_STATUS_CODE_H
#define AKA_STATUS_CODE_H
// 所有状态码
const int KAkaNormalExit = 0x000;
const int KAkaCmdNotFound = 0x100;
const int KAkaInvalidNum = 0x101;
const int KAkaMissingParameter = 0x102;
const int KAkaInvlidCmdFunc = 0x103; // 绑定命令的函数不存在(为null)或者绑定出错

#endif // AKA_STATUS_CODE_H
