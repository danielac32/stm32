#include <os.h>


 
local uint32_t *prepare_task_stack(uint32_t *task_stack,uint32_t size,void *entry, uint32_t nargs, void *parg){
  task_stack += size - sizeof(context_t); /* End of task_stack, minus what we are about to push */
  context_t *ctx = (context_t*)task_stack;

  ctx->r0 = (uint32_t) nargs;
  ctx->r1 = (uint32_t) parg;
  ctx->lr = (uint32_t) thread_terminate;
  ctx->pc = (uint32_t) entry;
  ctx->psr = (uint32_t) 0x01000000; /* PSR Thumb bit */
  return task_stack;
}

void terminal()
{
	while (1) {
        printf("%s",SHELL_PROMPT);
        while(!uart0_available()){
               //PEND_SV();
        }
        while(uart0_available()){
              len = readBytes(buf,sizeof(buf));
        }
        if (len == EOF) {
           break;
        }
        if (len <= 1) {
            continue;
        }

        buf[len] = SH_NEWLINE;  /* terminate line */

  

    ntok = lexan(buf, len, tokbuf, &tlen, tok, toktyp);

    /* Handle parsing error */

    if (ntok == SYSERR) {
      printf("%s\n",SHELL_SYNERRMSG);
      continue;
    }

    /* If line is empty, go to next input line */

    if (ntok == 0) {
      printf( "\n");
      continue;
    }
    
    for (j = 0; j < ncmd; j++) {
      src = cmdtab[j].cname;
      cmp = tokbuf;
      diff = false;
      while (*src != NULLCH) {
        if (*cmp != *src) {
          diff = true;
          break;
        }
        src++;
        cmp++;
      }
      if (diff || (*cmp != NULLCH)) {
        continue;
      } else {
        break;
      }
    }

    /* Handle command not found */

    if (j >= ncmd) {
        printf("command %s not found %d\n",tokbuf,len);
        continue;
    }else{
      for (i=0; i<ntok; i++) {
           args[i] = &tokbuf[tok[i]];
      }

      if (cmdtab[j].cbuiltin) {
          (*cmdtab[j].cfunc)(ntok, args);
      }else{
          task_t *tmp;
          int pid=create(cmdtab[j].cfunc,STACK_SIZE,1,cmdtab[j].cname,ntok); 
          tmp =& task[pid];
          for (int i=0; i < ntok; i++){
               tmp->parg[i] = &tokbuf[tok[i]];
          }
          tmp->sp =  prepare_task_stack(tmp->prstkbase,tmp->prstklen,tmp->paddr,ntok,&tmp->parg[0]);
          ready(pid);
      }
    

        /*printf("ok");
        printf(" %d ",j);
      for (int i=0; i<ntok; i++) {
          args[i] = &tokbuf[tok[i]];
          printf("%s",args[i]);
          printf(" ");
      }
      if(!strcmp(args[0],"create")){
          //ready(create((void *)t3,4096,1,"task2",2,"hola","daniel"));
      }else if(!strcmp(args[0],"kill")){
          //kill(atoi(args[1]));
      }else if(!strcmp(args[0],"mem")){
          list();
      }
      printf("\n");   */
    }
  }
}