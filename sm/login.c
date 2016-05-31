
#include <sm.h>

#include <message.h>
#include <message_const.h>

#define SM_CONST_ERROR_TOKENTIME "{\"result_code\": -2,\"result_msg\" : \"tokentime is expire\",}"
#define SM_CONST_ERROR_UID "{\"result_code\": -1,\"result_msg\" : \"token is null\",}"
#define SM_CONST_SUCCESS_INSERT	"{\"result_code\": 0,\"result_msg\" : \"success\",\"date\":\"%s\"}"

static char* sm_success_insert(char* uid, char* body, int* date_len){

	void* msg = (void*)message_init_no_data();

	message_set_body(msg, body, strlen(body));
	message_set_type(msg, Message_Login_Type);

	int size = 0;
	message_serialization_size(msg, &size);
	*date_len = size;

	char* m = malloc(sizeof(char)*size);
	message_to_serialization(msg, m, sizeof(char)*size);

	message_destory(msg);
	return m;

}

// 返回的值需要释放
char* sm_error_msg(char* uid, char* body, int* data_len){
	void* msg = (void*)message_init_no_data();
	message_set_from(msg, uid, strlen(uid));
	message_set_body(msg, body, strlen(body));


	int size = 0;
	message_serialization_size(msg, &size);
	*data_len = size;

	char* m = malloc(sizeof(char)*size);
	message_to_serialization(msg, m, size*sizeof(char));

	message_destory(msg);
	return m;

}

char* sm_login_auth(sqlite_db_t db, char* data, int len, int* data_len){

	db = sqlite_db_open("/Cajian/imServer/db/imserver.db");

	void* msg = (void*)message_parse_from_data(data, len);
	if (!msg){
		sqlite_db_close(db);
		return "";
	}

	char* uid = (char*)message_get_from(msg);
	char* token = (char*)message_get_body(msg);

	// 1.判断token和uid是否为空
	if (uid == NULL || token == NULL){
		char* result = sm_error_msg(uid, SM_CONST_ERROR_UID, data_len);
		sqlite_db_close(db);
		message_destory(msg);
		return result;
	}

	time_t t = time(NULL);
	char buf[20] = "";
	snprintf(buf, 20, "%d", (int)t);

	char bodys[sizeof(SM_CONST_SUCCESS_INSERT) + 20] = "";
	snprintf(bodys, (sizeof(SM_CONST_SUCCESS_INSERT) + 20), SM_CONST_SUCCESS_INSERT, buf);

	char* result = sm_success_insert(uid,bodys,data_len);

	sqlite_db_close(db);
	message_destory(msg);
	return result;

	// 2.判断该用户是否在数据库里，如果不在，插入，返回成功
// 	int isexist = sqlite_select_uid_is_exist(db, uid);
// 	if (!isexist){
// 		sqlite_login_insert(db, uid, token, buf, "864000");
// 		char bodys[sizeof(SM_CONST_SUCCESS_INSERT) + 20] = "";
// 		snprintf(bodys, (sizeof(SM_CONST_SUCCESS_INSERT) + 20), SM_CONST_SUCCESS_INSERT, buf);
// 
// 		char* result = sm_success_insert(uid, bodys,data_len);
// 		free(uid);
// 		free(token);
// 		free(body);
// 		login_request_free(login_request);
// 		sqlite_db_close(db);
// 		return result;
// 	}
// 
// 	// 3.判断该uid在数据库里的tokentime是否超时，默认是10天时间
// 	char* tokentime = sqlite_login_get_tokentime(db, uid);
// 	if (tokentime){
// 		int time = atoi(tokentime);
// 		int currentTime = (int)t;
// 		if (currentTime - time > 10 * 24 * 60 * 60){
// 			// 时间过长，需要重新登录
// 			char* result = sm_error_msg(uid, SM_CONST_ERROR_TOKENTIME, data_len);
// 			free(tokentime);
// 			free(uid);
// 			free(token);
// 			free(body);
// 			login_request_free(login_request);
// 			sqlite_db_close(db);
// 			return result;
// 		}
// 	}
// 
// 	// 4.更新token和tokentime
// 	sqlite_login_update_token_tokentime_base_uid(db, uid, token, buf);
// 	free(uid);
// 	free(token);
// 	free(body);
// 	login_request_free(login_request);
// 
// 	// 5.返回成功信息
// 	char bodys[sizeof(SM_CONST_SUCCESS_INSERT) + 20] = "";
// 	snprintf(bodys, sizeof(SM_CONST_SUCCESS_INSERT) + 20, SM_CONST_SUCCESS_INSERT, buf);
// 	char* result = sm_success_insert(uid, bodys, data_len);
// 	sqlite_db_close(db);
// 	return result;
}
