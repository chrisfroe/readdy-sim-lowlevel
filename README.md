This was built against an installed readdy version from conda, specifically with the following setup
```bash
conda create -n rdysim python=3.7                                                                                                                                                                                                                                                                                    
conda activate rdysim                                                                                                                                                                                                                                                                                                
conda config --env --add channels conda-forge                                                                                                                                                                                                                                                                        
conda install -c readdy/label/dev readdy
conda install cmake
```
which (at the time of writing this) results in
```bash
conda list

...
hdf5                      1.10.4          nompi_h3c11f04_1106    conda-forge
...
python                    3.7.6                h0371630_2  
readdy                    v2.0.2          py37_125_gda02c28    readdy/label/dev
...
```

Configure, build and run
```bash
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=$CONDA_PREFIX ..
make
./rdysim
```
