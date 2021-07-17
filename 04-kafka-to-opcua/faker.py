# Copyright 2021 DPeshkoff && let-robots-reign;
# Distributed under the GNU General Public License, Version 3.0. (See
# accompanying file LICENSE)
######################################################################
## TECHNICAL INFORMATION                                            ##
## Faker                                                            ##
## Based on Python 3.9.1 64-bit                                     ##
######################################################################
# IMPORTS
from json import dumps
from random import randint
from time import sleep
######################################################################

FIELDS = ['temperature', 'brightness']
SECONDS_TIMEOUT = 10


def get_faked_measurements(index):
    return {
        'iteration': index,
        **{field: randint(0, 100) for field in FIELDS}
    }

######################################################################


def main():
    iteration = 0
    while True:
        measurement = dumps(get_faked_measurements(iteration), indent=4)
        print(measurement)
        iteration += 1
        sleep(SECONDS_TIMEOUT)

######################################################################

    
if __name__ == '__main__':
    main()
