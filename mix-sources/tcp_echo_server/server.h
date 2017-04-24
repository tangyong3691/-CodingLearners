
#ifndef SERVER_COMMON_HEAD
#define SERVER_COMMON_HEAD

#define LENGTH_OF_LISTEN_QUEUE 1024

#define mutex_type pthread_mutex_t*

typedef void *et_item_hanle;	//
typedef struct et_node {
	et_item_hanle handle;	//
	struct et_node *next;	//

} *et_list;

struct tcp_socket_client {
	int socket_fd;
	char tcp_client_ddr[20];
	int port;
	int time_out;
	char userid[66];
	int flag;
	int keep_alive;
	unsigned short packetid;
	int need_write_size;
	char *need_write_ptr;
};

typedef struct {
	/** A token identifying the successful request. Can be used to refer to the request later. */
	//et_context_token token;
	/** A union of the different values that can be returned for subscribe, upload file, down file ,unsubscribe and publish. */
	union {

		/* For connect, the server connected to, ET version used, and session_present flag */
		struct {
			char *server_uri;
			int et_version;
			int session_present;
		} connect;
		/*for request user state */
		struct {
			int user_state;
			char *user_id;
			int user_id_len;
		} state;

	} alt;
} et_context_success_data;

typedef struct _et_server_data {
	int type;
	int port;
	char addr[20];
	char userid[128];
} et_server_data;

typedef struct {
	/** A token identifying the failed request. */
	//et_context_token token;
	/** A numeric code identifying the error. */
	int code;
	/** Optional text explaining the error. Can be NULL. */
	char *message;

	union {			/* A union of the different values for server or file */
		/* server data struct */
		et_server_data server_data;
		char *file_name;
		int discover_fail_type;
	} alt;

} et_context_failure_data;

typedef void et_context_on_success(void *context,
				   et_context_success_data * response);

typedef void et_context_on_failure(void *context,
				   et_context_failure_data * response);

typedef struct {
	/**
    * A pointer to a callback function to be called if the API call successfully
    * completes.  Can be set to NULL, in which case no indication of successful
    * completion will be received.
    */
	et_context_on_success *on_success;
	/**
    * A pointer to a callback function to be called if the API call fails.
    * Can be set to NULL, in which case no indication of unsuccessful
    * completion will be received.
    */
	et_context_on_failure *on_failure;
	/**
    * A pointer to a callback function to be called if the on query
    * (abandoned, so you should not use it)
    */
	//te_on_query* pfn_on_query;
	/**
    * A pointer to a callback function to be called if a client connect
    */
	//te_on_client_connect *pfn_on_connect;
	/**
	* A pointer to any application-specific context. The
    * the <i>context</i> pointer is passed to success or failure callback functions to
    * provide access to the context information in the callback.
    */
	void *context;
	//et_context_token token;
} et_context_on_query_options;

typedef void *et_context;

typedef struct {
	char magic[36];
	int usr_login_stat;
	void *callback_context;
	//et_callback_handles callback;
	//int socketfd;
	//et_server_data mqtt_servrdata;
	et_list conect_server_list;
	et_list connect_tcp_client_list;
	mutex_type local_imsrv_lock;
	int mqtt_server_flag;
	int inner_server_flag;
	int connet_thread_flag;
	int discove_thread_flag;
	int discove_mqtt_thread_flag;
	int server_thread_flag;
	int persistence_type;
	et_context_on_query_options query_data;
	void *persistence_context;
	char local_ip[32];
	char app_key[66];
	char secret_key[66];
	char load_uri[66];
	int load_port;
	char userid[128];

	char net_interface_name[16];

	int local_server_listen_port;
	int local_server_auto_send_cycle;
	int local_server_auto_send_data_size;
} et_context_data;

#endif
