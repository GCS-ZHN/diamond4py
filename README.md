# diamond4py
[![pypi](https://github.com/GCS-ZHN/diamond4py/actions/workflows/pypi.yml/badge.svg)](https://pypi.org/project/diamond4py/)
![license](https://img.shields.io/badge/license-GPLv3-blue.svg)
![python](https://img.shields.io/badge/python->=3.7-success.svg)

This is python binding for [diamond](https://github.com/bbuchfink/diamond)--a sequence alignment BLAST software implements in C++.
Current base version is [diamond v2.1.0](https://github.com/bbuchfink/diamond/releases/tag/v2.1.0).

## install

- from source code
```bash
git clone https://github.com/GCS-ZHN/diamond4py.git
cd diamond4py
conda create -n diamond4py python=3.8
# zlib is required by diamond.
conda install -c anaconda zlib
pip install -v .
```

- from pypi
```bahs
pip install diamond4py
```

## Usage
```python
from diamond4py import Diamond

# create a object
diamond = Diamond(
    database="database.dmnd",
    n_threads=4
)

# make db if you don't create it or just download one from websites
diamond.makedb("database.fasta")
print(diamond.version)

# print database statistic info
diamond.dbinfo()

# run blast for proteins
diamond.blastp(
    query="test_proteins.fasta",
    out="test_output"
)
```

### Advanced usage.

Some parameter options are not implemented in current release,
You could directly use it like this.

```python
diamond.blastp(
    query="test_proteins.fasta",
    out="test_blastp_output",
    outfmt=OutFormat.BLAST_TABULAR,
    sensitivity=Sensitivity.DEFAULT,
    top=5 # `--top` option is not directly defined
    block_size=2.0 # `--block-size` is not directly defined
)
```

and you can global change the tabular output with customed title by
```python
OutFormat.BLAST_TABULAR.with_extra_option(
        "qseqid", "qstart",
        "qend", "qlen", "sseqid"
    ) # it will change it globally

# call this will reset to default
OutFormat.BLAST_TABULAR.reset()
```

In fact, you can call the original C++ main method like this:
````python
from .libdiamond import main
args = ['bastp', '--query', 'data.fasta', '--db', 'db.dmnd']
main(*args)
````
But it is not recommended because the original C++ api lacks necessary
interface encapsulation and parameter checking. At the same time, 
it is not pythonic enough, and there is no parameter reminder during programming
unless you read the original parameter document.

## How to contrib
The core of python wrap is to use custom python methods and `Diamond._build_options` to 
call the `libdiamond.main` method More convenient packaging.
Therefore, anyone can make customized extensions through the 
above ideas. I maintains this project after work, 
so I cannot make it in time Keep up to date with all 
diamond developments. Everyone is welcome to submit PRs.