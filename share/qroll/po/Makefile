
BASENAME = qroll
SRCPATH  = `cd ../../src; pwd`
LANGUAGES = en fr

all:	project

project:
	PRO=/tmp/qroll-linguist.$$$$; \
	printf 'TRANSLATIONS =' >> $${PRO}; \
	for i in ${LANGUAGES}; do \
	  echo ' \' >> $${PRO}; \
	  printf '  '`pwd`/$$i'/${BASENAME}.ts' >> $${PRO}; \
	done; \
	echo >> $${PRO}; \
	echo >> $${PRO}; \
	echo 'SOURCES = \' >> $${PRO}; \
	find $(SRCPATH) -name '*.cc' -exec echo "  {} \\" >> $${PRO} \; ; \
	${QTDIR}/bin/lupdate $${PRO}; \
	${QTDIR}/bin/lrelease $${PRO}; \
	rm $${PRO}

