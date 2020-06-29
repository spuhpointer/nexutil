TESTS = test/*.js

all: test

build: clean compile

compile:
	npm install .
	npm run install

test: build
	@./node_modules/nodeunit/bin/nodeunit \
		$(TESTS)

clean:
	rm -rf node_modules  lib  build-tmp-napi-v1  build  package-lock.json


.PHONY: clean test build
