/*
 *  This file defines the interface between top and the machine-dependent
 *  module.  It is NOT machine dependent and should not need to be changed
 *  for any specific machine.
 */

/*
 * The statics struct is filled in by machine_init.  Fields marked as
 * "optional" are not filled in by every module.
 */
struct statics
{
    char **procstate_names;
    char **cpustate_names;
    char **memory_names;
    char **swap_names;		/* optional */
    char **order_names;		/* optional */
    char **color_names;		/* optional */
    time_t boottime;		/* optional */
    struct {
	unsigned int fullcmds : 1;
	unsigned int idle : 1;
	unsigned int warmup : 1;
    } flags;
};

/*
 * the system_info struct is filled in by a machine dependent routine.
 */

#ifdef p_active     /* uw7 define macro p_active */
#define P_ACTIVE p_pactive
#else
#define P_ACTIVE p_active
#endif

struct system_info
{
    int    last_pid;
    double load_avg[NUM_AVERAGES];
    int    p_total;
    int    P_ACTIVE;     /* number of procs considered "active" */
    int    *procstates;
    int    *cpustates;
    long   *memory;
    long   *swap;
};

/* cpu_states is an array of percentages * 10.  For example, 
   the (integer) value 105 is 10.5% (or .105).
 */

/*
 * the process_select struct tells get_process_info what processes we
 * are interested in seeing
 */

struct process_select
{
    int idle;		/* show idle processes */
    int system;		/* show system processes */
    int fullcmd;	/* show full command */
    int uid;		/* only this uid (unless uid == -1) */
    char *command;	/* only this command (unless == NULL) */
};

/* routines defined by the machine dependent module */
int machine_init(struct statics *);
void get_system_info(struct system_info *);
caddr_t get_process_info(struct system_info *, struct process_select *, int);
char *format_header(char *);
char *format_next_process(caddr_t, char *(*)(int));
int proc_owner(int);
