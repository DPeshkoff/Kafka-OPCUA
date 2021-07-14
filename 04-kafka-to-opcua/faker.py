# Copyright 2021 DPeshkoff && let-robots-reign;
# Distributed under the GNU General Public License, Version 3.0. (See
# accompanying file LICENSE)
######################################################################
## TECHNICAL INFORMATION                                            ##
## Faker                                                            ##
## Based on Python 3.9.1 64-bit                                     ##
######################################################################
# IMPORTS
from random import randint
from json import dumps
######################################################################

FIELDS = ['temperature', 'brightness']
ITERATIONS = 10


def get_faked_measurements(index):
    return {
        'iteration': index,
        **{field: randint(0, 100) for field in FIELDS}
    }

######################################################################


def main():
    measurements = [dumps(get_faked_measurements(_), indent=4) for _ in range(ITERATIONS)]
    print(*measurements, sep='\n')

######################################################################

    
if __name__ == '__main__':
    main()
