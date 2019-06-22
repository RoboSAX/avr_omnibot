###############################################################################
#                                                                             #
# Makefile                                                                    #
# ========                                                                    #
#                                                                             #
# Version: 1.0.0                                                              #
# Date   : 22.07.19                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# See also:                                                                   #
#   https://github.com/RoboSAX/avr_omnibot                                    #
###############################################################################

SUBDIRS = 2014/ \
          \
          devel/peter/001_test_display/

###############################################################################
.PHONY: all clean $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	@echo "### compiling $@ ###"
	$(MAKE) -C $@ -f "$$ROBOLIB_MAKEFILE"

clean:
	@echo
	@echo "### clean ###"
	@for dir in $(SUBDIRS); do \
	    echo "# clean $$dir #"; \
	    $(MAKE) clean -C $$dir -f "$$ROBOLIB_MAKEFILE"; \
	done

###############################################################################
