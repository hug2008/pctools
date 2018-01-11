#ifndef __USER_VERIFY__
#define __USER_VERIFY__

#define NAME_SIZE   (20)
#define PASSW_SIZE (10)

#define VERIFY_FILE _T("userId.vf")

typedef struct VerifyNodeTag{
	char szName[NAME_SIZE];
	char szPassW[PASSW_SIZE];
}VerifyNode;

#define SizeofVerifyNode sizeof(VerifyNode)

typedef enum VerifyCodeTag{
	VE_NULL = -1,
	VE_OK = -2,
	VE_ERR_NO_FILE = -3,
	VE_ERR_NAME_LEN = -4,
	VE_ERR_WORDID_LEN = -5,
	VE_ERR_INEXISTENT = -6,
	VE_OVER_FLOW = -7
}VerifyCode;

// 用户名是否存在 VE_OK/VE_ERR_INEXISTENT/VE_ERR_NO_FILE
VerifyCode  isAvailableUserName(const char *szUserName,
								const char *szVerfyFile);

// 用户是否存在 VE_OK/VE_ERR_INEXISTENT/VE_ERR_NO_FILE
VerifyCode  isAvailableNode(const char *szName,
							const char *szPassW,
							const char *szVerfyFile);

// 增加一个用户
unsigned long addVerifyNode(const char *szName,
							const char *szPassW,
							const char *szVerfyFile);

VerifyCode deleteVerifyNode(const char *szName,
							const char *szPassW,
							const char *szVerfyFile);

unsigned long loadAllUser(char szUserArray[][NAME_SIZE],
						  char szPasswArray[][PASSW_SIZE],
						  const char *szVerfyFile);

#endif