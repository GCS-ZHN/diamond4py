FROM dockcross/manylinux2014-x64

ENV PATH /opt/conda/bin:$PATH

RUN wget https://mirrors.bfsu.edu.cn/anaconda/miniconda/Miniconda3-latest-Linux-x86_64.sh -O ~/miniconda.sh && \
    /bin/bash ~/miniconda.sh -b -p /opt/conda && \
    rm ~/miniconda.sh && \
    /opt/conda/bin/conda init

ENV PATH /opt/conda/bin:$PATH

CMD ["/bin/bash"]

# echo "conda activate diamond4py" >> ~/.bashrc
# /opt/conda/envs/diamond4py/bin/pip
# RUN conda create -n diamond4py python=3.8 -y
# RUN pip install https://pypi.tuna.tsinghua.edu.cn/simple
