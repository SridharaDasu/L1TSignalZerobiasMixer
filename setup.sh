#/bin/bash
if [ -d '/cvmfs' ]; then
    if [[ `uname -r` == *"el7"* ]]; then
        source /cvmfs/sft.cern.ch/lcg/views/LCG_101/x86_64-centos7-gcc11-opt/setup.sh
        if [ -d "/nfs_scratch/$USER/CentOS7/MG5_aMC_v3_2_0" ]; then
            export mg5dir=/nfs_scratch/$USER/CentOS7/MG5_aMC_v3_2_0/
	fi
    elif [[ `uname -r` == *"el8"* ]]; then
        source /cvmfs/sft.cern.ch/lcg/views/LCG_101/x86_64-centos8-gcc11-opt/setup.sh
        if [ -d "/nfs_scratch/$USER/CentOS8/MG5_aMC_v3_2_0" ]; then
            export mg5dir=/nfs_scratch/$USER/CentOS8/MG5_aMC_v3_2_0/
	fi
    fi
fi
if [ -d "$ROOTSYS" ]; then
    echo "Using ROOT from $ROOTSYS";
else
    echo "ROOT is needed for this to work";
fi

if [ -d "$mg5dir" ]; then 
    echo "Using Madgraph5 from $mg5dir";
else
    echo "Madgraph is needed for this to work - export mg5dir=... and rerun";
fi
export PYTHIA8DATA=`$mg5dir/HEPTools/bin/pythia8-config --xmldoc`
export LHAPDF_DATA_PATH=`$mg5dir/HEPTools/bin/lhapdf-config --data`
source $mg5dir/Delphes/DelphesEnv.sh
export workdir=`dirname -- "${BASH_SOURCE[0]}"`
export basedir=`dirname $workdir`;
export datadir=$workdir/data;
mkdir -p $datadir
echo "Using basedir=$basedir";
echo "Using workdir=$workdir";
echo "Using datadir=$datadir";
cd $workdir
echo "Current directory set to $datadir";
