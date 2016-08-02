################################################################################
#Applications Top Makefile.
################################################################################
subdirs = Tests \
          ClassLoaderTest \
          CameraTest CVTest GUI_Test SvarTest \
          TimerTest 


all : $(subdirs)
	@for dir in $(subdirs);do \
	make -C $$dir; done

clean : $(subdirs)
	@for dir in $(subdirs);do \
	make clean -C $$dir; done

