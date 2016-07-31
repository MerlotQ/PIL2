################################################################################
#Applications Top Makefile.
################################################################################
subdirs = Tests \
          ClassLoader_Test \
          Camera_Test CV_Test GUI_Test Svar_Test \
          Timer_Test 


all : $(subdirs)
	@for dir in $(subdirs);do \
	make -C $$dir; done

clean : $(subdirs)
	@for dir in $(subdirs);do \
	make clean -C $$dir; done

