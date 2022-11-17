#ifndef AKA_STATUS_CODE_H
#define AKA_STATUS_CODE_H
// 所有状态码
const int KAkaNormalExit = 0x000;
const int KAkaCmdNotFound = 0x100;
const int KAkaInvalidNum = 0x101;
const int KAkaMissingParameter = 0x102;
const int KAkaInvlidCmdFunc = 0x103; // 绑定命令的函数不存在(为null)或者绑定出错
const int KAkaTooManyParameters = 0x104;
const int KAkaMissingQuote = 0x105; // 缺少引号或引号不是成对出现
const int KAkaInvalidPath = 0x106;
const int KAkaFileAlreadyExists = 0x107;
const int KAkaInvalidParameter = 0x108;
const int KAkaInvalidName = 0x109;
const int KAkaBrokenDataFolder = 0x110;
const int KAkaFileNotFound = 0x111;
#endif // AKA_STATUS_CODE_H
