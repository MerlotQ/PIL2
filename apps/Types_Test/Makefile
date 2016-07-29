################################################################################
#Applications Top Makefile.
################################################################################
subdirs = SE3_Test SIM3_Test SO3_Test


all : $(subdirs)
	@for dir in $(subdirs);do \
	make -C $$dir --no-print-directory; done

clean :$(subdirs)
	@for dir in $(subdirs);do \
	make clean -C $$dir; done

