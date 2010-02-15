TEMPLATE=	subdirs

SUBDIRS	=	libroll

PSUBDIRS =	qroll \
		qrsndconv

darwin:PSUBDIRS	+= mac-frontend
