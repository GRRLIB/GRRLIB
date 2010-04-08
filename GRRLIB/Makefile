SUBDIRS:= `ls | egrep -v '^(\.svn)$$'`

all:
	@for i in $(SUBDIRS); do \
	  if test -e $$i/Makefile ; then \
	    echo "-----------------------------------------------------------";\
	    $(MAKE) -C $$i || { exit 1;} \
	  fi; \
	done;

clean:
	@rm -f *.bz2
	@for i in $(SUBDIRS); do \
	  if test -e $$i/Makefile ; then \
	    $(MAKE) -C $$i clean || { exit 1;} \
	  fi; \
	done;

install:
	@rm -f *.bz2
	@for i in $(SUBDIRS); do \
	  if test -e $$i/Makefile ; then \
	    $(MAKE) -C $$i install || { exit 1;} \
	  fi; \
	done;