# diamond4py
[![pypi](https://github.com/GCS-ZHN/diamond4py/actions/workflows/pypi.yml/badge.svg)](https://pypi.org/project/diamond4py/)
![license](https://img.shields.io/badge/license-GPLv3-blue.svg)
![python](https://img.shields.io/badge/python->=3.7-success.svg)

This is python binding for [diamond](https://github.com/bbuchfink/diamond)--a sequence alignment BLAST software implements in C++.

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
