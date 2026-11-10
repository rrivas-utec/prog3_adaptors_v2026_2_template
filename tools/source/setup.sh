#!/usr/bin/env bash

set -euo pipefail
export DEBIAN_FRONTEND=noninteractive
SOURCE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# Keep this version aligned with the C++23 clang-tidy runner. Do not follow
# LLVM's development repository: a new release should be reviewed explicitly.
CLANG_VERSION=20

# Retry transient repository failures (including HTTP 503) per download.
# Also retry the command: some APT download paths return immediately on 503.
# Keep failures fatal once retries are exhausted; never skip dependencies.
apt_get() {
    local attempt status
    for attempt in 1 2 3; do
        if apt-get -o Acquire::Retries=5 \
            -o Acquire::http::Timeout=30 \
            -o Acquire::https::Timeout=30 "$@"; then
            return 0
        else
            status=$?
        fi
        if [[ "$attempt" -eq 3 ]]; then
            echo "ERROR: APT failed after 3 attempts (exit ${status})." >&2
            return "$status"
        fi
        echo "APT failed (attempt ${attempt}/3); retrying in $((attempt * 5)) seconds..." >&2
        sleep "$((attempt * 5))"
    done
}

# Assignment-specific libraries are explicit, never guessed from C++ includes.
EXTRA_PACKAGES=()
if [[ -f "${SOURCE_DIR}/apt-packages.txt" ]]; then
    while IFS= read -r package || [[ -n "$package" ]]; do
        package="${package%$'\r'}"
        [[ -z "$package" || "$package" == \#* ]] && continue
        if [[ ! "$package" =~ ^[a-z0-9][a-z0-9+.-]+(:[a-z0-9][a-z0-9-]*)?(=[A-Za-z0-9.+:~_-]+)?$ ]]; then
            echo "ERROR: invalid APT package in apt-packages.txt: ${package}" >&2
            exit 1
        fi
        EXTRA_PACKAGES+=("$package")
    done < "${SOURCE_DIR}/apt-packages.txt"
fi
# Preserve the supported instructor override without installing cppcheck for
# projects that only use the standard clang-tidy and sanitizer tests.
if [[ -f "${SOURCE_DIR}/tests/cppcheck/run_test" ]]; then
    EXTRA_PACKAGES+=(cppcheck)
fi

apt_get -o APT::Update::Error-Mode=any update
apt_get install -y --no-install-recommends \
    software-properties-common ca-certificates gnupg wget

# Register both repositories before refreshing their package indexes once.
add-apt-repository -y --no-update ppa:ubuntu-toolchain-r/test
# shellcheck disable=SC1091
source /etc/os-release
UBUNTU_CODENAME="${VERSION_CODENAME:?Ubuntu codename is required}"
mkdir -p /etc/apt/keyrings
wget --quiet --tries=3 --timeout=30 -O /etc/apt/keyrings/llvm.asc \
    https://apt.llvm.org/llvm-snapshot.gpg.key
cat > /etc/apt/sources.list.d/llvm.list <<EOF
deb [signed-by=/etc/apt/keyrings/llvm.asc] https://apt.llvm.org/${UBUNTU_CODENAME}/ llvm-toolchain-${UBUNTU_CODENAME}-${CLANG_VERSION} main
EOF

apt_get -o APT::Update::Error-Mode=any update
# GCC 14 supplies the C++23 standard library and ASan/LSan/UBSan runtime.
# make is sufficient for bootstrap.py; build-essential also pulls an unused
# default GCC/G++ toolchain. clang-tools and clang-format are not used by tests.
apt_get install -y --no-install-recommends \
    make gcc-14 g++-14 catch2 \
    python3 python3-pip python-is-python3 dos2unix \
    "clang-${CLANG_VERSION}" "clang-tidy-${CLANG_VERSION}" \
    "${EXTRA_PACKAGES[@]}"

if [[ ! -x /usr/bin/gcc-14 || ! -x /usr/bin/g++-14 ]]; then
    echo "ERROR: gcc-14/g++-14 are required but were not installed." >&2
    exit 1
fi

if [[ -x /usr/bin/gcov-14 ]]; then
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14 140 \
        --slave /usr/bin/g++ g++ /usr/bin/g++-14 \
        --slave /usr/bin/gcov gcov /usr/bin/gcov-14
else
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14 140 \
        --slave /usr/bin/g++ g++ /usr/bin/g++-14
fi

update-alternatives --install /usr/bin/clang clang "/usr/bin/clang-${CLANG_VERSION}" 200
update-alternatives --install /usr/bin/clang++ clang++ "/usr/bin/clang++-${CLANG_VERSION}" 200
update-alternatives --install /usr/bin/clang-tidy clang-tidy "/usr/bin/clang-tidy-${CLANG_VERSION}" 200

python3 -m pip install --no-cache-dir -r "${SOURCE_DIR}/requirements.txt"
# Catch missing runtime imports during image creation, before any submissions.
python3 -c 'import click, yaml, dateutil.parser, gradescope_utils.autograder_utils.json_test_runner'

cp "${SOURCE_DIR}/grade_util.py" /autograder
cp "${SOURCE_DIR}/bootstrap.py" /autograder
cp "${SOURCE_DIR}/config.yml" /autograder

apt-get clean
rm -rf /var/lib/apt/lists/*
