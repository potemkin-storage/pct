#!/bin/bash
# kiwinatra, 2026 (c)

set -e

TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT

git clone https://github.com/potemkin-storage/pct "$TMP/pct"
cd "$TMP/pct"

make

INSTALL_DIR="${PCT_INSTALL_DIR:-/usr/local/bin}"
if [ ! -w "$INSTALL_DIR" ]; then
    INSTALL_DIR="$HOME/.local/bin"
    mkdir -p "$INSTALL_DIR"
fi

install -m 755 pct "$INSTALL_DIR/pct"

# make it reachable from anywhere
case ":$PATH:" in
    *":$INSTALL_DIR:"*) ;;
    *)
        SHELL_RC="$HOME/.bashrc"
        [ -n "$ZSH_VERSION" ] && SHELL_RC="$HOME/.zshrc"
        echo "export PATH=\"$INSTALL_DIR:\$PATH\"" >> "$SHELL_RC"
        echo "added $INSTALL_DIR to PATH in $SHELL_RC (restart shell)"
        ;;
esac

echo "pct installed to $INSTALL_DIR/pct"