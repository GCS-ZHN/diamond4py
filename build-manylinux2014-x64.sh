env_name="diamond4py"
py_version=$1
conda create -n ${env_name} python=${py_version} scikit-build -y
if [ $? -ne 0 ]; then
    echo "Failed to create conda environment"
    exit 1
fi

/opt/conda/envs/${env_name}/bin/pip install build \
            cmake \
            ninja \
            -i https://pypi.tuna.tsinghua.edu.cn/simple
if [ $? -ne 0 ]; then
    echo "Failed to install build"
    exit 1
fi

/opt/conda/envs/${env_name}/bin/python -m build -n
if [ $? -ne 0 ]; then
    echo "Failed to build diamond4py"
    exit 1
fi
ARCH=`arch`
# replace python version without dot
/opt/conda/envs/${env_name}/bin/pip install dist/${env_name}-*-cp${py_version//./}-*-linux_${ARCH}.whl \
            -i https://pypi.tuna.tsinghua.edu.cn/simple
if [ $? -ne 0 ]; then
    echo "Failed to install diamond4py"
    exit 1
fi

/opt/conda/envs/${env_name}/bin/python test/test.py
if [ $? -ne 0 ]; then
    echo "Failed to test diamond4py"
    exit 1
fi
