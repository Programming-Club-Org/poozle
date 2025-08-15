#!/usr/bin/env bash
set -e

# --- 1. Prefer any existing clang-format ---
CLANG_FORMAT_CMD="clang-format"

# Function to install clang-format on Windows (MinGW64)
install_clang_format_windows() {
    echo "Installing clang-format-16 on Windows (MinGW64)..."

    if ! command -v pacman &>/dev/null; then
        echo "MSYS2 pacman not found. Please install MSYS2 from https://www.msys2.org/ and re-run."
        exit 1
    fi

    # Update package database
    pacman -Sy --noconfirm

    # Install clang-format (part of clang-tools-extra)
    pacman -S --needed --noconfirm mingw-w64-x86_64-clang-tools-extra

    if command -v clang-format &>/dev/null; then
        CLANG_FORMAT_CMD="clang-format"
    else
        echo "clang-format installation failed."
        exit 1
    fi
}

# --- 2. Check if clang-format exists ---
if ! command -v "$CLANG_FORMAT_CMD" &>/dev/null; then
    echo "$CLANG_FORMAT_CMD not found. Attempting to install clang-format-16..."

    case "$OSTYPE" in
        linux-gnu*)
            sudo apt update
            sudo apt install -y clang-format-16
            CLANG_FORMAT_CMD="clang-format-16"
            ;;
        darwin*)
            if ! command -v brew &>/dev/null; then
                echo "Homebrew not found. Please install Homebrew first."
                exit 1
            fi
            brew install llvm@16
            LLVM_PATH="$(brew --prefix llvm@16)/bin/clang-format"
            if [[ -x "$LLVM_PATH" ]]; then
                CLANG_FORMAT_CMD="$LLVM_PATH"
            else
                echo "Failed to find clang-format in llvm@16 bin directory."
                exit 1
            fi
            ;;
        msys*|cygwin*|win32)
            install_clang_format_windows
            ;;
        *)
            echo "Unsupported OS. Please install clang-format manually."
            exit 1
            ;;
    esac
else
    echo "$CLANG_FORMAT_CMD is already installed."
fi

# --- 3. Run clang-format on all .cpp, .hpp, .c, .h files ---
echo "Running $CLANG_FORMAT_CMD on source files..."
find . -type f \( -iname "*.cpp" -o -iname "*.hpp" -o -iname "*.c" -o -iname "*.h" \) \
    -exec "$CLANG_FORMAT_CMD" -i {} +

echo "Formatting complete!"
