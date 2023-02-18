"""
A python wrapper module of diamond
"""

from enum import Enum
from typing import Any, Union
from .libdiamond import main, version
import os
import functools
import inspect


def _require_db(func):
    """
    An internal decorator for diamond to check database.
    """
    @functools.wraps(func)
    def wrapper_func(*args, **kwargs):
        if len(args) == 0 or not isinstance(args[0], Diamond):
            raise ValueError(
                "This decorator is only used for methods of `Diamond`")
        self: Diamond = args[0]
        if not self._check_db():
            raise RuntimeError("Database not existed, please check it first")
        return func(*args, **kwargs)
    return wrapper_func


def not_null(func):
    """
    A decorator for checking params.
    When parameter type is annotated, and default
    value is not `None` if given, this parameter is 
    required as annotation type.

    When type is not annotated or annotated as `Any`, or 
    default value is set to `None, we will allow it to
    be `None`.
    """
    @functools.wraps(func)
    def wrapper_func(*args, **kwargs):
        params = inspect.signature(func).parameters
        name_list = func.__code__.co_varnames

        def check_annotation(param_name, param_val):
            annotation_type: type = params[param_name].annotation
            default_value = params[param_name].default
            if annotation_type == inspect._empty or annotation_type == Any:
                return
            if default_value is None and param_val is None:
                return
            if str(annotation_type).startswith("typing."):
                if param_val is None:
                    raise ValueError(
                        f"Parameter '{param_name}' requires not be `NoneType`")

            elif not isinstance(param_val, annotation_type):
                raise ValueError(
                    f"Parameter '{param_name}' requires be an instance of {annotation_type}")

        # check position params
        for i, param_val in enumerate(args):
            param_name = name_list[i]
            check_annotation(param_name, param_val)

        # check keyword params
        for param_name, param_val in kwargs.items():
            check_annotation(param_name, param_val)
        return func(*args, **kwargs)
    return wrapper_func


class OutFormat(Enum):
    """
    Output format of blast alignment.
    """
    BLAST_PAIRWISE = 0
    BLAST_XML = 5
    BLAST_TABULAR = 6
    DAA = 100
    SAM = 101


class Sensitivity(Enum):
    """
    Sensitivity of blast alignment.
    """
    FAST = 0
    MID_SENSITIVE = 1
    DEFAULT = 2
    SENSITIVE = 3
    MORE_SENSITIVE = 4
    VERY_SENSITIVE = 5
    ULTRA_SENSITIVE = 6

    def get_cmd_option(self):
        if self != Sensitivity.DEFAULT:
            return "--" + self.name.lower().replace("_", "-")


class Diamond(object):
    """
    Diamond python wrapper object

    Parameters:
    ------------
    database: str
        the database file, the input of other methods
        and the output of `makedb` method.
    n_threads: int
        number of threads to use
    quiet: bool
        do not print progress information
    log: bool
        log progress information to file
    header: bool
        print header line in output
    """

    @not_null
    def __init__(self, database: str, n_threads: int = 1,
                 quiet: bool = False, log: bool = False, header: bool = False) -> None:

        self._value_options = {
            "db": database,
            "threads": n_threads
        }
        self._flag_options = {
            "quiet": quiet,
            "log": log,
            "header": header
        }

    @_require_db
    @not_null
    def blastp(self, query: str, out: str,
               outfmt: Union[int,OutFormat] = OutFormat.BLAST_TABULAR,
               sensitivity: Union[Sensitivity, int] = 2) -> int:
        """
        Sequence alignment for proteins by diamond.

        Parameters:
        ------------
        db: str
            the database file
        query: str
            the input fasta query file
        outfmt: Union[int,OutFormat]
            output format, default is 6, which is tabular format.
        sensitivity: Union[Sensitivity, int]
            sensitivity of blast alignment, default is 2, which is
            the default sensitivity of diamond.

        """
        if isinstance(outfmt, int):
            outfmt = OutFormat(outfmt)
        if isinstance(sensitivity, int):
            sensitivity = Sensitivity(sensitivity)
        args = self._add_general_options(
            "blastp",
            sensitivity.get_cmd_option(),
            query=query, 
            out=out,
            outfmt=outfmt.value)
        return main(*args)

    @_require_db
    @not_null
    def dbinfo(self):
        """
        Print the information of current database.
        """
        args = self._add_general_options("dbinfo")
        return main(*args)

    @not_null
    def makedb(self, input_seqs_file: str,
               taxonmap: str = None,
               taxonnodes: str = None,
               taxonnames: str = None):
        """
        Make database based on given FASTA files.

        Parameters:
        -------------
        input_seqs_file: str
            Path to the input protein reference database
            file in FASTA format (may be gzip compressed).
        
        taxonmap: str
            Path to mapping file that maps NCBI protein
            accession numbers to taxon ids (gzip compressed).
        
        taxonnodes: str
            Path to the nodes.dmp file from the NCBI taxonomy.
        
        taxonnames: str
            Path to the names.dmp file from the NCBI taxonomy. 
        """
        args = self._add_general_options(
            "makedb", "--in", input_seqs_file,
            taxonmap=taxonmap,
            taxonnodes=taxonnodes,
            taxonnames=taxonnames)
        return main(*args)

    def _add_general_options(self, *args, **kwargs) -> list:
        new_args = list(filter(lambda s: isinstance(s, str) and len(s) > 0, args))
        kwargs.update(self._value_options)
        for k, v in kwargs.items():
            if v:
                new_args.append(f"--{k}")
                new_args.append(str(v))

        for k, v in self._flag_options.items():
            if v:
                new_args.append(f"--{k}")

        return new_args

    def _check_db(self) -> bool:
        return os.path.exists(self._value_options["db"])

    @property
    def version(self) -> str:
        """Version of diamond"""
        return version()

from . import _version
__version__ = _version.get_versions()['version']
