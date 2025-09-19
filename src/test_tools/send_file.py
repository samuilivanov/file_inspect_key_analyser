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
import os

from generators import (
    generate_png, generate_jpg, generate_gif, generate_bmp,
    generate_pdf, generate_docx, generate_xlsx, generate_zip,
    generate_wav, generate_mp4, generate_pptx
)

# map type strings to generator functions
FILE_GENERATORS = {
    "png": generate_png,
    "jpg": generate_jpg,
    "gif": generate_gif,
    "bmp": generate_bmp,
    "pdf": generate_pdf,
    "docx": generate_docx,
    "xlsx": generate_xlsx,
    "zip": generate_zip,
    "wav": generate_wav,
    "mp4": generate_mp4,
    "pptx": generate_pptx,
}

DEFAULT_URL = "http://0.0.0.0:8080/upload"

def send_file(url, filepath):
    try:
        with open(filepath, "rb") as f:
            data = f.read()
        headers = {"X-Filename": os.path.basename(filepath)}
        response = requests.post(url, data=data, headers=headers)
        print(response.text)
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)

def main():
    parser = argparse.ArgumentParser(description="Send a file to fika-ingestd")
    parser.add_argument("--url", default=DEFAULT_URL,
                        help=f"HTTP URL of the ingest service (default: {DEFAULT_URL})")
    parser.add_argument("--file", help="Path to an existing file to send")
    parser.add_argument("--type", choices=FILE_GENERATORS.keys(),
                        help="Type of synthetic file to generate and send")
    args = parser.parse_args()

    if args.file:
        filepath = args.file
    elif args.type:
        generator = FILE_GENERATORS[args.type]
        synthetic_dir = os.path.join(os.path.dirname(__file__), "synthetic_files")
        os.makedirs(synthetic_dir, exist_ok=True)
        filepath = os.path.join(synthetic_dir, f"test.{args.type}")
        generator(filepath)
    else:
        print("Error: Must provide either --file or --type", file=sys.stderr)
        sys.exit(1)

    send_file(args.url, filepath)

if __name__ == "__main__":
    main()
