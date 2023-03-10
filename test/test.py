import os
from diamond4py import Diamond, OutFormat, Sensitivity
os.chdir(os.path.dirname(os.path.abspath(__file__)))
diamond = Diamond(
    database="cafa4.dmnd",
    n_threads=4,
)
# diamond version, see: https://github.com/bbuchfink/diamond/releases
print(diamond.version)
if not os.path.exists("cafa4.dmnd"):
    diamond.makedb("test_proteins.fasta")
diamond.dbinfo()
diamond.blastp(
    query="test_proteins.fasta",
    out="test_blastp_output",
    outfmt=OutFormat.BLAST_TABULAR,
    sensitivity=Sensitivity.DEFAULT,
)
print("done")