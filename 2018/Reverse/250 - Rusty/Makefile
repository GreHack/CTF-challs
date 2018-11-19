BIN:=target/debug/rusty
RBIN:=target/release/rusty

all: debug release

debug: $(BIN)

release: $(RBIN)

$(BIN):
	cargo build

$(RBIN):
	cargo build --release
	strip $@

zip:
	cp $(RBIN) ./
	zip rusty.zip rusty
	rm rusty

clean:
	rm -fr target
	rm -fr rusty.zip
