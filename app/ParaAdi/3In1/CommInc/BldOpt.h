
/************************************************************************/
/*                   单模 3In1 工程编译条件                             */
/*                   huangqr 2008-12-06 14:05 Add                       */
/************************************************************************/

/************************************************************************/
/*             本系统信息定义                                           */
/************************************************************************/
#if !defined(SYS_NAME)
#if defined(R16_ATC)
#define SYS_NAME  _T("3In1PhManagR16")
#else
#define SYS_NAME  _T("3In1PhManagR10")
#endif
#endif

#if !defined(R16_ATC)
#define  R16_ATC
#endif

#if !defined(ENABLE_AREA_CODE)
#define ENABLE_AREA_CODE
#endif

/*
#if !defined(ICCID_BUILD)
#define ICCID_BUILD
#endif

#if !defined(CELL_LEVEL)
#define CELL_LEVEL
#endif
*/

/************************************************************************/
/*  FOR_CLIENT 客户版与生产版的编译开关                                 */
/************************************************************************/
#if !defined(FOR_CLIENT)
#define FOR_CLIENT
#endif

#if !defined(XMSTART_NET_AND_COMMGR_ENABLE)
#define XMSTART_NET_AND_COMMGR_ENABLE
#endif

#if !defined(XMSTART_3IN1_NET_MANAGER)
#define XMSTART_3IN1_NET_MANAGER
#endif


#if !defined(SIM_ENCRYPT_COMPLETE_SOLUTIONS)
#define SIM_ENCRYPT_COMPLETE_SOLUTIONS
#if !defined(MAIN_SIMMPK_XMSTART)
#define MAIN_SIMMPK_XMSTART
#endif
#else
#if !defined(MAIN_SIMMPK_XMSTART)
#define MAIN_SIMMPK_XMSTART
#endif
#endif


#if !defined(LOCK_LAC)
#define LOCK_LAC
#endif

/*
#if !defined(FULL_LIST)
#define FULL_LIST
#endif
*/

#if !defined(ENABLE_SESSION_TOLL_FUNCTION)
#define ENABLE_SESSION_TOLL_FUNCTION
#endif

#if !defined(ENABLE_MOBILE_HEAD)
#define ENABLE_MOBILE_HEAD/* 移动电话号段头 */
#endif
/*
#if !defined(XMSTART_OPT_DIALFLOW)
#define XMSTART_OPT_DIALFLOW
#endif
*/
#if !defined(ENGLISH_VERSION)
#define ENGLISH_VERSION
#endif


