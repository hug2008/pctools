#if !defined(MSG_TYPE_H)
#define MSG_TYPE_H

#if (_MSC_VER < 1500)
#define WM_ON_MSG	LRESULT
#define WM_ON_OK	(0L)
#define WM_ON_ERR	(-1L)
#else
#define WM_ON_MSG	void
#define WM_ON_OK
#define WM_ON_ERR
#endif

#define	WAV_FILE_NAME	"sound.wav"

#endif