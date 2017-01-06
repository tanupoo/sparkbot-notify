/*
 * taken from http://stackoverflow.com/questions/13905774/in-c-how-do-you-use-libcurl-to-read-a-http-response-into-a-string
 */
#include <sys/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <err.h>

#include "xcurl_write_data.h"

void
xcurl_free_write_data(struct xcurl_write_data *wd)
{
	free(wd->data);
	free(wd);
}

void
xcurl_rewind_write_data(struct xcurl_write_data *wd)
{
	wd->size = 0;
	wd->data[0] = '\0';
}

struct xcurl_write_data*
xcurl_init_write_data()
{
	struct xcurl_write_data *wd;
	wd = malloc(sizeof(struct xcurl_write_data));
	if (wd == NULL)
		err(1, "ERROR: malloc(xcurl_write_data)");

	wd->size = 0;
	wd->data = malloc(4096);
	if(NULL == wd->data)
		err(1, "ERROR: malloc(xcurl_write_data.data)");

	wd->data[0] = '\0';

	return wd;
}

size_t
xcurl_write_data_cb(void *ptr, size_t size, size_t nmemb,
		struct xcurl_write_data *data)
{
	size_t index = data->size;
	size_t n = (size * nmemb);
	char* tmp;

	data->size += (size * nmemb);

#ifdef DEBUG
	fprintf(stderr, "DEBUG: write data at %p size=%ld nmemb=%ld\n",
			ptr, size, nmemb);
#endif
	tmp = realloc(data->data, data->size + 1); /* +1 for '\0' */

	if(tmp) {
		data->data = tmp;
	} else {
		if(data->data) {
			free(data->data);
		}
		fprintf(stderr, "ERROR: Failed to allocate memory.\n");
		return 0;
	}

	memcpy((data->data + index), ptr, n);
	data->data[data->size] = '\0';

	return size * nmemb;
}
