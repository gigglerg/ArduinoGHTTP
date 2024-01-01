#!/usr/bin/python
# @file
# Simple file checksum utility.  Use it to match output from Arduino maple mini running ghttp_ethercard_post_file_upload.ino

import argparse
import ctypes

if __name__ == '__main__':
    # Parse shell args
    parser = argparse.ArgumentParser(description="Sime file checksum utility")
    # Shared arguments
    parser.add_argument("-i", "--input", dest="ipf", help="Input file to checksum", action="store", required=True)

    # Parse arguments...
    try:
        args = parser.parse_args()
    except:
        print("ERROR: argument parsing??")
        exit(1)

    # Load file as binary octet array
    with open(args.ipf, "rb") as binaryfile :
        bdata = bytearray(binaryfile.read())

    checksum = 0
    
    # Loop over bytes calculating checksum
    for b in bdata:
        checksum = (checksum + ((0xff ^ b) & 0xff)) & 0xffff
    checksum ^= 0xffff

    print("Checksum " + str(checksum) + "\n");
