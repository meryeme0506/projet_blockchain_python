CPPFLAGS += `python3-config --includes`
CPPFLAGS += -I/usr/include/openssl
LIBS=`python3-config --ldflags`
LIBS += -lssl -lcrypto

COMPOSANT = bip39

LIBSO = ${COMPOSANT}.so
LIBSO_OBJS = ${COMPOSANT}.o 


all: ${LIBSO}

include ../gcc.mk
include ../pybind11.mk

test: $(LIBSO)
	echo "Test execution"
	python3 -m pytest -s test.py

	
clean:
	rm -f ${LIBSO}  ${LIBSO_OBJS}


${DESTINATION_LIBSO}: ${LIBSO}
	cp $< $@
