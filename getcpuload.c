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
#include <err.h>

struct tweet_t {
	float level;
	char *msg;
} tw[] = {
	{  10., "..." },
	{  20., "よゆう" },
	{  50., "ばっちこーい！" },
	{  80., "まだいける！" },
	{ 100., "ち、ちょっと…" },
	{ 200., "もうだめ…" }
};

static int f_debug = 0;

const char *
get_msg(float level)
{
	int i;

	for (i = 0; i < sizeof(tw)/sizeof(tw[0]); i++) {
		if (level < tw[i].level)
			return tw[i].msg;
	}

	return "FATAL:";
}

int
get_cpuload(int *sum, int *sum3)
{
	FILE *fp;
	char buf[512];
	char *sv, *bp;
	int sm, sm3, num;

#ifdef __linux__
	if ((fp = fopen("/proc/stat", "r")) == NULL)
		err(1, "ERROR: fopen(/proc/stat)");
#else
	*sum = 0;
	*sum3 = 0;
	return 0;
#endif

	sm = sm3 = num = 0;
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		if (f_debug)
			printf("DEBUG: [%s]\n", buf);
		bp = buf;
		if ((sv = strsep(&bp, " ")) == NULL)
			break;
		if (strcmp("cpu", sv) != 0)
			break;
		while ((sv = strsep(&bp, " ")) != NULL) {
			int v;
			char *np;
			if (*sv == '\0')
				continue;
			v = strtol(sv, &np, 10);
			if (f_debug) {
				printf("DEBUG: [%d]\n", v);
			}
			if (*np != '\0' && *np != '\n') {
				warnx("WARN: [%s] is not a number.", sv);
				break;
			}
			if (num++ < 3)
				sm3 += v;
			sm += v;
		}
	}

	fclose(fp);

	if (!num)
		return -1;

	if (f_debug)
		printf("sum=%d sum3=%d\n", sm, sm3);

	*sum = sm;
	*sum3 = sm3;

	return 0;
}

#ifdef TEST
int
run(int period)
{
	int total_0, work_0;
	int total_1, work_1;
	float total_d, work_d, cpu_d;

	total_0 = work_0 = 0;
	while (1) {
		if (get_cpuload(&total_1, &work_1) < 0) {
			warnx("ERROR: failed to get cpu load.");
			continue;
		}
		if (work_0) {
			total_d = total_1 - total_0;
			work_d = work_1 - work_0;
			cpu_d = work_d / total_d * 100.;
			printf("%s (%.3f)\n", get_msg(cpu_d), cpu_d);
		}
		total_0 = total_1;
		work_0 = work_1;
		sleep(period);
	}

	return 0;
}

int
main(int argc, char *argv[])
{
	int ch;
	int period = 5;

	prog_name = 1 + rindex(argv[0], '/');

	while ((ch = getopt(argc, argv, "t:dh")) != -1) {
		switch (ch) {
		case 't':
			period = atoi(optarg);
			break;
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

	run(period);

	return 0;
}
#endif /* TEST */
