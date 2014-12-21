#!/usr/bin/env perl 

use warnings;
use strict;
use Time::HiRes qw(time);

my $milli0 = time;
my $milli1;
my $millit;
my $errors=0;
my $errorsString="testJsonArray|Integrity Values Test%ELAPSEDTIME|";

sub testFail;

my $sample='0:Open:1:OpenNew:Open New:2:ZoomIn:Zoom In:3:ZoomOut:Zoom Out:4:OriginalView:Original View:5:Quality:6:Pause:7:Mute:8:<null>:9:<null>:10:Find:Find...:11:FindAgain:Find Again:12:Copy:13:CopyAgain:Copy Again:14:CopySVG:Copy SVG:15:ViewSVG:View SVG:16:ViewSource:View Source:17:SaveAs:Save As:18:Help:19:About:About Adobe CVG Viewer...:';
my $output="";
my @array = ();

print STDERR "Array Data\n";

while (<>){
	chomp();
	$output= $output . $_;
}

testFail("Array Data Coerency") if($sample ne $output);

print STDERR "Test: pass\n";
$milli1 = time;
$millit = $milli1 - $milli0;
$errorsString =~ s/ELAPSEDTIME/$millit/;

system("perl testCasesScripts/genJunitReport.pl -n JsonArray -d \"$errorsString\" -t $millit -o .");
exit 0 if($errors == 0);
exit -1;

sub testFail{
        print STDERR "Test: fail - @_\n";
        $errorsString = $errorsString . @_ . "%0" . "\@" . @_ . " Error" . "|" ;
        $errors++;
}



