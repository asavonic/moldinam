#!/bin/env python

import time
import subprocess
import argparse
import csv


def time_call(args):
    start = time.process_time()
    subprocess.call(args)
    finish = time.process_time()

    return finish - start


def bench(platforms, iterations, result, command):
    with open(result, 'w') as csvfile:
        fieldnames = ['iterations'] + platforms
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

        writer.writeheader()
        for iter_num in iterations:
            result = {}
            result['iterations'] = iter_num
            for platform in platforms:
                comm = substitute_command(command, iter_num, platform)
                result[platform] = time_call(comm)

            writer.writerow(result)


def substitute_command(command, iter_num, platform):
    result = command[:]
    for i, value in enumerate(result):
        if result[i] == 'ITER':
            result[i] = str(iter_num)

        if result[i] == 'PLAT':
            result[i] = platform

    return result


def main():
    parser = argparse.ArgumentParser(description='Benchmark script with csv output.')

    parser.add_argument('--result', dest='result', type=str,
                        required=True, help='result file')

    parser.add_argument('--platforms', nargs='+', dest='platforms',
                        required=True, help='list of platforms to test')

    parser.add_argument('--iterations', type=int, nargs='+', dest='iterations',
                        required=True, help='list of iterations')

    parser.add_argument('command', metavar='COMMAND', nargs='+',
                        help='command to execute where ITER is a number of iterations, PLAT is a platform')

    args = parser.parse_args()

    bench(args.platforms, args.iterations, args.result, args.command)

if __name__ == "__main__":
    main()
