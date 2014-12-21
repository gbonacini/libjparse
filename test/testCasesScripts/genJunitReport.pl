#!/usr/bin/env perl

use warnings;
use strict;

use Getopt::Std;

my %options;
my %tests; 
my $ntest=0;
my $nerr=0;
my $elapsedtime=0.0;

my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst)=localtime(time);
$year+=1900;
$mon++;
my $timestamp = $year . "-" . $mon . "-" . $mday . "T" . $hour . ":" . $min . ":" . $sec;

my $ERR_MSG = " [-n <test class nams> -d <test_results> -o <output directory> | -t <elapsed time> ] | [ -h ]\n";
getopts("n:d:o:t:h",\%options);
die ("$0 $ERR_MSG") if(defined $options{h} or not defined $options{n} or not defined $options{d} or not defined $options{o});
die ("Wrong Path.") unless(-e $options{o});
$elapsedtime = $options{t} if(defined $options{t});

my $filename = $options{o} . "/" . $options{n} . ".xml";
open(REPO, "> $filename");
$nerr++ while ($options{d} =~ m/@/g);

my @temp = split('[|]', $options{d});
for (my $i=0;;$i=$i+2) {
	last if(not defined $temp[$i]);
	$tests{$temp[$i]} = $temp[$i+1];
	$ntest++;
}
my $testUnit=$options{n};
my $compilerString=qx/\$(which cc) -v 2>&1 | grep version/;
chomp($compilerString);

my $template = "";
$template .= sprintf("%s", "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
$template .= sprintf(" <testsuites disabled=\"0\" errors=\"%d\" failures=\"%d\" name=\"%s\" tests=\"%d\" time=\"%f\">\n", 
			$nerr, $nerr, $testUnit, $ntest, $elapsedtime);
$template .= sprintf("  <testsuite name=\"%s\" errors=\"%d\" tests=\"%d\" failures=\"%d\" time=\"%f\" timestamp=\"%s\" />\n",
			 $testUnit, $nerr, $ntest, $nerr, $elapsedtime,  $timestamp);
$template .= sprintf("   <properties>\n");
$template .= sprintf("     <property name=\"cc.vendor\" value=\"%s\" />\n", $compilerString? $compilerString : 'Not available');
$template .= sprintf("     <property name=\"compiler.debug\" value=\"on\" />\n");
$template .= sprintf("   </properties>\n");
for my $tst ( keys %tests ) {
	die ("Message Wrong Format") if($tests{$tst} !~ /[%]/ or $tests{$tst} =~ /[@].*[%]/ or $tst =~ /[@%]/ );
	my @descriptions = split('[@%]', $tests{$tst});
	my $partialtime = $descriptions[1] ? $descriptions[1] : 0;
	$template .= sprintf("   <testcase classname=\"%s\" name=\"%s\" time=\"%f\">\n", $tst, $descriptions[0], $partialtime );
	$template .= sprintf("     <failure message=\"%s failure\">%s</failure>\n", $tst, $descriptions[2]) if($descriptions[2]);
	$template .= sprintf("   </testcase>\n");
}
$template .= sprintf(" </testsuite>\n");
$template .= sprintf("</testsuites>\n");

print REPO $template;

close(REPO);

