#
# Makefile for the linux arm-specific parts of the memory manager.
#
# Note! Dependencies are done automagically by 'make dep', which also
# removes any old dependencies. DON'T put your own dependencies here
# unless it's something special (ie not a .c file).
#
# Note 2! The CFLAGS definition is now in the main makefile...

O_TARGET := mm.o
O_OBJS   := init.o memory.o fault-$(PROCESSOR).o mm-$(MACHINE).o

ifeq ($(PROCESSOR),armo)
 O_OBJS += kmalloc-arm.o proc-arm2,3.o
endif

ifeq ($(PROCESSOR),armv)
 O_OBJS += small_page.o proc-arm7tdmi.o
endif

include $(TOPDIR)/Rules.make

proc-arm2,3.o:  ../lib/constants.h
proc-arm6,7.o:  ../lib/constants.h
proc-sa110.o:   ../lib/constants.h

.PHONY: ../lib/constants.h
../lib/constants.h:
	@$(MAKE) -C ../lib constants.h

%.o: %.S
ifneq ($(CONFIG_BINUTILS_NEW),y)
	$(CC) $(CFLAGS) -D__ASSEMBLY__ -E $< | tr ';$$' '\n#' > ..tmp.s
	$(CC) $(CFLAGS) -c -o $@ ..tmp.s
	$(RM) ..tmp.s
else
	$(CC) $(CFLAGS) -D__ASSEMBLY__ -c -o $@ $<
endif
