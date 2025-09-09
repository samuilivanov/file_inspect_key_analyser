#!/usr/bin/env python3
# filesource.py

import argparse
import requests
import sys

def send_file(url, filepath):
    try:
        with open(filepath, "rb") as f:
            data = f.read()
        headers = {"X-Filename": filepath.split("/")[-1]}  # optional filename header
        response = requests.post(url, data=data, headers=headers)
        print(response.text)
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)

def main():
    parser = argparse.ArgumentParser(description="Send a file to fika-ingestd")
    parser.add_argument("url", help="HTTP URL of the ingest service (e.g., http://0.0.0.0:8080/upload)")
    parser.add_argument("file", help="Path to the file to send")
    args = parser.parse_args()

    send_file(args.url, args.file)

if __name__ == "__main__":
    main()
