# Build directories
IFRED_DIR=$(shell pwd -P)
BUILD_DIR=${IFRED_DIR}/build
RELEASE_BUILD_DIR=${BUILD_DIR}/release
DEBUG_BUILD_DIR=${BUILD_DIR}/debug

.PHONY: release
release:
	mkdir -p ${RELEASE_BUILD_DIR} && \
	cd ${RELEASE_BUILD_DIR} && \
	cmake -DCMAKE_BUILD_TYPE=Release -GNinja ${IFRED_DIR} && \
	ninja

.PHONY: debug
debug:
	mkdir -p ${DEBUG_BUILD_DIR} && \
	cd ${DEBUG_BUILD_DIR} && \
	cmake -DCMAKE_BUILD_TYPE=Debug -GNinja ${IFRED_DIR} && \
	ninja

.PHONY: distclean
distclean:
	rm -fr ${BUILD_DIR}
