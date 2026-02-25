#!/usr/bin/env python3

import sys
import time

def main():
    if len(sys.argv) != 2:
        print("Uso: python sleep_script.py <secondi>")
        sys.exit(1)

    try:
        seconds = float(sys.argv[1])
        if seconds < 0:
            raise ValueError("Il numero di secondi deve essere non negativo.")
    except ValueError as e:
        print(f"Errore: {e}")
        sys.exit(1)

    time.sleep(seconds)
    print("seconds passed" + str(seconds))

if __name__ == "__main__":
    main()