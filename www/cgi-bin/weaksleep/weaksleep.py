#!/usr/bin/env python3

import sys
import time
import signal

def handle_sigint(signum, frame):
    print("KO|0000000000|Ctrl^c")
    sys.exit(0)

def main():
    signal.signal(signal.SIGINT, handle_sigint)

    if len(sys.argv) != 2:
        print("KO|0000000000|Uso: python sleep_script.py <seconds>")
        sys.exit(1)

    try:
        seconds = float(sys.argv[1])
        if seconds < 0:
            raise ValueError("bad value")
    except ValueError as e:
        print(f"KO|0000000000| {e}")
        sys.exit(1)

    try:
        time.sleep(seconds)
        print("seconds passed " + str(seconds))
    except KeyboardInterrupt:
        print("KO|0000000000|Interrupt")
        sys.exit(0)

if __name__ == "__main__":
    main()