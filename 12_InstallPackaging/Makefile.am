SUBDIRS = src man

EXTRA_DIST = README.md autogen.sh Doxyfile

doc:
	doxygen Doxyfile

.PHONY: doc

GETTEXT_PACKAGE = myproject
localedir = $(datadir)/locale

POFILES = po/en.po po/ru.po
MO_EN = po/en.mo
MO_RU = po/ru.mo

all-local: $(MO_EN) $(MO_RU)

$(MO_EN): po/en.po
	$(MSGFMT) -o $@ $<

$(MO_RU): po/ru.po
	$(MSGFMT) -o $@ $<

install-data-local: all-local
	$(MKDIR_P) $(DESTDIR)$(localedir)/en/LC_MESSAGES
	$(MKDIR_P) $(DESTDIR)$(localedir)/ru/LC_MESSAGES
	$(INSTALL_DATA) $(MO_EN) $(DESTDIR)$(localedir)/en/LC_MESSAGES/$(GETTEXT_PACKAGE).mo
	$(INSTALL_DATA) $(MO_RU) $(DESTDIR)$(localedir)/ru/LC_MESSAGES/$(GETTEXT_PACKAGE).mo

uninstall-local:
	rm -f $(DESTDIR)$(localedir)/en/LC_MESSAGES/$(GETTEXT_PACKAGE).mo
	rm -f $(DESTDIR)$(localedir)/ru/LC_MESSAGES/$(GETTEXT_PACKAGE).mo

CLEANFILES = $(MO_EN) $(MO_RU)
EXTRA_DIST = $(POFILES)
