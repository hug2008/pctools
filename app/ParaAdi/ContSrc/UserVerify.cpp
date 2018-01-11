
#include "stdafx.h"
#include "UserVerify.h"
#include <io.h>

static const VerifyNode EmptyNote = {{0}, {0}};

VerifyCode  isAvailableUserName(const char *szUserName,
								const char *szVerfyFile)
{
	VerifyNode readNote;
	FILE *fp = NULL;
	fp = fopen(szVerfyFile, "rb");
	if (!fp){
		return VE_ERR_NO_FILE;
	}
	while (!feof(fp)){
		fread(&readNote, SizeofVerifyNode, 0x01, fp);

		if (0 == memcmp(readNote.szName, szUserName, NAME_SIZE)){
			fclose(fp);
			return VE_OK;
		}
	}

	return VE_ERR_INEXISTENT;
}

VerifyCode  isAvailableNode(const char *szName,
							const char *szPassW,
							const char *szVerfyFile)
{
	VerifyNode verifyNode,
		       readNote;
	FILE *fp = NULL;

	memset(&verifyNode, 0x00, SizeofVerifyNode);
	memcpy(verifyNode.szName,   szName,   NAME_SIZE);
	memcpy(verifyNode.szPassW,  szPassW,  PASSW_SIZE);

	fp = fopen(szVerfyFile, "rb");
	if (!fp){
		return VE_ERR_NO_FILE;
	}

	while (!feof(fp)){
		fread(&readNote, SizeofVerifyNode, 0x01, fp);

		if (0 == memcmp(&verifyNode, &readNote, SizeofVerifyNode)){
			fclose(fp);
			return VE_OK;
		}

		memset(&readNote, 0x00, SizeofVerifyNode);
	}

	fclose(fp);
	return VE_ERR_INEXISTENT;
}


unsigned long addVerifyNode(const char *szName,
							const char *szPassW,
							const char *szVerfyFile)
{
	unsigned long nSize = 0L;
	VerifyNode newNote;
	FILE *fp = NULL;

	if (VE_OK == isAvailableUserName(szName, szVerfyFile)){
		return nSize;
	}

	if (access(szVerfyFile,0) == 0){
		fp = fopen(szVerfyFile, "rb+");
	}else{
		fp = fopen(szVerfyFile, "wb+");
	}

	if (!fp){
		return VE_ERR_NO_FILE;
	}

	while (!feof(fp)){
		fread(&newNote, SizeofVerifyNode, 0x01, fp);
		nSize++;
	}

	memset(&newNote, 0x00, SizeofVerifyNode);
	memcpy(newNote.szName,   szName,   NAME_SIZE);
	memcpy(newNote.szPassW,  szPassW,  PASSW_SIZE);

	// 定位到文件结尾
	fseek(fp, 0L, SEEK_END);
	// 写入一个node
	fwrite(&newNote, SizeofVerifyNode, 0x01, fp);
	nSize++;

	fclose(fp);

	return nSize;
}


VerifyCode deleteVerifyNode(const char *szName,
							const char *szPassW,
							const char *szVerfyFile)
{
	VerifyNode delNote,
		       findNote;
	VerifyCode ret = VE_ERR_INEXISTENT;
	FILE *fpS = NULL,
		 *fpT = NULL;

	char *pTf = (char *)malloc(strlen(szVerfyFile) + 10);
	if (!pTf){
		pTf = NULL;
		return VE_OVER_FLOW;
	}
	sprintf(pTf, "%s._tmp", szVerfyFile);

	memset(&delNote, 0x00, SizeofVerifyNode);
	memcpy(delNote.szName,   szName,   NAME_SIZE);
	memcpy(delNote.szPassW,  szPassW,  PASSW_SIZE);

	fpS = fopen(szVerfyFile, "rb");
	if (!fpS){
		free(pTf);
		pTf = NULL;
		return VE_ERR_NO_FILE;
	}

	fpT = fopen(pTf, "wb");
	if (!fpT){
		free(pTf);
		pTf = NULL;
		fclose(fpS);
		return VE_ERR_NO_FILE;
	}

	while (!feof(fpS))
	{
		fread(&findNote, SizeofVerifyNode, 0x01, fpS);
		if ((0 == strcmp(findNote.szName, delNote.szName)) && (0 == strcmp(findNote.szPassW, delNote.szPassW)))
		{
			memset(&findNote, 0x00, SizeofVerifyNode);
			ret = VE_OK;
			continue;
		}

		if (0 != memcmp(&findNote, &EmptyNote, SizeofVerifyNode))
		{
			fwrite(&findNote, SizeofVerifyNode, 0x01, fpT);
			memset(&findNote, 0x00, SizeofVerifyNode);
		}
	}

	
	fclose(fpS);
	fclose(fpT);

	if (VE_OK == ret)
	{
		unlink(szVerfyFile);
		rename(pTf, szVerfyFile);
	}
	else
	{
		unlink(pTf);
	}

	free(pTf);

	return ret;
}

unsigned long loadAllUser(char szUserArray[][NAME_SIZE],
						  char szPasswArray[][PASSW_SIZE],
						  const char *szVerfyFile)
{
	unsigned long count = 0L;
	FILE *fp = NULL;
	VerifyNode findNote;

	fp = fopen(szVerfyFile, "rb");
	if (!fp)
	{
		return count;
	}

	while (!feof(fp))
	{
		memcpy(&findNote, &EmptyNote, SizeofVerifyNode);
		fread(&findNote, SizeofVerifyNode, 0x01, fp);
		if (0 == strlen(findNote.szName))
		{
			continue;
		}

		memcpy(szUserArray[count],    findNote.szName,  NAME_SIZE);
		memcpy(szPasswArray[count++], findNote.szPassW, PASSW_SIZE);
	}

	fclose(fp);

	return count;
}
