import subprocess
import sys
import os

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python checker.py <path_to_binary> <path_to_library>", file=sys.stderr)
        sys.exit(1)
    binary_path = sys.argv[1]
    lib_path = sys.argv[2]
    env = os.environ.copy()
    env["LD_PRELOAD"] = lib_path
    res = subprocess.run(binary_path, stdout=subprocess.PIPE, stderr=subprocess.PIPE, env=env)
    if res.returncode >= 0:
        print("child process should fail with abort", file=sys.stderr)
        sys.exit(2)
    stdout = res.stdout.decode("utf-8")
    stderr = res.stderr.decode("utf-8")
    assert "ABORT" in stdout, "abort handler hasn't been called"
    
    # check that stderr look like expected
    assert "libabort_stacktrace.so" in stderr
    assert "libc.so" in stderr
    assert "__libc_start_main" in stderr
