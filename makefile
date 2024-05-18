# Generated automatically from Makefile.in by configure.
CC=g++
CFLAGS=-g -I/usr/local/openssl/include/  -Wall -w
LD=-L/usr/local/openssl/lib  -lssl -lcrypto -ldl -lpthread

DISTDIR=Test
DISTFILE = ${DISTDIR}.tar.gz


LIB= -lcurl

DIST=common.cpp \
	common.h \
	HttpProtocol.cpp \
	HttpProtocol.h \
	MyWebServer.cpp \
	bm.h \
	bm.cpp \
	Http_client.cpp \
	Http_client.h \

all: MyWebServer Http_client	
  
MyWebServer: 
	@$(CC) $(CFLAGS) common.cpp MyWebServer.cpp HttpProtocol.cpp  bm.cpp -o MyWebServer $(LD)

Http_client: 
	@$(CC) $(CFLAGS) Http_client.cpp  -o Http_client $(LD) $(LIB)
clean:	
	@rm  MyWebServer Http_client
dist:
	rm -rf ${DISTDIR}; mkdir ${DISTDIR}
	cp ${DIST} ${DISTDIR}
	rm -f ${DISTFILE}
	tar cf - ${DISTDIR} | gzip > ${DISTFILE}
