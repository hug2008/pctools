
1. 基于商务电话框架；

2. 每个View独立完成数据操作，打印等,取消宏"SEC_ZPL_LOG"的定义可以去除打印指令的log功能；

3. 每个View的所有功能应该在这 4 个函数内完成：
    1）virtual BOOL PreTranslateMessage(MSG* pMsg);// 主要处理回车键事件
    2) BOOL CheckMeSeting(void);                   // 打印前所有数据合法性检查
    3）BOOL PrintMeLable(void);                    // 生成打印指令并且执行
    4) void UpDateMeAfterPrint(void);              // 数据更新、复位等

4. 2007/11/12 添加了PG530Content & Imei，PG530 C/T box ID两张标签的View

5. 安装程序制作：
    1)将以下文件拷贝到Bin目录下：
       a、ZebraPrinter操作说明.pdf
       b、mfc71d.dll
       c、msvcp71d.dll
       d、msvcr71d.dll
       e、ZebraPrinter_D.exe
       f、ZebraPrinter.mdb
    2）执行脚本：Setup.iss

6. 后续View在此框架的基础上增加;

7. 2007-12-14 Update by huangqr
   1) 更该标记字符(WM_TAG_CHAR)的定义 '-' -> '@'；
   2) 增加机型列表AG-169H(PCO)的选项；
   3) 修正读取配置文件 Contents 下 IDENT_CONTENTS_MAINSET 键的一个读写Bug
  ***** 向工程部发布了第一版

8. 2007-12-19 增加数据库记录卡通箱帖纸信息
9. 版本号：1.0.1


//******************* 2008-01-10 ********************
10. 为AG-169H贴纸的Serial No.增加一个去除前缀的功能:
    S/NO:01234567899  -> 01234567899
11. 版本号：1.0.2

//******************** 2008-03-17 *******************
12. 增加西班牙文的打印支持;
13. 版本号: 1.0.3  &&  标签: V1_0_3
14. 默认不排序(量产正式使用);
13. 版本号: 1.0.4  &&  标签: V1_0_4

//******************** 2008-04-30 *******************
15. 增加机卡分离装箱模块;
16. 修正的Ado对Access的打开已经查询操作;
17. 给数据库添加了密码(DESAYELE);
13. 版本号: 1.0.5  &&  标签: V1_0_5




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 *                   已 知 bug                               *
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
1. Ado 访问Access时，不能够正确返回含有 LIKE 关键字查询的记录集;