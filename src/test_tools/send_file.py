#!/usr/bin/env python3

"""
* This file is part of file_ingest_key_analyser (FIKA).
*
* file_ingest_key_analyser (FIKA) is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; version 2 of the License.
*
* file_ingest_key_analyser (FIKA) is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with file_ingest_key_analyser (FIKA).  If not, see <http://www.gnu.org/licenses/>.
"""

import argparse
import requests
import sys
import os

from generators import (
    generate_png,
    generate_jpg,
    generate_gif,
    generate_bmp,
    generate_pdf,
    generate_docx,
    generate_xlsx,
    generate_zip,
    generate_wav,
    generate_mp4,
    generate_pptx,
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
    parser = argparse.ArgumentParser(description="Send files to fika-ingestd")
    parser.add_argument(
        "--url",
        default=DEFAULT_URL,
        help=f"HTTP URL of the ingest service (default: {DEFAULT_URL})",
    )
    parser.add_argument(
        "--file",
        action="append",
        help="Path to an existing file to send (can be repeated)",
    )
    parser.add_argument(
        "--type",
        choices=FILE_GENERATORS.keys(),
        nargs="+",
        help="Type(s) of synthetic file(s) to generate and send (space-separated)",
    )
    parser.add_argument(
        "--count",
        type=int,
        default=1,
        help="Number of files to generate per type (default: 1)",
    )
    args = parser.parse_args()

    synthetic_dir = os.path.join(os.path.dirname(__file__), "synthetic_files")
    os.makedirs(synthetic_dir, exist_ok=True)

    # Collect all files to send
    files_to_send = []

    # Add existing files
    if args.file:
        files_to_send.extend(args.file)

    # Generate synthetic files
    if args.type:
        for ftype in args.type:
            generator = FILE_GENERATORS[ftype]
            for i in range(args.count):
                filepath = os.path.join(synthetic_dir, f"test_{ftype}_{i+1}.{ftype}")
                generator(filepath)
                files_to_send.append(filepath)

    if not files_to_send:
        print("Error: Must provide at least --file or --type", file=sys.stderr)
        sys.exit(1)

    # Send all files
    for fpath in files_to_send:
        send_file(args.url, fpath)


if __name__ == "__main__":
    main()
