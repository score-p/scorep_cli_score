# scorep-cli-score

scorep-cli-score is a user interface for scorep-score based on ncurses. It lets you create filter files by sorting and filtering regions. [1]

## Installation

### Requirements

-   Python3
-   libscorep-score povided by Score-P 4.0 [2]

### Build

Open your terminal, navigate to the repository and install scorep-score-gui:

	cd scorep_cli_score
	pip install .

## Usage

### start:

	scorep_cli_score <profile> [-h] [-o file path] [-f file path] [-i | -e | -v]

### command line options:

|  positional arguments    |                                              |
|--------------------------|----------------------------------------------|
| profile           	   | path to Cube profile                         |

|  optional arguments      |                                              |
|--------------------------|----------------------------------------------|
| -h, --help               | show help                                    |
| -o, --output <file path> | path for filter file                         | 
| -f, --filter <file path> | load existing filter file                    | 
|                          |                                              | 
| -i, --inclusive-time     | use and sort by inclusive time [default]     |
| -e, --exclusive-time     | use and sort by exclusive time               | 
| -v, --visits             | sort regions by visits                       |


## Todo

- ...

## Changelog

- ...

## Links

[1] https://github.com/score-p/scorep_cli_score

[2] http://score-p.org/
