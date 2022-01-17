# L1TSignalZerobiasMixer

This package contains instructions for
1) producing Madgraph5 based signals,
2) hadronize them using Pythia8
3) simulate for CMS detector response using Delphes
4) read zerobias L1TCaloRegion data, in a format suitable for L1TAutoEncoder studies, from separtately produced in CMSSW runs
5) overlay a random zerobias event with the produced signal and save the data back in a format suitable for L1TAutoEncoder studies.

The inputs are madgraph, pythia8 and Delphes control files for the signal root file producer. For the region dumper, the inputs are the root file from Delphes and the Zerobias file from the L1TRegionDumper.

First time only

```
if [ -d '/nfs_scratch' ]; then export basedir=/nfs_scratch/$USER/`date +%Y-%m-%d`; else basedir=$PWD/`date +%Y-%m-%d`; fi
mkdir -p $basedir
cd $basedir
git clone https://github.com/SridharaDasu/L1TSignalZerobiasMixer.git
export workdir=$basedir/L1TSignalZerobiasMixer
cd $workdir
mkdir -p $workdir/data
export datadir=$workdir/data
wget https://launchpad.net/mg5amcnlo/3.0/3.2.x/+download/MG5_aMC_v3.2.0.tar.gz
tar zxf MG5_aMC_v3.2.0.tar.gz 
export mg5dir=$workdir/MG5_aMC_v3_2_0/
cd $mg5dir
if [ -d '/cvmfs' ]; then source /cvmfs/sft.cern.ch/lcg/views/LCG_97a/x86_64-centos7-gcc8-opt/setup.sh; fi
if [ `uname` == 'Darwin' ]; then echo export MACOSX_DEPLOYMENT_TARGET=10.15; fi
python $mg5dir/bin/mg5_aMC
```

From within the MG5_aMC prompt execute the following. They take a long time 10 mins to finish.
Optionally, you can track the log files in the secondary login window, if you wish.

```
      install pythia8
      install Delphes
      exit
```

On relogin cd to the base directory, i.e., the directory with the date of creation above, e.g., /Users/dasu/2021-11-03/

```
export basedir=$PWD
export workdir=$basedir/L1TSignalZerobiasMixer
export datadir=$workdir/data
export mg5dir=$workdir/MG5_aMC_v3_2_0/
```

To produce data use  .txt files with different configurations

```
cd $datadir
if [ -d '/cvmfs' ]; then source /cvmfs/sft.cern.ch/lcg/views/LCG_97a/x86_64-centos7-gcc8-opt/setup.sh; fi
export PYTHIA8DATA=$mg5dir/HEPTools/pythia8/share/Pythia8/xmldoc/
python $mg5dir/bin/mg5_aMC $workdir/cms-hh-pythia8-delphes.txt
```

With above .txt file the event files will be in the directory $mg5dir/cms-hh-pythia8-delphes/run_01/

There should now be a root file in your directory $datadir/Events/run_01/tag_1_delphes_events.root

We use root to read this data, and the CSV file from the zerobias run to produce the final file:

```
c++ -o runL1TSignalZerobiasMixer runL1TSignalZerobiasMixer.cpp
runL1TSignalZerobiasMixer tag_1_delphes_events.root /nfs_scratch/dasu/2022-01/CMSSW_11_1_9/src/L1Trigger/L1TRegionDumper/test/L1TRegionDump.csv 
```


