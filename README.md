# L1TSignalZerobiasMixer

This package contains instructions for
1) producing Madgraph5 based signals,
2) hadronize them using Pythia8
3) simulate for CMS detector response using Delphes
4) read zerobias L1TCaloRegion data, in a format suitable for L1TAutoEncoder studies, from separtately produced in CMSSW runs
5) overlay a random zerobias event with the produced signal and save the data back in a format suitable for L1TAutoEncoder studies.

The inputs are madgraph, pythia8 and Delphes control files for the signal root file producer are named *.txt.

For the region dumper, the inputs are the root file from Delphes and the Zerobias file from the L1TRegionDumper.

Code installation instructions:

First time only

If you are on machines with /cvmfs and CentOS8 (mucol01.hep.wisc.edu), you may use ROOT from there:

```
source /cvmfs/sft.cern.ch/lcg/views/LCG_101/x86_64-centos8-gcc11-opt/setup.sh
```

If you are on a Apple MacOS system, you need gcc, gfortran and root installedm and you may need this additionally for Madgraph and Delphes work:

```
if [ `uname` == 'Darwin' ]; then echo export MACOSX_DEPLOYMENT_TARGET=10.15; fi
```

If you do not have a working Madgraph5 installation, do the following, in a directory with plenty of space:

```
wget https://launchpad.net/mg5amcnlo/3.0/3.2.x/+download/MG5_aMC_v3.2.0.tar.gz
tar zxf MG5_aMC_v3.2.0.tar.gz 
export mg5dir=$PWD/MG5_aMC_v3_2_0/
cd $mg5dir
python $mg5dir/bin/mg5_aMC
```

From within the MG5_aMC prompt execute the following. They take a long time 10 mins to finish.
Optionally, you can track the log files in the secondary login window, if you wish.

```
install pythia8
install Delphes
exit
```

If you do have a Madgraph5 directory already setup:

```
export mg5dir=<your MG5_aMC_v3_2_0 directory>
```

Go to the dirctory where you wish to work and then install this code:

```
if [ -d '/nfs_scratch' ]; then export basedir=/nfs_scratch/$USER/`date +%Y-%m-%d`; else basedir=$PWD/`date +%Y-%m-%d`; fi
mkdir -p $basedir
git clone git@github.com:SridharaDasu/L1TSignalZerobiasMixer.git
source $basedir/L1TSignalZerobiasMixer/setup.sh
```

On relogin use the base directory, i.e., the directory with the date of creation above, e.g., /nfs_scratch/dasu/2021-11-03/

```
source $basedir/L1TSignalZerobiasMixer/setup.sh
```

To produce data use  .txt files with different configurations

```
cd $datadir
python $mg5dir/bin/mg5_aMC $workdir/cms-vbfh-pythia8-delphes.txt
```

With above .txt file the event files will be in the directory $mg5dir/cms-vbfh-pythia8-delphes/run_01/

There should now be a root file in your directory $datadir/Events/run_01/tag_1_delphes_events.root

We use root to read this data, and the CSV file from the zerobias run to produce the final file:

```
root -l L1TSignalZerobiasMixer.C\(\"$datadir/cms-vbfh-pythia8-delphes/Events/run_01/tag_1_delphes_events.root\"\)
```
