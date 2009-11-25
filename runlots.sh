#!/usr/bin/perl
# runlots.sh

### Set the job name
#PBS -N hattrick

### Request email when job begins and ends
#PBS -m bea

### Specify email address to use for notification.
#PBS -M wto@email.arizona.edu

### Specify the PI group found with va command
#PBS -W group_list=lclose

### Set the queue to submit this job.
#PBS -q default

### Set the number of cpus that will be used.
#PBS -l nodes=1:ppn=16

### Specify up to a maximum of 1600 hours total cpu time for 1-processor job
#PBS -l cput=192:0:0

### Specify up to a maximum of 240 hours walltime for the job
#PBS -l walltime=24:0:0


system "date";

$CPUNUM = 2;

$workdir_base = "/scr1/dittmann";
#chdir $workdir_base;


$RUN = 0;
for ($M = 1e-6; $M <= 1e-2; $M+=1e-6) #99999
{
	for ($R = .005; $R <= 0.05; $R+=0.001) #45
	{
		$RUN++;
		$pid[$RUN-1] = -1;

		if (!defined($pid[$RUN-1]=fork())) {
			print "ERROR: cannot fork: 1\n";
		}
		if ($pid[$RUN-1]==0) {
			system "./hattrick hat.txt 1 $M $R >> Run3.txt";
			exit;
		}

		if ($RUN >= $CPUNUM) {
			waitpid($pid[$RUN-$CPUNUM],0);
		}
	}
}

for ($i = $totalsims-$CPUNUM+1; $i <= $totalsims; $i++) {
	waitpid($pid[$i],0);
}

system "date";
