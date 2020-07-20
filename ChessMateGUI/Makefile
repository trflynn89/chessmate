# Define the path to the source directory
SOURCE_ROOT := $(CURDIR)/src

# Define the project version
VERSION := $(shell cat $(CURDIR)/../VERSION.md)

.PHONY: build
.PHONY: clean
.PHONY: release
.PHONY: run
.PHONY: tests
.PHONY: setup
.PHONY: style

include /usr/src/fly/config.mk
include /usr/src/fly/flags.mk

CLASSPATH := "$(CURDIR):$(CURDIR)/jars/tablelayout.jar:$(CURDIR)/target/classes"

build:
	@echo -e "[$(CYAN)Compile$(DEFAULT) target]"
	$(Q)mvn compile

clean:
	@echo -e "[$(RED)Clean$(DEFAULT) target]"
	$(Q)mvn clean

release:
	$(Q)mvn clean compile assembly:single

run: build
	$(Q)java -cp $(CLASSPATH) com.flynn.chessmate.ChessMateGUI

tests:
	$(Q)mvn test

setup:
	$(Q)sudo apt install -y openjdk-14-jdk maven

	$(Q)mvn install:install-file \
		-Dfile=$(CURDIR)/jars/tablelayout.jar \
		-DgroupId=org.freixas.tablelayout \
		-DartifactId=TableLayout \
		-Dversion=1.0 \
		-Dpackaging=jar \
		-DgeneratePom=true

style:
	$(Q)clang-format -i $$(find $(SOURCE_ROOT) -name "*.java")
