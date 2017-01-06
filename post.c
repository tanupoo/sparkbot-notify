/*
 * Copyright (C) 2000 Shoichi Sakane <sakane@tanu.org>, All rights reserved.
 * See the file LICENSE in the top level directory for more details.
 */
#include <sys/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <err.h>

#include <curl/curl.h>

#include "ioxutil/ioxutil.h"
#include "xcurl_write_data.h"
#include "xcurl_ext_debug.h"

char *g_server_url = NULL;
char *g_auth_key = NULL;
char *g_room_id = NULL;
char *g_dialect = NULL;
int g_frequency = 0;

int f_debug = 0;

char *prog_name = NULL;

void
usage()
{
	printf(
"Usage: %s [-dh]\n"
	, prog_name);

	exit(0);
}

int
get_caf_env()
{
	ioxutil_init();
	ioxutil_cafenv_dump();

	ioxutil_config_load();
	ioxutil_config_get("spark", "server_url", &g_server_url, NULL);
	ioxutil_config_get("spark", "auth_key", &g_auth_key, NULL);
	ioxutil_config_get("spark", "room_id", &g_room_id, NULL);
	ioxutil_config_get("spark", "dialect", &g_dialect, NULL);
	ioxutil_config_getint("spark", "frequency", &g_frequency, 60);

	return 0;
}

int
mk_text(char *buf, size_t buf_len)
{
	struct tm *tm;
	time_t t;
	size_t len;

	t = time(NULL);
	tm = localtime(&t);
	len = strftime(buf, buf_len, g_dialect, tm);

	return len < buf_len ? 0 : -1;
}

int
mk_body(char *msg, char *buf, int buf_len)
{
	int len;

	len = snprintf(buf, buf_len, "{\"roomId\":\"%s\",\"text\":\"%s\"}",
			g_room_id, msg);

	if (f_debug) {
		printf("DEBUG: body\n");
		printf("%s\n", buf);
		printf("----\n");
	}

	return len < buf_len ? 0 : -1;
}

int
post_data(CURL *curl)
{
	CURLcode res = 0;

	char *text_buf;
	int text_len = 128;

	char *msg_buf;
	int msg_len = 256;

	if ((text_buf = malloc(text_len)) == NULL)
		err(1, "ERROR: malloc(text_buf)");

	if ((msg_buf = malloc(msg_len)) == NULL)
		err(1, "ERROR: malloc(msg_buf)");

	/* set body */
	text_buf[0] = '\0';
	mk_text(text_buf, text_len);

	msg_buf[0] = '\0';
	mk_body(text_buf, msg_buf, msg_len);

	curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, msg_buf);

	/* submit */
	res = curl_easy_perform(curl);
	if(res != CURLE_OK) {
		err(1, "ERROR: curl_easy_perform() failed: %s",
			curl_easy_strerror(res));
	}

	free(text_buf);
	free(msg_buf);

	return 0;
}

int
run(void)
{
	CURL *curl;
	CURLcode res = 0;
	struct curl_slist *hdr;
	char *key_buf;
	int key_len = 256;
	struct xcurl_write_data *wd;

	get_caf_env();

	if ((key_buf = malloc(key_len)) == NULL)
		err(1, "ERROR: malloc(key_buf)");

	curl_global_init(CURL_GLOBAL_ALL);
 
	curl = curl_easy_init();
	if (!curl) {
		err(1, "ERROR: curl_easy_init() failed: %s",
				curl_easy_strerror(res));
	}

	/* set debug options */
	if (f_debug > 1)
		xcurl_set_ext_debug(curl);

	/* set callback */
	wd = xcurl_init_write_data();
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, wd);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, xcurl_write_data_cb);

	/* set header */
	snprintf(key_buf, key_len, "Authorization: Bearer %s", g_auth_key);
	if (f_debug)
		printf("DEBUG: keybuf=%s\n", key_buf);

	hdr = NULL;
	hdr = curl_slist_append(hdr, "Content-Type: application/json");
	hdr = curl_slist_append(hdr, key_buf);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hdr);

	curl_easy_setopt(curl, CURLOPT_URL, g_server_url);

	while (1) {
		post_data(curl);
		if (f_debug) {
			printf("wd->size=%d\n", (int)wd->size);
			if(wd->size) {
				printf("%s\n", wd->data);
			}
		}
		xcurl_rewind_write_data(wd);

		/* sleep for next */
		sleep(g_frequency);
	}

	xcurl_free_write_data(wd);

	/* clean up */
	curl_slist_free_all(hdr);
	curl_easy_cleanup(curl);
	curl_global_cleanup();

	free(key_buf);

	return 0;
}

int
main(int argc, char *argv[])
{
	int ch;

	prog_name = 1 + rindex(argv[0], '/');

	while ((ch = getopt(argc, argv, "dh")) != -1) {
		switch (ch) {
		case 'd':
			f_debug++;
			break;
		case 'h':
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc > 0)
		usage();

	run();

	return 0;
}
