################################################################################
#Applications Top Makefile.
################################################################################
subdirs = Test \
          CameraTest CV_Test SO3_Test ScSO3_Test SIM3_Test Svar_Test Timer_Test  \
          LogTest SE3_Test


all : $(subdirs)
	@for dir in $(subdirs);do \
	make -C $$dir --no-print-directory; done

clean :$(subdirs)
	@for dir in $(subdirs);do \
	make clean -C $$dir; done

