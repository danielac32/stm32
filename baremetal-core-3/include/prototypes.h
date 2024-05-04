
extern void OTG_FS_IRQHandler(void);
extern void init_mem();
extern void uartinit();
extern void mount_fs();
/* in file clkhandler.c */
extern	void clkhandler(void);
/* in file panic.c */
extern	void	panic(char *);
/* in file platinit.c */
extern	void	platinit(void);
extern uint32 disable();
extern void restore(uint32);
extern void halt();

extern int getpid();
extern void load_context(void* psp);
extern void* save_context();

extern uint32 *prepare_task_stack(uint32 *task_stack,uint32 size,void *entry, uint32 nargs, void *parg);
extern int create(
      int       (*procaddr)(),  /* procedure address        */
      int   ssize,      /* stack size in bytes      */
      int       priority,   /* process priority > 0     */
      char      *name,      /* name (for debugging)     */
      int   nargs,      /* number of args that follow   */
      ...
    );
extern void ready(int pid);
extern void sleep(uint32 ticks);
extern void kill(int pid);
extern void enable();
extern int shell(int argc, char *argv[]);
extern void	panic (char	*msg);


