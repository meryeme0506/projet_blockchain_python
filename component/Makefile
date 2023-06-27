CPPFLAGS += `python3-config --includes`
CPPFLAGS += -I/usr/include/openssl
LIBS = `python3-config --ldflags` -lssl -lcrypto

COMPOSANT = bip39

LIBSO = ${COMPOSANT}.so
LIBSO_OBJS = ${COMPOSANT}.o sha256.o


all: ${LIBSO}

include ../gcc.mk
include ../pybind11.mk

test: $(LIBSO)
	echo "Running test.py"
	python3 test.py

clean:
	rm -f ${LIBSO}  ${LIBSO_OBJS}

${LIBSO}: ${LIBSO_OBJS}
	$(CXX) -shared -o $@ $^ $(LIBS)


