#ifndef __curl_data_H__
#define __curl_data_H__

struct xcurl_write_data {
    size_t size;
    char* data;
};

void xcurl_free_write_data(struct xcurl_write_data *);
void xcurl_rewind_write_data(struct xcurl_write_data *);
struct xcurl_write_data *xcurl_init_write_data(void);
size_t xcurl_write_data_cb(void *, size_t, size_t, struct xcurl_write_data *);

#endif

