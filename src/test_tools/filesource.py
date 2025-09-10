'''
 * This file is part of Fika.
 *
 * Fika is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * Fika is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Fika.  If not, see <http://www.gnu.org/licenses/>.
'''

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
