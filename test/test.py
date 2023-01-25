import os
from diamond4py import Diamond

diamond = Diamond(
    database="cafa4.dmnd",
    n_threads=4,
)
print(diamond.version)
if not os.path.exists("cafa4.dmnd"):
    diamond.makedb("test_proteins.fasta")
diamond.dbinfo()
diamond.blastp(
    query="test_proteins.fasta",
    out="test_blastp_output"
)
print("done")