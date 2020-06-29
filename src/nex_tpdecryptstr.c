/**
 * Call tpdecrypt from nodejs
 */

#include <assert.h>
#include <node_api.h>
#include <stdio.h>
#include <string.h>
#include <atmi.h>
#include <stdlib.h>
#include <ndebug.h>


/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/

#define GET_AND_THROW_LAST_ERROR(env)                                    \
    const napi_extended_error_info *error_info;                          \
    napi_get_last_error_info((env), &error_info);                        \
    bool is_pending;                                                     \
    napi_is_exception_pending((env), &is_pending);                       \
    /* If an exception is already pending, don't rethrow it */           \
    if (!is_pending) {                                                   \
      const char* error_message = error_info->error_message != NULL ?    \
        error_info->error_message :                                      \
        "empty error message";                                           \
      napi_throw_error((env), NULL, error_message);                      \
    }

#define NAPI_CALL(env, the_call) {                                       \
    if ((the_call) != napi_ok) {                                         \
        GET_AND_THROW_LAST_ERROR((env));                                 \
        goto out;							 \
    }                                                                    \
}

/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/
/**
 * call Enduro/X tpdecrypt() API call
 * @param env nodejs standard call
 * @param info call info requires 1 string argument (base64 encrypted data)
 * @return clear text value or excpetion
 */
napi_value nexutil_tpdecryptstr(napi_env env, napi_callback_info info) 
{
	size_t argc = 1;
	size_t str_len = 0;
	char buf[1024];
	long len;
	char *value0=NULL;
	napi_value args[1];
	napi_value ret=NULL;
	napi_valuetype valuetype0;
	
	NDRX_LOG(log_debug, "nexutil_tpdecryptstr called");

	NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

	if (argc < 1)
	{
		napi_throw_type_error(env, NULL, 
				      "Wrong number of arguments, needs data to decrypt");
		goto out;
	}

	NAPI_CALL(env, napi_typeof(env, args[0], &valuetype0));

	if (valuetype0 != napi_string)
	{
		napi_throw_type_error(env, NULL, "Wrong arguments");
		goto out;
	}
	
	/* get len */
	NAPI_CALL(env, napi_get_value_string_utf8(env, args[0], NULL, 0, &str_len));

	value0 = malloc(str_len + 1);

	if (NULL==value0)
	{
		napi_throw_type_error(env, NULL, "malloc() returnes NULL");
		goto out;
	}
	
	NAPI_CALL(env, napi_get_value_string_utf8(env, args[0], value0, 
						  str_len + 1, NULL));
	
	/* decryp the data block... */
	len = sizeof(buf);
	
	/* call Enduro */
	if (0!=tpdecrypt(value0, 0, buf, &len, TPEX_STRING))
	{
		napi_throw_error(env, tpecodestr(tperrno), tpstrerror(tperrno));
		goto out;
	}
	
	NAPI_CALL(env, napi_create_string_utf8(env, buf, len, &ret));
	
out:

	if (NULL!=value0)
	{
		free(value0);
	}

	return ret;
}

#define DECLARE_NAPI_METHOD(name, func)                                        \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

napi_value Init(napi_env env, napi_value exports)
{
	napi_property_descriptor addDescriptor = DECLARE_NAPI_METHOD("tpdecryptstr", nexutil_tpdecryptstr);
	NAPI_CALL(env, napi_define_properties(env, exports, 1, &addDescriptor));
out:
	return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
