# Define the path to the source directory
SOURCE_ROOT := $(CURDIR)

# Define the project version
VERSION := $(shell cat $(SOURCE_ROOT)/../VERSION.md)

.PHONY: build
.PHONY: clean
.PHONY: run
.PHONY: release
.PHONY: tests
.PHONY: setup

include /usr/src/fly/config.mk
include /usr/src/fly/flags.mk

CLASSPATH := "$(SOURCE_ROOT):$(SOURCE_ROOT)/jars/tablelayout.jar:$(SOURCE_ROOT)/target/classes"

build:
	@echo -e "[$(CYAN)Compile$(DEFAULT) target]"
	$(Q)mvn compile

clean:
	@echo -e "[$(RED)Clean$(DEFAULT) target]"
	$(Q)mvn clean

run: build
	$(Q)java -cp $(CLASSPATH) com.flynn.chessmate.ChessMateGUI

release:
	$(Q)mvn clean compile assembly:single

tests:
	$(Q)mvn test

setup:
	$(Q)sudo apt install -y openjdk-14-jdk maven

	$(Q)mvn install:install-file \
		-Dfile=$(SOURCE_ROOT)/jars/tablelayout.jar \
		-DgroupId=org.freixas.tablelayout \
		-DartifactId=TableLayout \
		-Dversion=1.0 \
		-Dpackaging=jar \
		-DgeneratePom=true
