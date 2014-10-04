#!/usr/bin/env perl 

use warnings;
use strict;

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
exit 0;

sub testFail{
	print STDERR "Test: fail - @_\n";
	exit -1;
}



