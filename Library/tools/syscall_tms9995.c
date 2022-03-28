/*
 *	Generate the syscall functions
 *
 *	Mostly TBA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syscall_name.h"

static char namebuf[128];

static void write_call(int n)
{
  FILE *fp;
  snprintf(namebuf, 128, "fuzixtms9995/syscall_%s.s",syscall_name[n]);
  fp = fopen(namebuf, "w");
  if (fp == NULL) {
    perror(namebuf);
    exit(1);
  }
  fprintf(fp, "\t.code\n\n");
  fprintf(fp, "\t.export _%s\n\n", syscall_name[n]);
  fprintf(fp, "_%s:\n\tli r2, 0x%x\n", syscall_name[n], n << 8);
  fprintf(fp, "\tdect r13\n");
  fprintf(fp, "\tmov r11,*r13\n");
  fprintf(fp, "\tci r0, 0\n");
  fprintf(fp, "\tblwp @head\n");
  fprintf(fp, "\tjeq @noerror\n");
  fprintf(fp, "\tmov r0,@_errno\n");
  fprintf(fp, "noerror:\n");
  fprintf(fp, "\tb *r13+\n");
  fclose(fp);
}

static void write_call_table(void)
{
  int i;
  for (i = 0; i < NR_SYSCALL; i++)
    write_call(i);
}

static void write_makefile(void)
{
  int i;
  char path[256];
  FILE *fp;

  fp = fopen("fuzixtms9995/Makefile", "w");
  if (fp == NULL) {
    perror("Makefile");
    exit(1);
  }
  fprintf(fp, "# Autogenerated by tools/syscalltms9995\n");
  fprintf(fp, "ASRCS = syscall_%s.s\n", syscall_name[0]);
  for (i = 1; i < NR_SYSCALL; i++)
    fprintf(fp, "ASRCS += syscall_%s.s\n", syscall_name[i]);
  fprintf(fp, "\n\nASRCALL = $(ASRCS)\n");
  fprintf(fp, "\nAOBJS = $(ASRCALL:.s=.o)\n\n");
  fprintf(fp, "../syslib.lib: $(AOBJS)\n");
  fprintf(fp, "\techo $(AOBJS) >syslib.l\n");
  fprintf(fp, "\tar rc ../syslib.lib $(AOBJS)\n\n");
  fprintf(fp, "$(AOBJS): %%.o: %%.s\n");
  fprintf(fp, "\tas9995 $<\n\n");
  fprintf(fp, "clean:\n");
  fprintf(fp, "\trm -f $(AOBJS) $(ASRCS) *~\n\n");
  fclose(fp);
}

int main(int argc, char *argv[])
{
  write_makefile();
  write_call_table();
  exit(0);
}