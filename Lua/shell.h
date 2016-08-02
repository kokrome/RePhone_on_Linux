
#ifndef __SHELL_H__
#define __SHELL_H__

#include "vmthread.h"
#include "vmtype.h"
#include "vmgsm_sms.h"
#include "vmtimer.h"
#include "vmdcl.h"
#include "vmbt_cm.h"
#include "vmsock.h"

#include "lua.h"
#include "mqtt_client.h"

#define USE_UART1_TARGET

#define LUA_NET "net"

#define REDLED           17
#define GREENLED         15
#define BLUELED          12
#define REG_BASE_ADDRESS 0xa0000000

#define MAX_SMS_CONTENT_LEN		160*4

#define UART_BUFFER_LEN 1024

#define DEFAULT_CCWAIT 1000

#define SHELL_MESSAGE_ID        326
#define SHELL_MESSAGE_QUIT      328
#define CCALL_MESSAGE_FOPEN     330
#define CCALL_MESSAGE_FCLOSE    332
#define CCALL_MESSAGE_FRENAME   334
#define CCALL_MESSAGE_FSIZE     336
#define CCALL_MESSAGE_FSEEK     338
#define CCALL_MESSAGE_FREAD     340
#define CCALL_MESSAGE_FWRITE    342
#define CCALL_MESSAGE_FCHECK    344
#define CCALL_MESSAGE_FFLUSH    346
#define CCALL_MESSAGE_LCDWR     348
#define CB_MESSAGE_ID		    400

typedef struct {
    VM_TIMER_ID_PRECISE timer_id;
    int					cb_ref;
    VMUINT32			runs;
    VMUINT32			pruns;
    VMUINT32			failed;
    VMUINT8				busy;
    VMUINT8				paused;
} timer_info_t;

typedef struct {
    int		uart_id;
    int		cb_ref;
    VMUINT8	busy;
    int		bufptr;
    VMUINT8	buffer[UART_BUFFER_LEN];
} uart_info_t;

typedef struct {
    VMINT	handle;
    int		type;
    int		connected;
    int		cb_ref;
    vm_soc_address_t address;
    char*	send_buf;
    int		bufsize;
    int		bufptr;
} net_info_t;


typedef enum
{
    CB_FUNC_INT = CB_MESSAGE_ID+1,
	CB_FUNC_TIMER,
	CB_FUNC_ADC,
	CB_FUNC_BT_RECV,
	CB_FUNC_BT_CONNECT,
	CB_FUNC_BT_DISCONNECT,
	CB_FUNC_SMS_LIST,
	CB_FUNC_SMS_READ,
	CB_FUNC_SMS_NEW,
	CB_FUNC_HTTPS_HEADER,
	CB_FUNC_HTTPS_DATA,
	CB_FUNC_DOTTY,
	CB_FUNC_REBOOT,
	CB_FUNC_MQTT_TIMER,
	CB_FUNC_MQTT_MESSAGE,
	CB_FUNC_MQTT_DISCONNECT,
	CB_FUNC_UART_RECV,
	CB_FUNC_NET,
	CB_FUNC_EINT
} CB_FUNC_TYPE;


typedef struct {
	char	*cpar1;
	char	*cpar2;
	int		ipar1;
	int		ipar2;
	int		ipar3;
	int		busy;
} cfunc_params_t;

typedef struct {
	int		nargs;
	int		nresults;
	int		busy;
} cbfunc_params_t;

typedef struct {
	int		cb_ref;
	int		par;
	int		busy;
} cb_func_param_int_t;

typedef struct {
	int					cb_ref;
	int					pin;
	VMUINT32			state;
	VMUINT32			count;
	VM_TIME_UST_COUNT	time;
	int					busy;
} cb_func_param_eint_t;

typedef struct {
	net_info_t *net_info;
	int		event;
	int		busy;
} cb_func_param_net_t;

typedef struct {
	int		cb_ref;
	int		recv_ref;
	int		connect_ref;
	int		disconnect_ref;
	int		connected;
	int		id;
	int		status;
	char	addr[18];
	char    *recvbuf;
	int		buflen;
	int		bufptr;
	int		busy;
} cb_func_param_bt_t;

typedef struct {
	int		cb_ref;
	int		len;
	int		more;
	char    *reply;
	int		busy;
} cb_func_param_httpsdata_t;

typedef struct {
	int		cb_ref;
	int		len;
	char    *header;
	int		busy;
} cb_func_param_httpsheader_t;

typedef struct {
	int		cb_ref;
	vm_gsm_sms_query_message_callback_t* list;
	int		busy;
} cb_func_param_smslist_t;

typedef struct {
	int		cb_ref;
	int		stat;
	vm_gsm_sms_read_message_data_callback_t* msg;
	int		busy;
} cb_func_param_smsread_t;

typedef struct {
	int		cb_ref;
	vm_gsm_sms_event_new_sms_t* msg;
	int		busy;
} cb_func_param_smsnew_t;

typedef struct {
	int			cb_ref;
	VMUINT32	ival;
	float		fval;
	int			chan;
	int			busy;
} cb_func_param_adc_t;


lua_State *shellL;			// Lua state
lua_State *ttyL;			// Lua state

VM_THREAD_HANDLE g_main_handle;
VM_THREAD_HANDLE g_ttythread_handle;
VM_THREAD_HANDLE g_shellthread_handle;

vm_mutex_t lua_func_mutex;
vm_thread_message_t g_shell_message;
vm_thread_message_t g_fcall_message;
vm_thread_message_t g_cbcall_message;

int g_shell_result;

VM_SIGNAL_ID g_shell_signal;
VM_SIGNAL_ID g_tty_signal;
//VM_SIGNAL_ID g_reboot_signal;

lua_CFunction g_CCfunc;
cfunc_params_t g_CCparams;
cbfunc_params_t g_CBparams;
int CCwait;

VM_DCL_OWNER_ID g_owner_id;  // Module owner of APP
VM_DCL_HANDLE retarget_usb_handle;
VM_DCL_HANDLE retarget_uart1_handle;
int retarget_target;

vm_mutex_t retarget_rx_mutex;

VMINT32 shell_thread(VM_THREAD_HANDLE thread_handle, void* user_data);
VMINT32 tty_thread(VM_THREAD_HANDLE thread_handle, void* user_data);
void l_message (const char *pname, const char *msg);
//void shell_docall(lua_State *L);
int remote_CCall(lua_CFunction func);
void remote_lua_call(VMUINT16 type, void *params);
int file_exists(const char *filename);

//void _mutex_lock(void);
//void _mutex_unlock(void);

#endif // __SHELL_H__
