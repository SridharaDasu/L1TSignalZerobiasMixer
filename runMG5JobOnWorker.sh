#!/bin/bash
source /cvmfs/sft.cern.ch/lcg/views/LCG_101/x86_64-centos7-gcc11-opt/setup.sh
tar zxf MyMG5Dir.tar.gz
export mg5dir=$PWD/MG5_aMC_v3_2_0
export workdir=$PWD
export datadir=$workdir/data;
export PYTHIA8DATA=`$mg5dir/HEPTools/bin/pythia8-config --xmldoc`
export LHAPDF_DATA_PATH=`$mg5dir/HEPTools/bin/lhapdf-config --datadir`
source $mg5dir/Delphes/DelphesEnv.sh
echo "Using ROOTSYS=$ROOTSYS"
echo "Using mg5dir=$mg5dir"
echo "Using workdir=$workdir"
echo "Using datadir=$datadir"
echo "Current directory set to $datadir"
mkdir -p $datadir
cd $datadir
export random=`date +"%S%M%H%S"`
cat $workdir/$1 | sed 's/set iseed 0/set iseed '"$random"'/g' > mg5_configuration_$random.txt
python $mg5dir/bin/mg5_aMC mg5_configuration_$random.txt
