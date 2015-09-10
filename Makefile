GUI = gtk #qt
CORE = core
SUBCLEAN = $(addsuffix .clean,$(GUI))

.PHONY: $(GUI) $(CORE)

all: $(CORE) qt_qmake $(GUI)

$(CORE):
	@$(MAKE) -C $@

qt_qmake: qt/qt.pro
	cd qt && qmake-qt4

$(GUI): core qt_qmake
	@$(MAKE) -C $@

clean:
	@if [ ! -f qt/Makefile ]; then make qt_qmake; fi;
	@for dir in $(GUI) $(CORE); do $(MAKE) clean -C $$dir; done
	@rm -rf bin/*

distclean: clean
	@if [ -f qt/Makefile ]; then make distclean -C qt; fi;

$(SUBCLEAN): %.clean:
	$(MAKE) -C $* clean

