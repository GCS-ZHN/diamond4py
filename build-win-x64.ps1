$py_versions=@("37", "38", "39", "310")

foreach ($version in $py_versions) {
    conda activate py$version
    pip install build ninja cmake scikit-build -i https://pypi.tuna.tsinghua.edu.cn/simple
    python -m build --sdist --wheel --outdir wind-dist -n
    conda deactivate
}