#!/bin/bash

OUTDIR=${1:-.}
shift 1
MMU=$*
echo "OUTDIR=${OUTDIR}  MMU=${MMU}"
[[ "${MMU}" == "" ]] && exit

#INFILE=$2
#FRAMES=$3
#OPTIONS=$4

./runit_one.sh    ${OUTDIR} in18    4      PFS  ${MMU}
./runit_one.sh    ${OUTDIR} in18    4     OPFS  ${MMU}
./runit_one.sh    ${OUTDIR} in18    4   pfOPFS  ${MMU}
./runit_oneNXY.sh ${OUTDIR} in18    4  apfOPFS  ${MMU}

./runit_one.sh    ${OUTDIR} in60    8      PFS  ${MMU}
./runit_one.sh    ${OUTDIR} in60    8     OPFS  ${MMU}
./runit_one.sh    ${OUTDIR} in60    8   pfOPFS  ${MMU}
./runit_oneNXY.sh ${OUTDIR} in60    8  apfOPFS  ${MMU}

./runit_one.sh    ${OUTDIR} in1K4   16     PFS  ${MMU}
./runit_one.sh    ${OUTDIR} in1K4   16    OPFS  ${MMU}
./runit_one.sh    ${OUTDIR} in1K4   16  pfOPFS  ${MMU}
./runit_oneNXY.sh ${OUTDIR} in1K4   16 apfOPFS  ${MMU}

./runit_one.sh    ${OUTDIR} in10K3  32    OPFS  ${MMU}
./runit_one.sh    ${OUTDIR} in1M2   32     PFS  ${MMU}


