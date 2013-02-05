#!/usr/bin/perl

my $dir = "101_ObjectCategories";
my $code = <<__END_OF_CODE__;
#pragma once
#include <string>

static const std::string path(".\\\\");
static const std::string file_info[][2] = {
__FILES__
};
__END_OF_CODE__

my @files = glob("$dir/*/*.jpg");
my $file_list = "";
foreach my $file (@files)
{
	next unless ($file=~m|$dir/([^/]*)/.*|);
	my $group = $1;
	$file=~s|/|\\\\|g;
	$file_list.="\t{\"$file\", \"$group\"},\n";
}

die "no image is found." if ($file_list eq '');

$code=~s/__FILES__/$file_list/;
print $code;
