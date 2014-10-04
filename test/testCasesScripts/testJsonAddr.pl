#!/usr/bin/env perl 

use warnings;
use strict;

sub testFail;

my $node=9;
my $previousNode=11;
my $nextNode=13;
my $innerNode=15;
my $outerNode=17;
my $exitNode=19;
my $nullNode="0x0";

my $SOp=qx/uname/;

my $i=0;
my @array = ();

chomp($SOp);
$nullNode="(nil)" if( $SOp eq "Linux");

while (<>){
	chomp();
	push @{ $array[$i] }, split(/;/);
	$i++;
}

# ------ Tests ------------------------
testFail("Root Node Null") if( $array[0][$node] eq $nullNode);
testFail("Root Node Next") if( $array[0][$nextNode] eq $nullNode || $array[0][$innerNode] ne $nullNode);
# -------------------------------------
testFail("Root Next -> Menu Node") if( $array[1][$node] ne $array[0][$nextNode]);
testFail("Menu Node Prev") if( $array[1][$previousNode] ne $array[0][$node]);
testFail("Menu Node Next") if( $array[1][$nextNode] ne $nullNode);
testFail("Menu Node In") if( $array[1][$innerNode] eq $nullNode);
testFail("Menu Node Out") if( $array[1][$outerNode] ne $nullNode);
testFail("Menu Node Exit") if( $array[1][$exitNode] ne $nullNode);
# -------------------------------------
testFail("Title Node Prev") if( $array[2][$previousNode] ne $nullNode);
testFail("Title Node Next") if( $array[2][$nextNode] eq $nullNode);
testFail("Title Node In") if( $array[2][$innerNode] ne $nullNode);
testFail("Title Node Out") if( $array[2][$outerNode] ne $array[1][$node]);
testFail("Title Node Exit") if( $array[2][$exitNode] ne $nullNode);
# -------------------------------------
testFail("Items Node Addr") if( $array[3][$node] eq $nullNode);
testFail("Items Node Prev") if( $array[3][$previousNode] ne $array[2][$node]);
testFail("Items Node Next") if( $array[3][$nextNode] ne $nullNode);
testFail("Items Node In") if( $array[3][$innerNode] ne $array[4][$node]);
testFail("Items Node Out") if( $array[3][$outerNode] ne $nullNode);
testFail("Items Node Exit") if( $array[3][$exitNode] ne $nullNode);
# -------------------------------------
testFail("Object_Data_1 Node Addr") if( $array[4][$node] eq $nullNode);
testFail("Object_Data_1 Node Prev") if( $array[4][$previousNode] ne $nullNode);
testFail("Object_Data_1 Node Next") if( $array[4][$nextNode] ne $array[6][$node]);
testFail("Object_Data_1 Node In") if( $array[4][$innerNode] ne $array[5][$node]);
testFail("Object_Data_1 Node Out") if( $array[4][$outerNode] ne $array[3][$node]);
testFail("Object_Data_1 Node Exit") if( $array[4][$exitNode] ne $nullNode);
# -------------------------------------
testFail("Object_Data_1.id Node Addr") if( $array[5][$node] eq $nullNode);
testFail("Object_Data_1.id Node Prev") if( $array[5][$previousNode] ne $nullNode);
testFail("Object_Data_1.id Node Next") if( $array[5][$nextNode] ne $nullNode);
testFail("Object_Data_1.id Node In") if( $array[5][$innerNode] ne $nullNode);
testFail("Object_Data_1.id Node Out") if( $array[5][$outerNode] ne $array[4][$node]);
testFail("Object_Data_1.id Node Exit") if( $array[5][$exitNode] ne $array[6][$node]);
# -------------------------------------
testFail("Object_Data_2 Node Addr") if( $array[6][$node] eq $nullNode);
testFail("Object_Data_2 Node Prev") if( $array[6][$previousNode] ne $array[4][$node]);
testFail("Object_Data_2 Node Next") if( $array[6][$nextNode] ne $array[9][$node]);
testFail("Object_Data_2 Node In") if( $array[6][$innerNode] ne $array[7][$node]);
testFail("Object_Data_2 Node Out") if( $array[6][$outerNode] ne $nullNode);
testFail("Object_Data_2 Node Exit") if( $array[6][$exitNode] ne $nullNode);
# -------------------------------------
testFail("Object_Data_2.id Node Addr") if( $array[7][$node] eq $nullNode);
testFail("Object_Data_2.id Node Prev") if( $array[7][$previousNode] ne $nullNode);
testFail("Object_Data_2.id Node Next") if( $array[7][$nextNode] ne $array[8][$node]);
testFail("Object_Data_2.id Node In") if( $array[7][$innerNode] ne $nullNode);
testFail("Object_Data_2.id Node Out") if( $array[7][$outerNode] ne $array[6][$node]);
testFail("Object_Data_2.id Node Exit") if( $array[7][$exitNode] ne $nullNode);
# -------------------------------------
testFail("Object_Data_2.label Node Addr") if( $array[8][$node] eq $nullNode);
testFail("Object_Data_2.label Node Prev") if( $array[8][$previousNode] ne $array[7][$node]);
testFail("Object_Data_2.label Node Next") if( $array[8][$nextNode] ne $nullNode);
testFail("Object_Data_2.label Node In") if( $array[8][$innerNode] ne $nullNode);
testFail("Object_Data_2.label Node Out") if( $array[8][$outerNode] ne $array[6][$node]);
testFail("Object_Data_2.label Node Exit") if( $array[8][$exitNode] ne $array[9][$node]);
# -------------------------------------
testFail("Object_Data_3 Node Addr") if( $array[9][$node] eq $nullNode);
testFail("Object_Data_3 Node Prev") if( $array[9][$previousNode] ne $array[6][$node]);
testFail("Object_Data_3 Node Next") if( $array[9][$nextNode] ne $array[12][$node]);
testFail("Object_Data_3 Node In") if( $array[9][$innerNode] ne $array[10][$node]);
testFail("Object_Data_3 Node Out") if( $array[9][$outerNode] ne $nullNode);
testFail("Object_Data_3 Node Exit") if( $array[9][$exitNode] ne $nullNode);
# -------------------------------------
testFail("Object_Data_10 Node Addr") if( $array[26][$node] eq $nullNode);
testFail("Object_Data_10 Node Prev") if( $array[26][$previousNode] ne $array[24][$node]);
testFail("Object_Data_10 Node Next") if( $array[26][$nextNode] ne $array[27][$node]);
testFail("Object_Data_10 Node In") if( $array[26][$innerNode] ne $nullNode);
testFail("Object_Data_10 Node Out") if( $array[26][$outerNode] ne $nullNode);
testFail("Object_Data_10 Node Exit") if( $array[26][$exitNode] ne $nullNode);
# -------------------------------------
testFail("Object_Data_11 Node Addr") if( $array[27][$node] eq $nullNode);
testFail("Object_Data_11 Node Prev") if( $array[27][$previousNode] ne $array[26][$node]);
testFail("Object_Data_11 Node Next") if( $array[27][$nextNode] ne $array[30][$node]);
testFail("Object_Data_11 Node In") if( $array[27][$innerNode] ne $array[28][$node]);
testFail("Object_Data_11 Node Out") if( $array[27][$outerNode] ne $nullNode);
testFail("Object_Data_11 Node Exit") if( $array[27][$exitNode] ne $nullNode);
# -------------------------------------
testFail("Object_Data_13 Node Addr") if( $array[33][$node] eq $nullNode);
testFail("Object_Data_13 Node Prev") if( $array[33][$previousNode] ne $array[30][$node]);
testFail("Object_Data_13 Node Next") if( $array[33][$nextNode] ne $array[35][$node]);
testFail("Object_Data_13 Node In") if( $array[33][$innerNode] ne $array[34][$node]);
testFail("Object_Data_13 Node Out") if( $array[33][$outerNode] ne $nullNode);
testFail("Object_Data_13 Node Exit") if( $array[33][$exitNode] ne $nullNode);
# -------------------------------------
testFail("Object_Data_13.id Node Addr") if( $array[34][$node] eq $nullNode);
testFail("Object_Data_13.id Node Prev") if( $array[34][$previousNode] ne $nullNode);
testFail("Object_Data_13.id Node Next") if( $array[34][$nextNode] ne $nullNode);
testFail("Object_Data_13.id Node In") if( $array[34][$innerNode] ne $nullNode);
testFail("Object_Data_13.id Node Out") if( $array[34][$outerNode] ne $array[33][$node]);
testFail("Object_Data_13.id Node Exit") if( $array[34][$exitNode] ne $array[35][$node]);
# -------------------------------------
testFail("Object_Data_20 Node Addr") if( $array[52][$node] eq $nullNode);
testFail("Object_Data_20 Node Prev") if( $array[52][$previousNode] ne $array[50][$node]);
testFail("Object_Data_20 Node Next") if( $array[52][$nextNode] ne $nullNode);
testFail("Object_Data_20 Node In") if( $array[52][$innerNode] ne $array[53][$node]);
testFail("Object_Data_20 Node Out") if( $array[52][$outerNode] ne $nullNode);
testFail("Object_Data_20 Node Exit") if( $array[52][$exitNode] ne $nullNode);
# -------------------------------------
testFail("Object_Data_20.id Node Addr") if( $array[53][$node] eq $nullNode);
testFail("Object_Data_20.id Node Prev") if( $array[53][$previousNode] ne $nullNode);
testFail("Object_Data_20.id Node Next") if( $array[53][$nextNode] ne $array[54][$node]);
testFail("Object_Data_20.id Node In") if( $array[53][$innerNode] ne $nullNode);
testFail("Object_Data_20.id Node Out") if( $array[53][$outerNode] ne $array[52][$node]);
testFail("Object_Data_20.id Node Exit") if( $array[53][$exitNode] ne $nullNode);
# -------------------------------------
testFail("Object_Data_20.label Node Addr") if( $array[54][$node] eq $nullNode);
testFail("Object_Data_20.label Node Prev") if( $array[54][$previousNode] ne $array[53][$node]);
testFail("Object_Data_20.label Node Next") if( $array[54][$nextNode] ne $nullNode);
testFail("Object_Data_20.label Node In") if( $array[54][$innerNode] ne $nullNode);
testFail("Object_Data_20.label Node Out") if( $array[54][$outerNode] ne $array[52][$node]);
testFail("Object_Data_20.label Node Exit") if( $array[54][$exitNode] ne $array[55][$node]);
# -------------------------------------
testFail("Footer Node Addr") if( $array[55][$node] eq $nullNode);
testFail("Footer Node Prev") if( $array[55][$previousNode] ne $array[3][$node]);
testFail("Footer Node Next") if( $array[55][$nextNode] ne $nullNode);
testFail("Footer Node In") if( $array[55][$innerNode] ne $nullNode);
testFail("Footer Node Out") if( $array[55][$outerNode] ne $array[1][$node]);
testFail("Footer Node Exit") if( $array[55][$exitNode] ne $nullNode);
# -------------------------------------


print STDERR "Test: pass\n";
exit 0;

sub testFail{
	print STDERR "Test: fail - @_\n";
	exit -1;
}
