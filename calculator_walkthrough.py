#!/usr/bin/env python
# child python script

### Imports ###
import os
import sys
import argparse

# Check if SDK_SETUP_ENV is set or not for checking whether environment is set or not.
# Exit walkthrough if SDK_SETUP_ENV is not set.
if not os.getenv('SDK_SETUP_ENV'):
    sys.exit("\nSDK Environment not set up -> please run setup_sdk_env script from SDK's root directory.")

hexagon_sdk_root = os.getenv('HEXAGON_SDK_ROOT')
script_dir = hexagon_sdk_root + '/utils/scripts/'
example_path='{}/examples/calculator'.format(hexagon_sdk_root)

# Import common walkthrough.
sys.path.append(script_dir)
import common_walkthrough as CW

def add_args_to_parser(parser):
    parser.add_argument('-r', type=int, default=0, dest='run_local', help='Run locally. 1: Run locally on APPS, 0: Run on DSP')
    parser.add_argument('-n', type=int, default=1000, dest='num', help='Array_size: Natural number up to which sum is calculated from 0 to (n-1)')

def main():

    # Adding example arguments to common walkthrough parser
    add_args_to_parser(CW.parser)
    # Making object of class device
    device = CW.device()
    # building for HLOS
    hlos_dir = device.build_hlos(example_path)
    # building for dsp
    dsp_dir = device.build_hexagon(example_path)
    # Signing the device
    device.sign()

    options = CW.get_options()
    executable_name = 'calculator'
    calculator_exe ='{}/{}/ship/calculator'.format(example_path,hlos_dir)
    calculator_stub ='{}/{}/ship/libcalculator.so'.format(example_path,hlos_dir)
    calculator_skel ='{}/{}/ship/libcalculator_skel.so'.format(example_path,dsp_dir)
    executables = [calculator_exe]
    hlos_libs = [calculator_stub]
    dsp_libs = [calculator_skel]

    device.copy_binaries_to_target(executables,hlos_libs,dsp_libs)

    # Run the example as per command line options
    if options.run_local == 1:
        print("--- Run Calculator Example Locally")

    if options.domain == 0:
        print("--- Run Calculator Example on aDSP")
    elif options.domain == 2:
        print("--- Run Calculator Example on sDSP")
    elif options.domain == 3:
        print("--- Run Calculator Example on cDSP")
    else:
        print("--- Run calculator Example on mDSP")
    args = '-r {} -d {} -n {} -U {}'.format(options.run_local,options.domain, options.num, options.unsigned)
    args_list = [args]
    device.run(executable_name,args_list)

if __name__ == '__main__':
    main()
    sys.stdout.flush()
