#!/bin/bash
# Author: Hubertus Franke  (frankeh@nyu.edu)

#example ./runit_one.sh in60 8 options

OUTDIR=$1
INFILE=$2
FRAMES=$3
OPTIONS=$4
shift 4
MMU=$*
echo "mmuprog=<$MMU> outdir=<$OUTDIR>"
[[ "${MMU}" == "" ]] && exit

ALGOS="N X Y"

SPID=0   # process pid we are monitoring
#ulimit -v 300000   # just limit the processes 

##########################  START TIMER ####################

TIMELIMIT=600
TPID=0
MPID=$$
EXIT_NOW=0
SPID_KILLED=0	# notification that the process got killed due to timeout

TimerOn()
{
	SPID_KILLED=0
	# echo "sleep ${TIMELIMIT} ; kill -SIGUSR1 $MPID"
	(sleep ${TIMELIMIT} ; kill -SIGUSR1 $MPID) &
	TPID=$!	
	# echo "timer ${TPID}"
}

TimerOff()
{
	if [[ ${TPID} != 0 ]]; then
		# echo "kill timer ${TPID}"
		kill -9 ${TPID} 2>&1 > /dev/null
		wait ${TPID}
		TPID=0
	fi
}

TimerFires()
{
	# echo "fires"
	ps -p ${SPID} | grep -v "^[ ]*PID" 2>&1 > /dev/null
	if [ $? == 0 ]; then
		start_redirect
		# echo "kill appl ${SPID}"
		kill -9 ${SPID} > /dev/null 2>&1 
		stop_redirect
		SPID_KILLED=1
	fi
	TPID=0
}

GetMeOutOfHere()
{
	echo "INTERRUPTED -> TERMINATING"
	EXIT_NOW=1
	TimerFires
}

start_redirect() 
{ 
	exec 3>&2 ; exec 2> /dev/null 
}
stop_redirect()  
{ 
	exec 2>&3 ; exec 3>&- 
}
check_exit()    
{ 
	[[ ${EXIT_NOW} == 1 ]] && exit 
}

trap TimerFires     SIGUSR1
trap GetMeOutOfHere SIGINT

##########################  END TIMER ####################


############################################################################
#  NO TRACING 
############################################################################

for X in ${ALGOS}; do 
	OUTF="${OUTDIR}/out_${INFILE}_${FRAMES}_${X}_${OPTIONS}"
	echo "${MMU} -a${X} -o${OPTIONS} -f${FRAMES} ${PARGS} ${INFILE} rfile > ${OUTF}"
	${MMU} -a${X} -o${OPTIONS} -f${FRAMES} ${PARGS} ${INFILE} rfile > ${OUTF} &

	SPID=$!

	start_redirect
	TimerOn

	wait ${SPID} 2>&1 > /dev/null
	SPID=0

	TimerOff
	stop_redirect

	[[ ${SPID_KILLED} == 1 ]] && echo "      Killed after ${TIMELIMIT}"

	OUTPUT=`egrep "^SUM" ${OUTF}`
	echo "${X}: ${OUTPUT}"

	check_exit
done

