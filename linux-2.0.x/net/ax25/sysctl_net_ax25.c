/* -*- linux-c -*-
 * sysctl_net_ax25.c: sysctl interface to net AX.25 subsystem.
 *
 * Begun April 1, 1996, Mike Shaver.
 * Added /proc/sys/net/ax25 directory entry (empty =) ). [MS]
 */

#include <linux/mm.h>
#include <linux/sysctl.h>
#include <net/ax25.h>

static int min_ipdefmode[] = {0},	max_ipdefmode[] = {1};
static int min_axdefmode[] = {0},	max_axdefmode[] = {1};
static int min_backoff[] = {0},		max_backoff[] = {2};
static int min_conmode[] = {0},		max_conmode[] = {2};
static int min_window[] = {1},		max_window[] = {7};
static int min_ewindow[] = {1},		max_ewindow[] = {63};
static int min_t1[] = {1},		max_t1[] = {30 * AX25_SLOWHZ};
static int min_t2[] = {1},		max_t2[] = {20 * AX25_SLOWHZ};
static int min_t3[] = {0},		max_t3[] = {3600 * AX25_SLOWHZ};
static int min_idle[] = {0},		max_idle[] = {65535 * AX25_SLOWHZ};
static int min_n2[] = {1},		max_n2[] = {31};
static int min_paclen[] = {1},		max_paclen[] = {512};

static struct ctl_table_header *ax25_table_header;

static ctl_table ax25_table[AX25_MAX_DEVICES + 1];

static ctl_table ax25_dir_table[] = {
	{NET_AX25, "ax25", NULL, 0, 0555, ax25_table},
	{0}
};

static ctl_table ax25_root_table[] = {
	{CTL_NET, "net", NULL, 0, 0555, ax25_dir_table},
	{0}
};

static const ctl_table ax25_param_table[] = {
	{NET_AX25_IP_DEFAULT_MODE, "ip_default_mode",
	 NULL, sizeof(int), 0644, NULL,
	 &proc_dointvec_minmax, &sysctl_intvec, NULL,
	 &min_ipdefmode, &max_ipdefmode},
	{NET_AX25_DEFAULT_MODE, "ax25_default_mode",
	 NULL, sizeof(int), 0644, NULL,
	 &proc_dointvec_minmax, &sysctl_intvec, NULL,
	 &min_axdefmode, &max_axdefmode},
	{NET_AX25_BACKOFF_TYPE, "backoff_type",
	 NULL, sizeof(int), 0644, NULL,
	 &proc_dointvec_minmax, &sysctl_intvec, NULL,
	 &min_backoff, &max_backoff},
	{NET_AX25_CONNECT_MODE, "connect_mode",
	 NULL, sizeof(int), 0644, NULL,
	 &proc_dointvec_minmax, &sysctl_intvec, NULL,
	 &min_conmode, &max_conmode},
	{NET_AX25_STANDARD_WINDOW, "standard_window_size",
	 NULL, sizeof(int), 0644, NULL,
	 &proc_dointvec_minmax, &sysctl_intvec, NULL,
	 &min_window, &max_window},
	{NET_AX25_EXTENDED_WINDOW, "extended_window_size",
	 NULL, sizeof(int), 0644, NULL,
	 &proc_dointvec_minmax, &sysctl_intvec, NULL,
	 &min_ewindow, &max_ewindow},
	{NET_AX25_T1_TIMEOUT, "t1_timeout",
	 NULL, sizeof(int), 0644, NULL,
	 &proc_dointvec_minmax, &sysctl_intvec, NULL,
	 &min_t1, &max_t1},
	{NET_AX25_T2_TIMEOUT, "t2_timeout",
	 NULL, sizeof(int), 0644, NULL,
	 &proc_dointvec_minmax, &sysctl_intvec, NULL,
	 &min_t2, &max_t2},
	{NET_AX25_T3_TIMEOUT, "t3_timeout",
	 NULL, sizeof(int), 0644, NULL,
	 &proc_dointvec_minmax, &sysctl_intvec, NULL,
	 &min_t3, &max_t3},
	{NET_AX25_IDLE_TIMEOUT, "idle_timeout",
	 NULL, sizeof(int), 0644, NULL,
	 &proc_dointvec_minmax, &sysctl_intvec, NULL,
	 &min_idle, &max_idle},
	{NET_AX25_N2, "maximum_retry_count",
	 NULL, sizeof(int), 0644, NULL,
	 &proc_dointvec_minmax, &sysctl_intvec, NULL,
	 &min_n2, &max_n2},
	{NET_AX25_PACLEN, "maximum_packet_length",
	 NULL, sizeof(int), 0644, NULL,
	 &proc_dointvec_minmax, &sysctl_intvec, NULL,
	 &min_paclen, &max_paclen},
	{0}	/* that's all, folks! */
};

void ax25_register_sysctl(void)
{
	int i, n, k;

	memset(ax25_table, 0x00, (AX25_MAX_DEVICES + 1) * sizeof(ctl_table));

	for (n = 0, i = 0; i < AX25_MAX_DEVICES; i++) {
		if (ax25_device[i].dev != NULL) {
			ax25_table[n].ctl_name     = n + 1;
			ax25_table[n].procname     = ax25_device[i].name;
			ax25_table[n].data         = NULL;
			ax25_table[n].maxlen       = 0;
			ax25_table[n].mode         = 0555;
			ax25_table[n].child        = ax25_device[i].systable;
			ax25_table[n].proc_handler = NULL;

			memcpy(ax25_device[i].systable, ax25_param_table, sizeof(ax25_device[i].systable));

			ax25_device[i].systable[AX25_MAX_VALUES].ctl_name = 0;	/* just in case... */

			for (k = 0; k < AX25_MAX_VALUES; k++)
				ax25_device[i].systable[k].data = &ax25_device[i].values[k];

			n++;
		}
	}

	ax25_table_header = register_sysctl_table(ax25_root_table, 1);
}

void ax25_unregister_sysctl(void)
{
	unregister_sysctl_table(ax25_table_header);
}
